TEMPLATE = lib
TARGET = smfclient
QT += core \
    gui \
    network
CONFIG += mobility
MOBILITY += contacts \
    location
DEFINES += WRITE_LOG \
    SMFCLIENT_LIB_EXPORT 

include(client/client.pri)
include(common/common.pri)

!symbian {
    include(../../desktop.pri)
    LIBS += -lqjson
}

HEADERS += $$PUBLIC_HEADERS \
    $$PRIVATE_HEADERS
symbian: { 
    TARGET.UID3 = 0xE08059D4
    TARGET.CAPABILITY = NetworkServices \
    	ReadUserData \
        WriteUserData \
        LocalServices \
        UserEnvironment \
        ReadDeviceData \
        WriteDeviceData
    TARGET.EPOCALLOWDLLDATA = 1
    LIBS += -lqjson.dll
}
