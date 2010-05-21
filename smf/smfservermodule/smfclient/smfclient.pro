TEMPLATE = lib
TARGET = smfclient
QT += core \
    gui \
    network
CONFIG += mobility
MOBILITY = contacts \
    location
DEFINES += WRITE_LOG \
    SMFCLIENT_LIB_EXPORT #\
#    OLDER_QT_MOBILITY

include(client/client.pri)
include(common/common.pri)

!symbian {
    include(../../desktop.pri)
    LIBS += -lqjson
}

# Private Headers
HEADERS += $$PUBLIC_HEADERS \
    $$PRIVATE_HEADERS
SOURCES += main.cpp

symbian: { 
    TARGET.UID3 = 0xE08059D4
    TARGET.CAPABILITY = ReadUserData \
        WriteUserData \
        LocalServices \
        NetworkServices \
        UserEnvironment
    TARGET.EPOCALLOWDLLDATA = 1
    SOURCES += smfclient_reg.rss
    LIBS += -lqjson.dll
} 

