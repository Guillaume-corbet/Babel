#ifndef USER_H
#define USER_H

#include <QWidget>
#include <QDebug>
#include <QTcpSocket>

namespace Ui {
class User;
}

class User : public QWidget
{
    Q_OBJECT

public:
    explicit User(QWidget *parent = nullptr, const QString *username = nullptr, int status = 0, QTcpSocket *socket = nullptr, const QString *mainusername = nullptr);
    ~User();

private slots:
    void on_callButton_pressed();

    void on_deleteButton_pressed();

private:
    Ui::User *ui;
    int _status;
    QString *_username;
    QString *mainUser;
    QTcpSocket *server;
};

#endif // USER_H
