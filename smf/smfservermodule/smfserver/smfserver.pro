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
    # On desktop, we need to include these locally, as there is
    # no /epoc32/include available.
    INCLUDEPATH += ../util/qjson/src
    LIBS += -L../util/qjson/lib -lqjson
    include(../smfclient/common/common.pri)
}

PUBLIC_HEADERS += \
	smfserverglobal.h

HEADERS += \
    $$PUBLIC_HEADERS \
    $$PRIVATE_HEADERS
    
SOURCES += \
	main.cpp

FORMS	  +=
RESOURCES +=

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
