# TestSmfGetServices

TEMPLATE = app

TARGET = testsmfgetservices

QT += core \
      testlib

SOURCES += ./testsmfgetservices.cpp

LIBS += -lsmfclient

!symbian {
    include(../../../desktop.pri)
    INCLUDEPATH += $$PWD/../../smfclient/client \
                   $$PWD/../../smfclient/common \
}
