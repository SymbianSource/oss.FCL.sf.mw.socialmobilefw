TEMPLATE = app
TARGET = TestApp
QT += gui \
    core \
    network
CONFIG += mobility
MOBILITY += contacts \
    location
SOURCES += TestAppmain.cpp \
    MainWindow.cpp
HEADERS += Mainwindow.h 

symbian: { 
    LIBS += -lsmfclient.dll \
        -lcone \
        -leikcore \
        -lavkon \
	-lsmfcommon
   
    TARGET.CAPABILITY = ReadUserData \
        WriteUserData\
    LocalServices\
NetworkServices\
UserEnvironment\
ReadDeviceData\
WriteDeviceData
}
