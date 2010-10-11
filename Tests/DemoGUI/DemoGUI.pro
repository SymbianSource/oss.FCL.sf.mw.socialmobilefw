# -------------------------------------------------
# Project created by QtCreator 2010-06-29T13:53:43
# -------------------------------------------------
TARGET = DemoGUI
TEMPLATE = app
QT += network
CONFIG += mobility
MOBILITY += contacts \
    location
SOURCES += CustomListWidget.cpp \
    DemoGUI.rss \
    DemoGUI_reg.rss \
    FriendView.cpp \
    GridView.cpp \
    HomeView.cpp \
    ImageDownload.cpp \
    SMF.rss \
    SMF_reg.rss \
    ToolBar.cpp \
    main.cpp \
    mainwindow.cpp 
    
HEADERS += CustomListWidget.h \
    DemoGUI.loc \
    FriendView.h \
    GridView.h \
    HomeView.h \
    ImageDownload.h \
    SMF.loc \
    ScreenSize.h \
    ToolBar.h \
    WidgetConstants.h \
    mainwindow.h 
   
   
FORMS += mainwindow.ui \
    mainwindow.ui
symbian: { 
    TARGET.CAPABILITY = NetworkServices \
        ReadUserData \
        WriteUserData \
        LocalServices \
        UserEnvironment \
        ReadDeviceData \
        WriteDeviceData
    LIBS += -lsmfclient.dll \
        -lcone \
        -leikcore \
        -lsmfcommon \
        -lavkon
    ICON = sasken_logo.svg
}
