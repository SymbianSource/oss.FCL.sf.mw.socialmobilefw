TEMPLATE = app
TARGET = smfclientapp
DEFINES += WRITE_LOG #\
    #OLDER_QT_MOBILITY
QT += core \
    gui \
    network \
    webkit
CONFIG += mobility
MOBILITY = contacts \
    location
HEADERS += displaywidget.h \
    testscreen.h \
    smfclientapp.h
SOURCES += displaywidget.cpp \
    testscreen.cpp \
    smfclientapp_reg.rss \
    main.cpp \
    smfclientapp.cpp
FORMS += displaywidget.ui \
    smfclientapp.ui
RESOURCES += 
symbian:TARGET.UID3 = 0xE08059D4
symbian:LIBS += -lsmfclient.dll
symbian:TARGET.CAPABILITY = ReadUserData \
    WriteUserData \
    LocalServices \
    NetworkServices \
    UserEnvironment
