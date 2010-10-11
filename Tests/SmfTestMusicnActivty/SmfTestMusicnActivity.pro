TEMPLATE = app
CONFIG += mobility
MOBILITY += contacts \
    location
TARGET = SmfTestMusicnActivity
QT += core \
    gui \
    xml \
    network
HEADERS += SmfTestMusicnActivity.loc \
    SmfTestMusicnActivity.h
SOURCES += main.cpp \
    SmfTestMusicnActivity.cpp
FORMS += SmfTestMusicnActivity.ui
RESOURCES += 
symbian: { 
    TARGET.UID3 = 0xEB5419B9
    TARGET.CAPABILITY = NetworkServices \
        ReadUserData \
        WriteUserData \
        LocalServices \
        UserEnvironment \
        ReadDeviceData \
        WriteDeviceData
    LIBS += -lsmfclient \
        -lsmfcommon
}
