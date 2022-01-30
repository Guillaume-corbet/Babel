#include "mainwindow.h"
#include "dialogconnection.h"
#include <QApplication>
#include "user.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    DialogConnection test(nullptr, &w);
    w.setWindowTitle("TeamSpeak4");
    w.show();
    test.setWindowTitle("Connect to a server");
    test.show();
    return a.exec();
}
