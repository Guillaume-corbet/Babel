#include "searchfriend.h"
#include "ui_searchfriend.h"

searchFriend::searchFriend(QWidget *parent, QTcpSocket *socket, QString *username) :
    QDialog(parent),
    ui(new Ui::searchFriend),
    serveur(socket),
    _username(username)
{
    ui->setupUi(this);
    cancelButton = ui->cancelRequest;
    inviteButton = ui->sendFriend;
    connect(ui->sendFriend, SIGNAL(pressed()), this, SLOT(sendFriendRequest()));
    connect(ui->cancelRequest, SIGNAL(pressed()), this, SLOT(cancel()));
}

searchFriend::~searchFriend()
{
    delete ui;
}

void searchFriend::updateResult(QString *message, bool error)
{
    cancelButton->setDisabled(false);
    inviteButton->setDisabled(false);
    ui->nameFriend->setDisabled(false);
    ui->statusMessage->setText(*message);
    if (error)
    {
        ui->statusMessage->setStyleSheet("QLabel {color : red;}");
    }
    else {
        ui->statusMessage->setStyleSheet("QLabel {color : green;}");
    }
}

void searchFriend::sendFriendRequest()
{
    std::string *tosend = new std::string();

    if (ui->nameFriend->text() != ""){
        cancelButton->setDisabled(true);
        inviteButton->setDisabled(true);
        ui->nameFriend->setDisabled(true);
        ui->statusMessage->setText("");

        tosend->append("ADDFRIEND ");
        tosend->append(_username->toLocal8Bit().constData());
        tosend->append(" ");
        tosend->append(ui->nameFriend->text().toLocal8Bit().constData());
        tosend->append("\n");
        serveur->write(tosend->c_str());
    }
    delete(tosend);
}

void searchFriend::cancel()
{
    this->close();
}

void searchFriend::reject()
{
    cancelButton->setDisabled(false);
    inviteButton->setDisabled(false);
    ui->nameFriend->setDisabled(false);
    QDialog::reject();
}

void searchFriend::onClose()
{
    cancelButton->setDisabled(false);
    inviteButton->setDisabled(false);
    ui->nameFriend->setDisabled(false);
}
