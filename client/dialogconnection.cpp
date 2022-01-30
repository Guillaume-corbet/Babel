#include "dialogconnection.h"
#include "ui_dialogconnection.h"

DialogConnection::DialogConnection(QWidget *parent, MainWindow *main) :
    QDialog(parent),
    ui(new Ui::DialogConnection),
    _main(main)
{
    ui->setupUi(this);
    connect(ui->connectButton, SIGNAL(pressed()), this, SLOT(connectPress()));
    connect(ui->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPress()));
}

DialogConnection::~DialogConnection()
{
    delete ui;
}

void DialogConnection::connectPress()
{
    //connection to the server
    this->close();
    _main->Connection(ui->ipAddress->text());
}

void DialogConnection::cancelPress()
{
    this->close();
}
