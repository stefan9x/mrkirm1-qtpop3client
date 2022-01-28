#include "client.h"

Client::Client(QObject *parent)
    : QObject{parent}
{
    socket = new QTcpSocket(this);
}

int Client::PrintMessage(QString message)
{
    QTextStream status(stdout);

    status << message << "\n";
    return QTextStream::Status();
}

bool Client::SendCommandToServer(QString command)
{
    qint64 status;

    if (userLoggedIn)
    {
        status = socket->write(command.toLocal8Bit() + "\r\n");
        if (status == -1) return false;

        socket->waitForReadyRead(500);
        PrintMessage(socket->readAll());
        return true;
    }

    else
    {
        PrintMessage("Please login to send commands.");
        return false;
    }

}

bool Client::ConnectToServer(QString address)
{

    QString ip = address.section(":", 0, 0);
    QString port = address.section(":", 1, 1);

    socket->connectToHost(ip, port.toInt());

    if(!socket->waitForConnected(1000))
    {
        PrintMessage("Error: " + socket->errorString());
        serverConnected = false;
        return false;
    }
    else
    {
        PrintMessage("Connected.");
        serverConnected = true;
        return true;
    }
}

bool Client::Login(QString loginData)
{

    QString username = loginData.section(":", 0, 0);
    QString password = loginData.section(":", 1, 1);
    QByteArray serverMsg;

    if (serverConnected)
    {
        socket->write("USER " + username.toLocal8Bit() + "\r\n");
        socket->waitForReadyRead(500);

        serverMsg = socket->readAll();

        if (serverMsg.startsWith("+"))
        {
            socket->write("PASS " + password.toLocal8Bit() + "\r\n");
            socket->waitForReadyRead(500);
            serverMsg = socket->readAll();

            if (serverMsg.startsWith("+"))
            {
                userLoggedIn = true;
                PrintMessage("Logged in!");
                return true;
            }
            else
            {
                PrintMessage("Wrong password.");
                return false;
            }
        }
        else
        {
            userLoggedIn = false;
            PrintMessage("User not found.");
            return false;
        }
    }
    else
    {
        PrintMessage("Please connect to server first.");
        return false;
    }

}

bool Client::Disconnect()
{
    if(userLoggedIn)
    {
        SendCommandToServer("QUIT");
        serverConnected = false;
        userLoggedIn = false;
        socket->disconnectFromHost();
        socket->close();
        PrintMessage("User logged out.");
        return true;
    }
    else
    {
        PrintMessage("Cannot logout, user not logged in.");
        return false;
    }

}
