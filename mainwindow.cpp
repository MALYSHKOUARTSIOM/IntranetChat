
#include "mainwindow.h"
#include "ui_MainWindow.h"
#include <QNetworkInterface>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->serverRadioButton, &QRadioButton::toggled, this, &MainWindow::updateUiMode);
    connect(ui->clientRadioButton, &QRadioButton::toggled, this, &MainWindow::updateUiMode);

    connect(ui->saveNicknameButton, &QPushButton::clicked, this, &MainWindow::handleSaveNickname);

    connect(ui->createButton, &QPushButton::clicked, this, &MainWindow::handleCreateServer);
    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::handleConnectToServer);


    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::handleSendMessage);
    connect(ui->messageLineEdit, &QLineEdit::returnPressed, this, &MainWindow::handleSendMessage);

    loadNickname();
    updateUiMode();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::handleSaveNickname()
{
    saveNickname();
}

void MainWindow::handleCreateServer()
{
    // Здесь будет логика создания сервера
    ui->chatBrowser->append("ЛОГИКА: Нажата кнопка 'Создать диалог'. Сервер будет запущен...");
    // (Сетевой код пойдет сюда)
}

void MainWindow::handleConnectToServer()
{
    // Здесь будет логика подключения к серверу
    ui->chatBrowser->append("ЛОГИКА: Нажата кнопка 'Подключиться'. Попытка подключения к " + ui->serverIpLineEdit->text());
    // (Сетевой код пойдет сюда)
}

void MainWindow::handleSendMessage()
{
    QString nickname = ui->nicknameLineEdit->text().trimmed();
    if (nickname.isEmpty()) {
        nickname = "Аноним";
    }

    QString messageText = ui->messageLineEdit->text().trimmed();
    if(messageText.isEmpty()){
        return; // Не отправляем пустые сообщения
    }

    QString fullMessage = nickname + ": " + messageText;

    ui->chatBrowser->append("ЛОГИКА: Отправка сообщения: " + fullMessage);
    // (Сетевой код для отправки сообщения пойдет сюда)

    ui->messageLineEdit->clear();
    ui->messageLineEdit->setFocus();
}


// --- Реализация приватных функций ---

void MainWindow::saveNickname()
{
    QString nickname = ui->nicknameLineEdit->text().trimmed();
    if (nickname.isEmpty()) {
        QMessageBox::warning(this, "Error", "The nickname field cannot be empty.");
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
        ui->statusbar->showMessage("The nickname was saved successfully!", 3000);
    } else {
        QMessageBox::critical(this, "Error", "The file with the nickname could not be saved.");
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
void MainWindow::updateUiMode()
{
    if (ui->serverRadioButton->isChecked()) {
        ui->stackedWidget->setCurrentWidget(ui->page_server);
        ui->ipAddressLabel->setText("Ваш IP в сети: " + getLocalIp());
    } else {
        ui->stackedWidget->setCurrentWidget(ui->page_client);
    }
}

QString MainWindow::getLocalIp()
{
    const QList<QHostAddress> allAddresses = QNetworkInterface::allAddresses();
    for (const QHostAddress &address : allAddresses) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress::LocalHost) {
            return address.toString();
        }
    }
    return "Not found";
}