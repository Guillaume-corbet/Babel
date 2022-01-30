#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QtNetwork/QTcpSocket>
#include <iostream>
#include "./searchfriend.h"
#include <map>
#include "./request.h"
#include "./user.h"
#include <QHBoxLayout>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Connection(QString connectionInformation);

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString *mootd;
    QString *username;
    QString *lastMessage;
    searchFriend *searchUI;
    bool syncAll;
    void friendRequestSucces();
    void friendRequestAlreadyExist();
    void friendRequestFriendAlreadyExist();
    void friendRequestNotExist();
    void friendRequestError();
    void loggedIn();
    void loginError();
    void loginInternalError();
    void getFriendRequestSuccess();
    void getFriendSuccess();
    void createContactSuccess();
    void deleteRequestSuccess();
    void logoutSuccess();
    std::map<std::string, void (MainWindow::*)()> command;
    QTimer *timer;
private slots:
    void online();
    void offline();
    void receive();
    void getError(QAbstractSocket::SocketError error);
    void logIn();
    void signIn();
    void on_actionSearchForFriend_triggered();
    void on_disconnectButton_pressed();
    void updateInfo();
};
#endif // MAINWINDOW_H
