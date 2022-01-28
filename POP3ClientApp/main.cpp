#include <QCoreApplication>
#include "client.h"
#include "user.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client client;
    User user;

    QObject::connect(&user, SIGNAL(sendCommand(QString)), &client, SLOT(SendCommandToServer(QString)));
    QObject::connect(&user, SIGNAL(connectToServer(QString)), &client, SLOT(ConnectToServer(QString)));
    QObject::connect(&user, SIGNAL(loginUser(QString)), &client, SLOT(Login(QString)));
    QObject::connect(&user, SIGNAL(disconnectUser()), &client, SLOT(Disconnect()));

    user.Start();

    return a.exec();
}
