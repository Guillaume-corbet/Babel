#ifndef CALL_H
#define CALL_H

#include <QWidget>
#include <QTcpSocket>
#include <QUdpSocket>

namespace Ui {
class Call;
}

class Call : public QWidget
{
    Q_OBJECT

public:
    explicit Call(QWidget *parent = nullptr, const QString *caller = nullptr, const QString *user = nullptr, QTcpSocket *socket = nullptr);
    ~Call();

private:
    Ui::Call *ui;
    QTcpSocket *server;
    QString *username;
    QString *callerName;
};

#endif // CALL_H
