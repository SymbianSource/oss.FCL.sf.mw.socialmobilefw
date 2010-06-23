# ##############################################################################
# /*
# ========================================================================
# Name : smfcredmgrclient.pro
# Author : Pritam Roy Biswas
# Copyright : Copyright (c) 2009-2010, Sasken Communication Technologies Ltd.
# Description :
# ========================================================================
# */
# ###############################################################################
QT += network
TARGET = smfcredmgrclient
TEMPLATE = lib
CONFIG += precompile_header
DEFINES += SMFCREDMGRCLIENT_LIBRARY \
	Q_OS_SYMBIAN
INCLUDEPATH += . \
    
SOURCES += smfcredmgrclient.cpp \
    smfcredmgrclientutil.cpp
HEADERS += smfcredmgrclient.h \
    smfcredmgrclientutil.h
symbian { 
    TARGET.UID3 = 0xEd21cfd3
    SOURCES += smfcredmgrclient_p.cpp \
        smfcredmgrclientsession.cpp
        
    HEADERS += smfcredmgrclient_p.h \
        smfcredmgrclientsession.h 
     
    BLD_INF_RULES.prj_exports += smfcredmgrclient.h
}
symbian::LIBS += -leuser \
    -lestor \
    -lefsrv \
     -lsmfcredmgrcommon
    