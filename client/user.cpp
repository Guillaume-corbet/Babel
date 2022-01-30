#include "user.h"
#include "ui_user.h"

User::User(QWidget *parent, const QString *username, int status, QTcpSocket *socket, const QString *mainusername) :
    QWidget(parent),
    ui(new Ui::User),
    _status(status),
    server(socket)

{
    _username = new QString(*username);
    mainUser = new QString(*mainusername);
    ui->setupUi(this);
    qDebug() << "Status = " << status << "\n";
    switch (status)
    {
        case 0:
            ui->statuts->setPixmap(*(new QPixmap(":/Ressources/offlineIcon.png")));
            ui->callButton->setHidden(true);
            break;
        case 1:
            ui->statuts->setPixmap(*(new QPixmap(":/Ressources/online.png")));
            ui->callButton->setHidden(false);
            break;
        case 2:
            ui->statuts->setPixmap(*(new QPixmap(":/Ressources/idleIcon.png")));
            ui->callButton->setHidden(true);
    }
    ui->username->setText(*username);
}

User::~User()
{
    delete ui;
}

void User::on_callButton_pressed()
{
    std::string *tosend = new std::string("STARTCALL ");

    tosend->append(mainUser->toLocal8Bit().constData());
    tosend->append(" ");
    tosend->append(_username->toLocal8Bit().constData());
    tosend->append(" 8081\n");
    delete(tosend);
}

void User::on_deleteButton_pressed()
{
    std::string *tosend = new std::string("DELETEFRIEND ");

    tosend->append(mainUser->toLocal8Bit().constData());
    tosend->append(" ");
    tosend->append(_username->toLocal8Bit().constData());
    tosend->append("\n");
    server->write(tosend->c_str());
    delete (tosend);
}
