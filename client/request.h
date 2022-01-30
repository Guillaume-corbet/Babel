#ifndef REQUEST_H
#define REQUEST_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Request;
}

class Request : public QWidget
{
    Q_OBJECT

public:
    explicit Request(QWidget *parent = nullptr, QTcpSocket *serveur = nullptr, const QString *username1 = nullptr, const QString *username2 = nullptr);
    ~Request();

private slots:

    void on_acceptRequestButton_pressed();

    void on_cancelRequestButton_pressed();

private:
    Ui::Request *ui;
    QTcpSocket *_serveur;
    const QString *usernameConnected;
    QString *usernameFriend;
};

#endif // REQUEST_H
