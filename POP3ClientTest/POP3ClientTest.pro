QT += testlib
QT -= gui
QT += network

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_pop3clienttest.cpp \
    $$PWD/../POP3ClientApp/client.cpp \
    $$PWD/../POP3ClientApp/user.cpp
HEADERS += \
    $$PWD/../POP3ClientApp/client.h \
    $$PWD/../POP3ClientApp/user.h

INCLUDEPATH += $$PWD/../POP3ClientApp
