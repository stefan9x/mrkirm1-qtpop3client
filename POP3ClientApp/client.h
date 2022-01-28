#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QAbstractSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    QTcpSocket *socket;

public slots:
    int PrintMessage(QString message);
    bool SendCommandToServer(QString command);
    bool ConnectToServer(QString address);
    bool Login(QString loginData);
    bool Disconnect();

signals:

private:
    bool serverConnected = false;
    bool userLoggedIn = false;

};

#endif // CLIENT_H
