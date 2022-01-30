#include "call.h"
#include "ui_call.h"

Call::Call(QWidget *parent, const QString *caller, const QString *user, QTcpSocket *socket) :
    QWidget(parent),
    ui(new Ui::Call),
    server(socket)
{
    username = new QString(*user);
    callerName = new QString(*caller);
    ui->callerName->setText(*caller);
    ui->setupUi(this);
}

Call::~Call()
{
    delete ui;
}
