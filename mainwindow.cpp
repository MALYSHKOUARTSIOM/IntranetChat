#include "mainwindow.h"
#include "ui_mainwindow.h"

// Все необходимые для проекта Qt-классы
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDir>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QDateTime>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent)
        , ui(new Ui::MainWindow)
        , m_server(nullptr)
        , m_socket(nullptr)
{
    ui->setupUi(this);

    // Устанавливаем коэффициенты растяжения для главного лэйаута
    ui->horizontalLayout_3->setStretch(0, 3);
    ui->horizontalLayout_3->setStretch(1, 7);

    // --- НОВЫЙ ДИЗАЙН С ПОМОЩЬЮ QSS ---
    QString styleSheet = R"(
        QWidget#centralwidget { background-color: #2c3e50; }
        QGroupBox { color: #ecf0f1; font-weight: bold; }
        QLabel { color: #ecf0f1; }
        QPushButton { background-color: #3498db; color: white; border: none; padding: 8px 16px; border-radius: 5px; font-weight: bold; }
        QPushButton:hover { background-color: #2980b9; }
        QPushButton:disabled { background-color: #566573; }
        QLineEdit, QTextBrowser { background-color: #34495e; border: 1px solid #566573; color: #ecf0f1; border-radius: 5px; padding: 5px; }
        QLineEdit:focus { border: 1px solid #3498db; }
        QRadioButton { color: #ecf0f1; }
        QStatusBar { color: white; }
    )";
    this->setStyleSheet(styleSheet);
    // ------------------------------------

    // --- Соединяем сигналы и слоты ---
    connect(ui->serverRadioButton, &QRadioButton::toggled, this, &MainWindow::handleModeChange);
    connect(ui->clientRadioButton, &QRadioButton::toggled, this, &MainWindow::handleModeChange);
    connect(ui->saveNicknameButton, &QPushButton::clicked, this, &MainWindow::handleSaveNickname);
    connect(ui->createButton, &QPushButton::clicked, this, &MainWindow::handleCreateServer);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::handleConnectToServer);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::handleSendMessage);
    connect(ui->messageLineEdit, &QLineEdit::returnPressed, this, &MainWindow::handleSendMessage);
    connect(ui->shutdownServerButton, &QPushButton::clicked, this, &MainWindow::handleShutdownServer);
    connect(ui->disconnectClientButton, &QPushButton::clicked, this, &MainWindow::handleDisconnectClient);

    // --- Начальная настройка ---
    loadNickname();
    handleModeChange();
    ui->shutdownServerButton->hide();
    ui->disconnectClientButton->hide();
    ui->serverPortLineEdit->setText("12345");
    ui->clientPortLineEdit->setText("12345");
}

MainWindow::~MainWindow()
{
    delete ui;
}

// --- Реализация слотов для UI ---

void MainWindow::handleModeChange()
{
    if (ui->serverRadioButton->isChecked()) {
        ui->connectionStackedWidget->setCurrentWidget(ui->page_server);
        ui->ipAddressLabel->setText("Ваш IP в сети: " + getLocalIp());
    } else {
        ui->connectionStackedWidget->setCurrentWidget(ui->page_client);
    }
}

void MainWindow::handleSaveNickname()
{
    saveNickname();
}

void MainWindow::handleCreateServer()
{
    if (m_server) return;

    bool ok;
    quint16 port = ui->serverPortLineEdit->text().toUShort(&ok);
    if (!ok || port == 0) {
        QMessageBox::warning(this, "Неверный порт", "Пожалуйста, введите корректный номер порта (1-65535).");
        return;
    }

    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &MainWindow::onNewConnection);

    if (!m_server->listen(QHostAddress::Any, port)) {
        QMessageBox::critical(this, "Ошибка сервера", "Не удалось запустить сервер: " + m_server->errorString());
        delete m_server; m_server = nullptr;
        return;
    }

    appendToChat(QString("--- Сервер успешно запущен на порту %1 ---").arg(port), SystemMessage);
    appendToChat("--- Ожидание подключений... ---", SystemMessage);

    ui->createButton->hide();
    ui->shutdownServerButton->show();
    ui->connectButton->setEnabled(false);
    ui->serverRadioButton->setEnabled(false);
    ui->clientRadioButton->setEnabled(false);
    ui->serverPortLineEdit->setEnabled(false);
}

void MainWindow::handleConnectToServer()
{
    if (m_socket) return;

    QString ip = ui->serverIpLineEdit->text().trimmed();
    if (ip.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите IP-адрес сервера.");
        return;
    }

    bool ok;
    quint16 port = ui->clientPortLineEdit->text().toUShort(&ok);
    if (!ok || port == 0) {
        QMessageBox::warning(this, "Неверный порт", "Пожалуйста, введите корректный номер порта (1-65535).");
        return;
    }

    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::onSocketReadyRead);
    connect(m_socket, &QTcpSocket::disconnected, this, &MainWindow::onSocketDisconnected);
    connect(m_socket, &QTcpSocket::stateChanged, this, &MainWindow::onSocketStateChanged);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred), this, &MainWindow::displaySocketError);

    appendToChat(QString("--- Попытка подключения к %1:%2 ---").arg(ip).arg(port), SystemMessage);
    m_socket->connectToHost(ip, port);

    ui->connectButton->setEnabled(false);
}

void MainWindow::handleSendMessage()
{
    QString nickname = ui->nicknameLineEdit->text().trimmed();
    if (nickname.isEmpty()) {
        QMessageBox::warning(this, "Внимание", "Пожалуйста, введите и сохраните никнейм.");
        return;
    }

    QString messageText = ui->messageLineEdit->text().trimmed();
    if (messageText.isEmpty()) return;

    qint64 timestamp = QDateTime::currentSecsSinceEpoch();
    QByteArray encodedNickname = nickname.toUtf8().toBase64();
    QByteArray encodedMessage = messageText.toUtf8().toBase64();

    QString packetString = QString("%1:%2:%3\n")
            .arg(timestamp)
            .arg(QString::fromLatin1(encodedNickname))
            .arg(QString::fromLatin1(encodedMessage));
    QByteArray dataToSend = packetString.toUtf8();

    if (m_server) {
        QString timeString = QDateTime::fromSecsSinceEpoch(timestamp).toString("yyyy-MM-dd hh:mm:ss");
        QString displayString = QString("[%1] %2: %3").arg(timeString).arg(nickname).arg(messageText);
        appendToChat(displayString, MyMessage);
        broadcastMessage(dataToSend, nullptr);
    } else if (m_socket && m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->write(dataToSend);
    }

    ui->messageLineEdit->clear();
    ui->messageLineEdit->setFocus();
}

void MainWindow::handleShutdownServer()
{
    if (!m_server) return;
    appendToChat("--- Сервер выключается... ---", SystemMessage);
    for (QTcpSocket *client : qAsConst(m_clients)) {
        client->disconnectFromHost();
    }
    m_clients.clear();
    m_buffers.clear();
    m_server->close();
    delete m_server;
    m_server = nullptr;
    appendToChat("--- Сервер остановлен ---", SystemMessage);
    ui->shutdownServerButton->hide();
    ui->createButton->show();
    ui->createButton->setEnabled(true);
    ui->serverRadioButton->setEnabled(true);
    ui->clientRadioButton->setEnabled(true);
    ui->serverPortLineEdit->setEnabled(true);
    ui->connectButton->setEnabled(true);
}

void MainWindow::handleDisconnectClient()
{
    if (m_socket && m_socket->state() == QAbstractSocket::ConnectedState) {
        appendToChat("--- Отключение от сервера... ---", SystemMessage);
        m_socket->disconnectFromHost();
    }
}


// --- Реализация сетевых слотов ---

void MainWindow::onNewConnection()
{
    while (m_server->hasPendingConnections()) {
        QTcpSocket *clientSocket = m_server->nextPendingConnection();
        m_clients.append(clientSocket);
        m_buffers.insert(clientSocket, QByteArray());
        connect(clientSocket, &QTcpSocket::readyRead, this, &MainWindow::onSocketReadyRead);
        connect(clientSocket, &QTcpSocket::disconnected, this, &MainWindow::onSocketDisconnected);

        // ИСПРАВЛЕНИЕ: Преобразуем адрес в чистый IPv4
        QHostAddress clientAddress = clientSocket->peerAddress();
        QString ipString = clientAddress.toString();
        if (ipString.startsWith("::ffff:")) {
            ipString = ipString.mid(7); // Отрезаем префикс "::ffff:"
        }

        appendToChat(QString("--- Новый клиент подключился: %1 ---").arg(ipString), SystemMessage);
    }
}

void MainWindow::onSocketReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;
    m_buffers[socket].append(socket->readAll());
    processBuffer(socket);
}

void MainWindow::onSocketDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    if (m_clients.contains(socket)) {
        // ИСПРАВЛЕНИЕ: Также исправляем формат IP здесь
        QHostAddress clientAddress = socket->peerAddress();
        QString ipString = clientAddress.toString();
        if (ipString.startsWith("::ffff:")) {
            ipString = ipString.mid(7);
        }
        appendToChat(QString("--- Клиент отключился: %1 ---").arg(ipString), SystemMessage);
        m_clients.removeAll(socket);
        m_buffers.remove(socket);
    } else if (socket == m_socket) {
        m_socket = nullptr;
    }
    socket->deleteLater();
}

void MainWindow::onSocketStateChanged(QAbstractSocket::SocketState state)
{
    if (state == QAbstractSocket::ConnectedState) {
        appendToChat("--- Успешно подключено к серверу! ---", SystemMessage);
        ui->connectButton->hide();
        ui->disconnectClientButton->show();
        ui->serverRadioButton->setEnabled(false);
        ui->clientRadioButton->setEnabled(false);
    } else if (state == QAbstractSocket::UnconnectedState) {
        if (ui->clientRadioButton->isChecked()) {
            if (!ui->connectButton->isEnabled()) {
                appendToChat("--- Соединение потеряно ---", SystemMessage);
            }
            ui->disconnectClientButton->hide();
            ui->connectButton->show();
            ui->connectButton->setEnabled(true);
            ui->serverRadioButton->setEnabled(true);
            ui->clientRadioButton->setEnabled(true);
        }
    }
}

void MainWindow::displaySocketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if(socket) {
        QMessageBox::critical(this, "Ошибка сети", socket->errorString());
        ui->connectButton->setEnabled(true);
    }
}


// --- Реализация приватных функций ---

void MainWindow::processBuffer(QTcpSocket* socket)
{
    while (m_buffers[socket].contains('\n')) {
        int pos = m_buffers[socket].indexOf('\n');
        QByteArray packet = m_buffers[socket].left(pos);
        m_buffers[socket].remove(0, pos + 1);

        QList<QByteArray> parts = packet.split(':');
        if (parts.size() != 3) {
            qWarning() << "Получен поврежденный пакет:" << packet;
            continue;
        }

        bool ok;
        qint64 timestamp = parts[0].toLongLong(&ok);
        if (!ok) continue;

        QString nickname = QString::fromUtf8(QByteArray::fromBase64(parts[1]));
        QString message = QString::fromUtf8(QByteArray::fromBase64(parts[2]));

        QString timeString = QDateTime::fromSecsSinceEpoch(timestamp).toString("yyyy-MM-dd hh:mm:ss");
        QString displayString = QString("[%1] %2: %3").arg(timeString).arg(nickname).arg(message);

        if (nickname == ui->nicknameLineEdit->text().trimmed()) {
            appendToChat(displayString, MyMessage);
        } else {
            appendToChat(displayString, OtherMessage);
        }

        if (m_server) {
            broadcastMessage(packet + '\n', socket);
        }
    }
}

void MainWindow::appendToChat(const QString& text, MessageType type)
{
    // Получаем курсор для работы с документом
    QTextCursor cursor(ui->chatBrowser->document());
    // Перемещаем курсор в самый конец документа
    cursor.movePosition(QTextCursor::End);

    // Создаем формат для блока (абзаца)
    QTextBlockFormat blockFormat;
    // Создаем формат для символов (текста)
    QTextCharFormat charFormat;

    // Настраиваем форматы в зависимости от типа сообщения
    switch (type) {
        case MyMessage:
            blockFormat.setAlignment(Qt::AlignRight); // Выравнивание блока по правому краю
            charFormat.setForeground(QColor("#aed6f1")); // Цвет текста
            break;
        case OtherMessage:
            blockFormat.setAlignment(Qt::AlignLeft); // Выравнивание блока по левому краю
            charFormat.setForeground(QColor("#ecf0f1")); // Цвет текста
            break;
        case SystemMessage:
            blockFormat.setAlignment(Qt::AlignCenter); // Выравнивание блока по центру
            charFormat.setForeground(QColor("#f1c40f")); // Цвет текста
            charFormat.setFontWeight(QFont::Bold); // Жирный шрифт
            break;
    }


    cursor.setBlockFormat(blockFormat);

    cursor.setCharFormat(charFormat);

    cursor.insertText(text + "\n");

    ui->chatBrowser->ensureCursorVisible();
}

void MainWindow::broadcastMessage(const QByteArray &data, QTcpSocket *excludeClient)
{
    for (QTcpSocket *client : qAsConst(m_clients)) {
        if (client != excludeClient) {
            client->write(data);
        }
    }
}

void MainWindow::saveNickname()
{
    QString nickname = ui->nicknameLineEdit->text().trimmed();
    if (nickname.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Поле никнейма не может быть пустым.");
        return;
    }
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    QFile file(path + "/nickname.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << nickname;
        file.close();
        ui->statusbar->showMessage("Никнейм успешно сохранен!", 3000);
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось сохранить файл с никнеймом.");
    }
}

void MainWindow::loadNickname()
{
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile file(path + "/nickname.txt");
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }
    QTextStream in(&file);
    QString nickname = in.readLine();
    file.close();
    if (!nickname.isEmpty()) {
        ui->nicknameLineEdit->setText(nickname);
    }
}

QString MainWindow::getLocalIp()
{
    const QList<QNetworkInterface> allInterfaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &interface : allInterfaces) {
        if (interface.type() == QNetworkInterface::Wifi && interface.flags().testFlag(QNetworkInterface::IsUp)) {
            for (const QNetworkAddressEntry &entry : interface.addressEntries()) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    return entry.ip().toString();
                }
            }
        }
    }
    for (const QNetworkInterface &interface : allInterfaces) {
        if (interface.type() == QNetworkInterface::Ethernet && interface.flags().testFlag(QNetworkInterface::IsUp)) {
            for (const QNetworkAddressEntry &entry : interface.addressEntries()) {
                if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    return entry.ip().toString();
                }
            }
        }
    }
    const QHostAddress &localhost = QHostAddress(QHostAddress::LocalHost);
    for (const QHostAddress &address: QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != localhost) {
            return address.toString();
        }
    }
    return "Не найден";
}