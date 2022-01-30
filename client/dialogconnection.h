#ifndef DIALOGCONNECTION_H
#define DIALOGCONNECTION_H

#include <QDialog>
#include "mainwindow.h"
#include <iostream>

namespace Ui {
class DialogConnection;
}

class DialogConnection : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConnection(QWidget *parent = nullptr, MainWindow *main = nullptr);
    ~DialogConnection();

public slots:
    void connectPress();
    void cancelPress();

protected:

private:
    Ui::DialogConnection *ui;
    MainWindow *_main;
};

#endif // DIALOGCONNECTION_H
