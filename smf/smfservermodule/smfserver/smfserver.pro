TEMPLATE = app
TARGET = smfserver 

DEFINES += NO_OTHER_MODULES
QT += core \
    gui \
    network \
    sql

 
CONFIG += mobility

MOBILITY = contacts \
    location \
    bearer \
    systeminfo
    
#include(datastoremgr/datastoremgr.pri)
include(pluginmgr/pluginmgr.pri)
include(server/server.pri)
include(smfplugins/smfplugins.pri)
include(transportmgr/transportmgr.pri)

!symbian {
    include(../../desktop.pri)
    include(../smfclient/common/common.pri)

    LIBS += -lqjson
}

PUBLIC_HEADERS += \
	smfserverglobal.h

HEADERS += \
    $$PUBLIC_HEADERS \
    $$PRIVATE_HEADERS
    
SOURCES += \
	main.cpp

symbian: { 
    TARGET.UID3 = 0xE5027327

    TARGET.CAPABILITY = ReadUserData \
        WriteUserData \
        LocalServices \
        NetworkServices \
        UserEnvironment

    LIBS += -lsmfclient.dll
    
    SOURCES += smfserver_reg.rss
}
