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
PUBLIC_HEADERS += 

# Private Headers
PRIVATE_HEADERS += 
HEADERS += $$PUBLIC_HEADERS \
    $$PRIVATE_HEADERS
SOURCES += main.cpp \
    smfclient_reg.rss
FORMS += 
RESOURCES += 
symbian: { 
    TARGET.UID3 = 0xE08059D4
    TARGET.CAPABILITY = ReadUserData \
        WriteUserData \
        LocalServices \
        NetworkServices \
        UserEnvironment
    TARGET.EPOCALLOWDLLDATA = 1
    LIBS += -lqjson.dll
}
