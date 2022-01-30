#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    syncAll = true;
    socket = new QTcpSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(receive()));
    connect(socket, SIGNAL(connected()), this, SLOT(online()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(offline()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(getError(QAbstractSocket::SocketError)));
    connect(ui->logInButton, SIGNAL(pressed()), this, SLOT(logIn()));
    connect(ui->signInButton, SIGNAL(pressed()), this, SLOT(signIn()));
    ui->logInButton->setHidden(true);
    ui->passwordText->setHidden(true);
    ui->signInButton->setHidden(true);
    ui->usernameText->setHidden(true);
    ui->friendConnectedList->setHidden(true);
    ui->friendDisconnectedList->setHidden(true);
    ui->friendRequestList->setHidden(true);
    ui->disconnectButton->setHidden(true);
    ui->callList->setHidden(true);
    mootd = new QString("");
    username = new QString("");
    searchUI = new searchFriend(this, socket, username);
    command["10"] = &MainWindow::loggedIn;
    command["11"] = &MainWindow::loginError;
    command["19"] = &MainWindow::loginInternalError;
    command["20"] = &MainWindow::logoutSuccess;
    command["30"] = &MainWindow::loggedIn;
    command["40"] = &MainWindow::createContactSuccess;
    command["50"] = &MainWindow::friendRequestSucces;
    command["51"] = &MainWindow::friendRequestAlreadyExist;
    command["52"] = &MainWindow::friendRequestFriendAlreadyExist;
    command["53"] = &MainWindow::friendRequestNotExist;
    command["59"] = &MainWindow::friendRequestError;
    command["60"] = &MainWindow::deleteRequestSuccess;
    command["120"] = &MainWindow::getFriendSuccess;
    command["130"] = &MainWindow::getFriendRequestSuccess;
    lastMessage = new QString("");
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateInfo);
}

MainWindow::~MainWindow()
{
    socket->close();
    delete(mootd);
    delete(username);
    delete(lastMessage);
    delete(searchUI);
    delete(socket);
    delete ui;
}

void MainWindow::Connection(QString connectionInformation)
{
    QStringList list = connectionInformation.split(QLatin1Char(':'));
    socket->connectToHost(list.at(0), list.at(1).toUInt());
}

void MainWindow::online()
{
    std::cout << "Connected to server" << std::endl;
}

void MainWindow::offline()
{

}

void MainWindow::receive()
{
    QString *data = new QString();
    QStringList list;

    if (mootd->length() == 0)
    {
        mootd->append(socket->readAll());
        ui->welcomeLabel->setText(*mootd);
        qDebug() << "-Serveur : " << *mootd << "\n";
        ui->logInButton->setHidden(false);
        ui->passwordText->setHidden(false);
        ui->signInButton->setHidden(false);
        ui->usernameText->setHidden(false);
    }
    else {
        lastMessage->clear();
        data->append(QString::fromUtf8(socket->readAll()));
        lastMessage->append(data);
        list = data->split(QLatin1Char(' '));
        auto f = command.find(list.at(0).toLocal8Bit().constData());
        if (f != command.end())
        {
            (this->*f->second)();
        }
        qDebug() << "-Serveur : " << data->toLocal8Bit().constData() << "\n";
    }
    delete(data);
}

void MainWindow::getError(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket erreur : " << error;
}

void MainWindow::signIn()
{
    std::string *tosend = new std::string("CREATEACCOUNT ");
    tosend->append(ui->usernameText->text().toLocal8Bit().constData());
    tosend->append(" ");
    tosend->append(ui->passwordText->text().toLocal8Bit().constData());
    tosend->append("\n");

    if (ui->usernameText->text() != "" && ui->passwordText->text() != "")
    {
        username->clear();
        username->append(ui->usernameText->text());
        socket->write(tosend->c_str());
    }
    delete(tosend);
}

void MainWindow::logIn()
{
    std::string *tosend = new std::string("LOGIN ");
    tosend->append(ui->usernameText->text().toLocal8Bit().constData());
    tosend->append(" ");
    tosend->append(ui->passwordText->text().toLocal8Bit().constData());
    tosend->append("\n");
    if (ui->usernameText->text() != "" && ui->passwordText->text() != "")
    {
        username->clear();
        username->append(ui->usernameText->text());
        socket->write(tosend->c_str());
    }
    delete(tosend);
}

void MainWindow::on_actionSearchForFriend_triggered()
{
    searchUI->show();
}

void MainWindow::friendRequestSucces()
{
    searchUI->updateResult(new QString("Friend request send succesfully !"), false);
}
void MainWindow::friendRequestAlreadyExist()
{
    searchUI->updateResult(new QString("You have already send a request to this friend"), true);
}

void MainWindow::friendRequestFriendAlreadyExist()
{
    searchUI->updateResult(new QString("This friend already exist in your friends list"), true);
}

void MainWindow::friendRequestNotExist()
{
    searchUI->updateResult(new QString("This username doesn't exist"), true);
}

void MainWindow::friendRequestError()
{
    searchUI->updateResult(new QString("An internal error as occured, please retry later"), true);
}

void MainWindow::loggedIn()
{
    std::string *tosend = new std::string("GETFRIEND ");

    tosend->append(username->toLocal8Bit().constData());
    tosend->append("\n");
    socket->write(tosend->c_str());
    ui->friendConnectedList->setHidden(false);
    ui->friendDisconnectedList->setHidden(false);
    ui->logInButton->setHidden(true);
    ui->passwordText->setHidden(true);
    ui->signInButton->setHidden(true);
    ui->usernameText->setHidden(true);
    ui->welcomeLabel->setHidden(true);
    ui->errorLog->setHidden(true);
    ui->callList->setHidden(false);
    ui->friendRequestList->setHidden(false);
    ui->disconnectButton->setHidden(false);
    ui->friendConnectedList->clear();
    ui->friendDisconnectedList->clear();
    ui->friendRequestList->clear();
    delete(tosend);
    timer->start(10000);
}

void MainWindow::loginError()
{
    ui->errorLog->setText("Incorrect Username/Password.");
    ui->errorLog->setStyleSheet("QLabel {color : red;}");
}

void MainWindow::loginInternalError()
{
    ui->errorLog->setText("An internal error as occured, please retry later");
    ui->errorLog->setStyleSheet("QLabel {color : red;}");
}

void MainWindow::getFriendRequestSuccess()
{
    QStringList data = lastMessage->split(QLatin1Char(' '));
    QStringList requests;
    QListWidgetItem *item;
    QHBoxLayout *layout;
    Request *requested;

    ui->friendRequestList->clear();
    if (data.length() > 1)
    {
        requests = data.at(1).split(QLatin1Char(';'));
        if (requests.length() >= 1)
        {
            for (int i = 0; i < requests.length(); i++)
            {
                item = new QListWidgetItem();
                requested = new Request(ui->friendRequestList, socket, username, &(requests.at(i)));
                layout = new QHBoxLayout();
                item->setSizeHint(QSize(requested->width(), requested->height()));
                layout->setSizeConstraint(QLayout::SetMinimumSize);
                requested->setLayout(layout);
                ui->friendRequestList->addItem(item);
                ui->friendRequestList->setItemWidget(item, requested);
            }
        }
    }

}

void MainWindow::getFriendSuccess()
{
    QStringList requests = lastMessage->split(QLatin1Char(' '));
    QStringList subData;
    QListWidgetItem *item;
    QHBoxLayout *layout;
    User *user;

    ui->friendConnectedList->clear();
    ui->friendDisconnectedList->clear();
    //qDebug() << requests.length() << "\n";
    if (requests.length() > 2)
    {
        subData = requests.at(1).split(QLatin1Char(';'));
        for (int i = 0; i < subData.length(); i+=2)
        {
            item = new QListWidgetItem();
            if (subData.at(i + 1).toInt() > 0)
            {
                user = new User(ui->friendConnectedList, &(subData.at(i)), subData.at(i + 1).toInt(), socket, username);
                layout = new QHBoxLayout();
                item->setSizeHint(QSize(user->width(), user->height()));
                layout->setSizeConstraint(QLayout::SetMinimumSize);
                user->setLayout(layout);
                ui->friendConnectedList->addItem(item);
                ui->friendConnectedList->setItemWidget(item, user);
            }
            else {
                user = new User(ui->friendDisconnectedList, &(subData.at(i)), subData.at(i + 1).toInt(), socket, username);
                layout = new QHBoxLayout();
                item->setSizeHint(QSize(user->width(), user->height()));
                layout->setSizeConstraint(QLayout::SetMinimumSize);
                user->setLayout(layout);
                ui->friendDisconnectedList->addItem(item);
                ui->friendDisconnectedList->setItemWidget(item, user);
            }

        }
    }
    if (syncAll)
    {
        syncAll = false;
        std::string *tosend = new std::string("GETFRIENDREQUEST ");
        tosend->append(username->toLocal8Bit().constData());
        tosend->append("\n");
        socket->write(tosend->c_str());
        delete(tosend);
    }
}

void MainWindow::createContactSuccess()
{
    syncAll = true;
    ui->friendRequestList->clear();
    std::string *tosend = new std::string("GETFRIEND ");
    tosend->append(username->toLocal8Bit().constData());
    tosend->append("\n");
    socket->write(tosend->c_str());
    delete(tosend);
}

void MainWindow::deleteRequestSuccess()
{
    std::string *tosend = new std::string("GETFRIENDREQUEST ");
    tosend->append(username->toLocal8Bit().constData());
    tosend->append("\n");
    socket->write(tosend->c_str());
    delete(tosend);
}

void MainWindow::on_disconnectButton_pressed()
{
    std::string *tosend = new std::string("LOGOUT ");
    tosend->append(username->toLocal8Bit().constData());
    tosend->append("\n");
    socket->write(tosend->c_str());
    delete(tosend);
}

void MainWindow::logoutSuccess()
{
    username->clear();
    ui->logInButton->setHidden(false);
    ui->passwordText->setHidden(false);
    ui->signInButton->setHidden(false);
    ui->usernameText->setHidden(false);
    ui->callList->setHidden(true);
    ui->errorLog->setHidden(false);
    ui->welcomeLabel->setHidden(false);
    ui->friendConnectedList->setHidden(true);
    ui->friendDisconnectedList->setHidden(true);
    ui->friendRequestList->setHidden(true);
    ui->disconnectButton->setHidden(true);
    syncAll = true;
    timer->stop();
}

void MainWindow::updateInfo()
{
    syncAll = true;
    std::string *tosend = new std::string("GETFRIEND ");

    tosend->append(username->toLocal8Bit().constData());
    tosend->append("\n");
    socket->write(tosend->c_str());
    delete (tosend);
}
