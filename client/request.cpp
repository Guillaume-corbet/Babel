#include "request.h"
#include "ui_request.h"

Request::Request(QWidget *parent, QTcpSocket *serveur, const QString *username1, const QString *username2) :
    QWidget(parent),
    ui(new Ui::Request),
    _serveur(serveur),
    usernameConnected(username1)
{
    usernameFriend = new QString("");
    usernameFriend->append(username2);
    ui->setupUi(this);
    ui->username->setText(*username2);
}

Request::~Request()
{
    delete(usernameFriend);
    delete ui;
}

void Request::on_acceptRequestButton_pressed()
{
    std::string *tosend = new std::string("ACCEPTFRIENDREQUEST ");
    tosend->append(usernameConnected->toLocal8Bit().constData());
    tosend->append(" ");
    tosend->append(usernameFriend->toLocal8Bit().constData());
    tosend->append("\n");
    qDebug() << "To send = " << tosend->c_str() << "\n";
    _serveur->write(tosend->c_str());
    delete (tosend);
}

void Request::on_cancelRequestButton_pressed()
{
    std::string *tosend = new std::string("REFUSEFRIENDREQUEST ");
    tosend->append(usernameConnected->toLocal8Bit().constData());
    tosend->append(" ");
    tosend->append(usernameFriend->toLocal8Bit().constData());
    tosend->append("\n");
    _serveur->write(tosend->c_str());
    delete (tosend);
}
