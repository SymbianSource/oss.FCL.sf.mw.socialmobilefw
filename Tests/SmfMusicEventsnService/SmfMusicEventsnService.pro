TEMPLATE = app
TARGET = SmfMusicEventsnService 
CONFIG += mobility
MOBILITY += contacts \
    location
QT += core \
    gui \
    xml \
    network 
HEADERS   += SmfMusicEventsnService.h
SOURCES   += SmfMusicEventsnService_reg.rss \
    main.cpp \
    SmfMusicEventsnService.cpp
FORMS	  += SmfMusicEventsnService.ui
RESOURCES +=
symbian:
	{
	TARGET.UID3 = 0xE565EECD
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
