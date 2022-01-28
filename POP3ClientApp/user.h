#ifndef USER_H
#define USER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QTextStream>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = nullptr);
    bool CheckAddress(QString address);

public slots:
    void Start();
    void ReadInput();
    bool ProcessInput(QString Line);
    int PrintMessage(QString Line);
    void ConnectServer(QString address);

signals:
    void onReadLine(QString Line);
    void sendCommand(QString command);
    void connectToServer(QString address);
    void loginUser(QString loginData);
    void disconnectUser();

private:

    QThread userThread;
    QStringList commands = {"STAT", "LIST", "RETR", "DELE", "RSET"};

};
#endif // USER_H
