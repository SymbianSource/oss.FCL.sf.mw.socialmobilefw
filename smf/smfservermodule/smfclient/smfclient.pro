TEMPLATE = lib
TARGET = smfclient
QT += core \
    gui \
    network
CONFIG += mobility
MOBILITY += contacts \
    location
DEFINES += SMFCLIENT_LIB_EXPORT
PUBLIC_HEADERS += smfclient.h \
    smfcontactfetcher.h \
    smfpostprovider.h \
    smfgallery.h \
    smfrelationmgr.h \
    smfmusic.h \
    smfactivityfetcher.h
PRIVATE_HEADERS += smfclient_p.h \
    smfcontactfetcher_p.h \
    smfpostprovider_p.h \
    smfgallery_p.h \
    smfrelationmgr_p.h \
    smfmusic_p.h \
    smfactivityfetcher_p.h
SOURCES +=  \
    smfrelationmgr_p.cpp \
    smfrelationmgr.cpp \
    smfgallery.cpp \
    smfgallery_p.cpp \
    smfpostprovider.cpp \
    smfpostprovider_p.cpp \
    smfmusic.cpp \
    smfclient.cpp \
    smfclient_p.cpp \
    smfcontactfetcher.cpp \
    smfcontactfetcher_p.cpp \
    smfmusic_p.cpp \
    smfactivityfetcher.cpp \
    smfactivityfetcher_p.cpp
HEADERS += $$PUBLIC_HEADERS \
    $$PRIVATE_HEADERS
!symbian { 
    include(../../desktop.pri)
    LIBS += -lqjson
}
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
    LIBS += -lsmfcommon
    BLD_INF_RULES.prj_exports += smfclient.h
    BLD_INF_RULES.prj_exports += smfclient_p.h
    BLD_INF_RULES.prj_exports += smfcontactfetcher.h
    BLD_INF_RULES.prj_exports += smfcontactfetcher_p.h
    BLD_INF_RULES.prj_exports += smfclientsymbian.h
    BLD_INF_RULES.prj_exports += smfclientsymbiansession.h
    BLD_INF_RULES.prj_exports += smfpostprovider.h
    BLD_INF_RULES.prj_exports += smfpostprovider_p.h
    BLD_INF_RULES.prj_exports += smfgallery.h
    BLD_INF_RULES.prj_exports += smfgallery_p.h
    BLD_INF_RULES.prj_exports += smfrelationmgr.h
    BLD_INF_RULES.prj_exports += smfrelationmgr_p.h
    BLD_INF_RULES.prj_exports += smfmusic.h
    BLD_INF_RULES.prj_exports += smfmusic_p.h
    BLD_INF_RULES.prj_exports += smfactivityfetcher.h
    BLD_INF_RULES.prj_exports += smfactivityfetcher_p.h
}
symbian: { 
    PUBLIC_HEADERS += smfclientsymbian.h \
        smfclientsymbiansession.h
    SOURCES += smfclientsymbian.cpp
}
else { 
    PRIVATE_HEADERS += smfclientqt.h
    SOURCES += smfclientqt.cpp
}
