#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractSocket>
#include <QHash>

class QTcpServer;
class QTcpSocket;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // UI
    void handleModeChange();
    void handleSaveNickname();
    void handleCreateServer();
    void handleConnectToServer();
    void handleSendMessage();
    void handleShutdownServer();
    void handleDisconnectClient();

    // Сеть
    void onNewConnection();
    void onSocketReadyRead();
    void onSocketDisconnected();
    void onSocketStateChanged(QAbstractSocket::SocketState state);
    void displaySocketError(QAbstractSocket::SocketError socketError);

private:
    // ИЗМЕНЕНИЕ: Перечисление для типов сообщений
    enum MessageType {
        MyMessage,
        OtherMessage,
        SystemMessage
    };

    // --- Приватные функции ---
    void loadNickname();
    void saveNickname();
    QString getLocalIp();
    void broadcastMessage(const QByteArray &data, QTcpSocket *excludeClient = nullptr);
    void processBuffer(QTcpSocket* socket);

    // ИЗМЕНЕНИЕ: Сигнатура функции обновлена
    void appendToChat(const QString& text, MessageType type);


    // --- Поля класса ---
    Ui::MainWindow *ui;
    QTcpServer* m_server;
    QTcpSocket* m_socket;
    QList<QTcpSocket*> m_clients;
    QHash<QTcpSocket*, QByteArray> m_buffers;
};

#endif // MAINWINDOW_H