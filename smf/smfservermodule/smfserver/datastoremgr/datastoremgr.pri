INCLUDEPATH += datastoremgr \
    ./

CONFIG += console \
    qt \
    debug \
    warn_on

PUBLIC_HEADERS += \
	datastoremgr/smfSns.h \
    datastoremgr/smfUserProfile.h \
    datastoremgr/smfSocialProfile.h \
    datastoremgr/dsm.h

PRIVATE_HEADERS += 

SOURCES += \
	datastoremgr/smfSns.cpp \
#    datastoremgr/test.cpp \
    datastoremgr/smfUserProfile.cpp \
    datastoremgr/smfSocialProfile.cpp \
    datastoremgr/dsm.cpp
