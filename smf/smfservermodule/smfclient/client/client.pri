INCLUDEPATH += client \
    ./

PUBLIC_HEADERS += client/smfclient.h \
    client/smfcontactfetcher.h \
    client/smfpostprovider.h \
    client/smfgallery.h \
    client/smfrelationmgr.h \
    client/smfmusic.h \
    client/smfactivityfetcher.h
    
PRIVATE_HEADERS += client/smfclient_p.h \
    client/smfcontactfetcher_p.h \
    client/smfpostprovider_p.h \
    client/smfgallery_p.h \
    client/smfrelationmgr_p.h \
    client/smfmusic_p.h \
    client/smfactivityfetcher_p.h
    
SOURCES += client/smfrelationmgr_p.cpp \
    client/smfrelationmgr.cpp \
    client/smfgallery.cpp \
    client/smfgallery_p.cpp \
    client/smfpostprovider.cpp \
    client/smfpostprovider_p.cpp \
    client/smfmusic.cpp \
    client/smfclient.cpp \
    client/smfclient_p.cpp \
    client/smfcontactfetcher.cpp \
    client/smfcontactfetcher_p.cpp \
    client/smfmusic_p.cpp \
    client/smfactivityfetcher.cpp \
    client/smfactivityfetcher_p.cpp 
    
symbian: { 
    BLD_INF_RULES.prj_exports += client/smfclient.h
    BLD_INF_RULES.prj_exports += client/smfclient_p.h
    BLD_INF_RULES.prj_exports += client/smfcontactfetcher.h
    BLD_INF_RULES.prj_exports += client/smfcontactfetcher_p.h
    BLD_INF_RULES.prj_exports += client/smfclientsymbian.h
    BLD_INF_RULES.prj_exports += client/smfclientsymbiansession.h
    BLD_INF_RULES.prj_exports += client/smfpostprovider.h
    BLD_INF_RULES.prj_exports += client/smfpostprovider_p.h
    BLD_INF_RULES.prj_exports += client/smfgallery.h
    BLD_INF_RULES.prj_exports += client/smfgallery_p.h
    BLD_INF_RULES.prj_exports += client/smfrelationmgr.h
    BLD_INF_RULES.prj_exports += client/smfrelationmgr_p.h
    BLD_INF_RULES.prj_exports += client/smfmusic.h
    BLD_INF_RULES.prj_exports += client/smfmusic_p.h
    BLD_INF_RULES.prj_exports += client/smfactivityfetcher.h
    BLD_INF_RULES.prj_exports += client/smfactivityfetcher_p.h
	
    PUBLIC_HEADERS += client/smfclientsymbian.h \
        client/smfclientsymbiansession.h

    SOURCES += client/smfclientsymbian.cpp
}
else { 
    PRIVATE_HEADERS += client/smfclientqt.h
    SOURCES += client/smfclientqt.cpp
}