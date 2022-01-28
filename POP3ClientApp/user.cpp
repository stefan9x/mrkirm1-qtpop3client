#include "user.h"
#include <QtTest/QTest>

User::User(QObject *parent)
    : QObject{parent}
{
    this->moveToThread(&userThread);

    connect(&userThread, SIGNAL(started()), this, SLOT(ReadInput()));
    connect(this, SIGNAL(onReadLine(QString)), this, SLOT(ProcessInput(QString)));

}

void User::Start()
{
    QTextStream info(stdout);

    info << "Welcome to simple text based POP3 Client\n\n";
    info << "USAGE:\n";
    info << "To connect to POP3 server, type  connect ip:port\n";
    info << "To login, type  login username:password\n\n";
    info << "After successful login, you can use commands(must be UPPER case):\n";
    info << "\tSTAT - to show number of messages and size.\n";
    info << "\tLIST - to get messages list.\n";
    info << "\tRETR # - to retrive message with number from list.\n";
    info << "\tDELE # - to delete message with number from list.\n";
    info << "\tRSET - to undelete all deleted messages.\n";
    info << "\tQUIT - to log out and disconnect from server.\n";
    info << "Type your commands now:\n";

    userThread.start();
}

void User::ReadInput()
{
    QTextStream input(stdin);

    QString userInput = input.readLine();
    emit onReadLine(userInput);
    ReadInput();
}

bool User::ProcessInput(QString Line)
{

    if(Line.startsWith("connect"))
    {
        QString address = Line.section(" ", 1, 1);

        if (CheckAddress(address))
        {
            ConnectServer(address);
            return true;
        }
        else
        {
            return false;
        }

    }

    else if (Line.startsWith("login"))
    {
        QString loginData = Line.section(" ", 1, 1);

        emit loginUser(loginData);
        return true;

    }

    else if (Line.startsWith("RETR") || Line.startsWith("DELE"))
    {
        emit sendCommand(Line);
        return true;
    }

    else if (commands.contains(Line))
    {
        emit sendCommand(Line);
        return true;
    }

    else if (Line.compare("QUIT") == 0)
    {
        emit disconnectUser();
        return true;
    }

    else
    {
        PrintMessage("Command not valid");
        return false;
    }

}

int User::PrintMessage(QString Line)
{
    QTextStream stream(stdout);
    stream << Line << "\n";
    return QTextStream::Status();
}

void User::ConnectServer(QString address)
{
    emit connectToServer(address);
}

bool User::CheckAddress(QString address)
{

    QString ip = address.section(":", 0, 0);
    QString port = address.section(":", 1, 1);
    bool ipOk = true;
    bool portOk = true;
    bool isNumber = true;
    ushort temp;

    QStringList ipParts = ip.split(".");

    if(ipParts.length() == 4)
    {
        for (int i = 0; i < 4; i++)
        {
            temp = ipParts[i].toUShort(&isNumber);
            if (!isNumber || temp > 255)
            {
                ipOk = false;
                break;
            }
        }
    }
    else
    {
        ipOk = false;
    }

    temp = port.toUShort(&isNumber);

    if (!isNumber) portOk = false;

    if (ipOk && portOk)
    {
        return true;
    }
    else
    {
        if (!ipOk) PrintMessage("Bad ip");
        if (!portOk) PrintMessage("Bad port");
        return false;
    }
}
