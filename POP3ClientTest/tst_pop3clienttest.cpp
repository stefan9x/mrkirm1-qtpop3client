#include <QtTest>

// add necessary includes here
#include "client.h"
#include "user.h"

class POP3ClientTest : public QObject
{
    Q_OBJECT

private:
    Client *client;
    User *user;

public:
    POP3ClientTest();
    ~POP3ClientTest();

private slots:
    void init();
    void cleanup();
    void TestClientPrintMessage();
    void TestClientConnectToActiveServer();
    void TestClientConnectToInctiveServer();
    void TestClientLoginActiveServer();
    void TestClientLoginBadActiveServer();
    void TestClientLoginInactiveServer();
    void TestClientSendCommandToActiveServer();
    void TestClientSendCommandToInactiveServer();
    void TestClientDisconnectTrue();
    void TestClientDisconnectFalse();
    void TestUserPrintMessage();
    void TestUserProcessInputConnectTrue();
    void TestUserProcessInputConnectFalse();
    void TestUserProcessInputLogin();
    void TestUserProcessInputRetr();
    void TestUserProcessInputOtherCommand();
    void TestUserProcessInputQuit();
    void TestUserProcessInputFalse();
    void TestUserCheckAddressTrue();
    void TestUserCheckAddressBadIp();
    void TestUserCheckAddressBadPort();
    void TestUserCheckAddressBadIpPort();


};

POP3ClientTest::POP3ClientTest()
{


}

POP3ClientTest::~POP3ClientTest()
{

}

void POP3ClientTest::init()
{
    client = new Client();
    user = new User();
}

void POP3ClientTest::cleanup()
{
    delete client;
    delete user;
}

void POP3ClientTest::TestClientPrintMessage()
{

    int ret = client->PrintMessage("Message");
    QVERIFY2(ret == 0, "Message not successfully displayed");

}

void POP3ClientTest::TestClientConnectToActiveServer()
{
    bool ret = client->ConnectToServer("127.0.0.1:110");

    QVERIFY2(ret == true, qPrintable(client->socket->errorString()));
}

void POP3ClientTest::TestClientConnectToInctiveServer()
{
    bool ret = client->ConnectToServer("127.0.0.1:111");

    QVERIFY(ret == false);
}

void POP3ClientTest::TestClientLoginActiveServer()
{
    client->ConnectToServer("127.0.0.1:110");
    QTest::qWait(500);
    bool ret = client->Login("Admin:");
    QTest::qWait(500);
    client->Disconnect();
    QVERIFY2(ret == true, "Server not connected, wrong password, or user not found");
}

void POP3ClientTest::TestClientLoginBadActiveServer()
{
    bool ret1 = client->ConnectToServer("127.0.0.1:110");
    QTest::qWait(500);
    bool ret2 = client->Login("Admin:asd");
    QTest::qWait(500);
    client->Disconnect();
    QVERIFY2(ret1 == true && ret2 == false, "Server not connected or password check passed");
}

void POP3ClientTest::TestClientLoginInactiveServer()
{
    bool ret = client->Login("Admin:");
    QVERIFY(ret == false);
}

void POP3ClientTest::TestClientSendCommandToActiveServer()
{
    client->ConnectToServer("127.0.0.1:110");
    QTest::qWait(500);
    client->Login("Admin:");
    QTest::qWait(500);
    bool ret = client->SendCommandToServer("STAT");
    QTest::qWait(500);
    client->Disconnect();

    QVERIFY2(ret == true, "Server not connected or user not logged in");
}

void POP3ClientTest::TestClientSendCommandToInactiveServer()
{
    bool ret = client->SendCommandToServer("STAT");

    QVERIFY(ret == false);

}

void POP3ClientTest::TestClientDisconnectTrue()
{
    client->ConnectToServer("127.0.0.1:110");
    QTest::qWait(500);
    client->Login("Admin:");
    QTest::qWait(500);
    bool ret = client->Disconnect();
    QVERIFY2(ret == true, "User not logged in");
}

void POP3ClientTest::TestClientDisconnectFalse()
{
    bool ret = client->Disconnect();
    QVERIFY(ret == false);
}

void POP3ClientTest::TestUserPrintMessage()
{
    int ret = user->PrintMessage("Message");
    QVERIFY2(ret == 0, "Message not successfully displayed");
}

void POP3ClientTest::TestUserProcessInputConnectTrue()
{
    bool ret = user->ProcessInput("connect 127.0.0.1:110");
    QVERIFY2(ret == true, "Bad address");
}

void POP3ClientTest::TestUserProcessInputConnectFalse()
{
    bool ret = user->ProcessInput("connect 127.0.a.1:a10");
    QVERIFY2(ret == false, "Adress is good, so test failed");
}

void POP3ClientTest::TestUserProcessInputLogin()
{
    bool ret = user->ProcessInput("login admin:a");
    QVERIFY(ret == true);
}

void POP3ClientTest::TestUserProcessInputRetr()
{
    bool ret = user->ProcessInput("RETR 1");
    QVERIFY(ret == true);
}

void POP3ClientTest::TestUserProcessInputOtherCommand()
{
    bool ret = user->ProcessInput("STAT");
    QVERIFY(ret == true);
}

void POP3ClientTest::TestUserProcessInputQuit()
{
    bool ret = user->ProcessInput("QUIT");
    QVERIFY(ret == true);
}

void POP3ClientTest::TestUserProcessInputFalse()
{
    bool ret = user->ProcessInput("something");
    QVERIFY(ret == false);
}

void POP3ClientTest::TestUserCheckAddressTrue()
{
    bool ret = user->CheckAddress("127.0.0.1:110");
    QVERIFY2(ret == true, "IP is good, return should be true");
}

void POP3ClientTest::TestUserCheckAddressBadIp()
{
    bool ret = user->CheckAddress("127.a.0.1:110");
    QVERIFY2(ret == false, "IP is bad, return should be false");
}

void POP3ClientTest::TestUserCheckAddressBadPort()
{
    bool ret = user->CheckAddress("127.a.0.1:1a0");
    QVERIFY2(ret == false, "Port are bad, return should be false");
}

void POP3ClientTest::TestUserCheckAddressBadIpPort()
{
    bool ret = user->CheckAddress("127.a0.1:1asd0");
    QVERIFY2(ret == false, "Port and IP are bad, return should be false");
}

QTEST_MAIN(POP3ClientTest)

#include "tst_pop3clienttest.moc"
