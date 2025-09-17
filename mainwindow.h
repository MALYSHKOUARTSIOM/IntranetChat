
#ifndef INTRANETCHAT_MAINWINDOW_H
#define INTRANETCHAT_MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void updateUiMode();

    void handleSaveNickname();
    void handleCreateServer();
    void handleConnectToServer();
    void handleSendMessage();
private:
    Ui::MainWindow *ui;

    void loadNickname();
    void saveNickname();

    QString getLocalIp();
};


#endif
