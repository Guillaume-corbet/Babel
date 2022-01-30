#ifndef SEARCHFRIEND_H
#define SEARCHFRIEND_H

#include <QDialog>
#include <QTcpSocket>

namespace Ui {
class searchFriend;
}

class searchFriend : public QDialog
{
    Q_OBJECT

public:
    explicit searchFriend(QWidget *parent = nullptr, QTcpSocket *socket = nullptr, QString *username = nullptr);
    ~searchFriend();
    void updateResult(QString *message, bool error);
    void reject();

private:
    Ui::searchFriend *ui;
    QTcpSocket *serveur;
    QString *_username;
    QPushButton *cancelButton;
    QPushButton *inviteButton;
private slots:
    void sendFriendRequest();
    void cancel();
    void onClose();
};

#endif // SEARCHFRIEND_H
