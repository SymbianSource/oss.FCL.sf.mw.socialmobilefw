TEMPLATE = lib
TARGET = smfcommon
QT += core \
    gui \
    network
CONFIG += mobility
MOBILITY += contacts \
    location
DEFINES += SMFCOMMON_LIB_EXPORT
PUBLIC_HEADERS += \
	smfactions.h \
	smfalbum.h \
	smfartists.h \
	smfcomment.h \
	smfcontact.h \
	smfevent.h \
	smfgroup.h \
	smflocation.h \
	smflyrics.h \
	smfmusicfingerprint.h \
	smfmusicprofile.h \
	smfmusicrating.h \
	smfpicture.h \
	smfpicturealbum.h \
	smfplaylist.h \
	smfpluginutil.h \
	smfpost.h \
	smfprovider.h \
	smfsubtitle.h \
	smftrackinfo.h \
	smfurl.h \
	smfclientglobal.h \
	smfglobal.h \
	smfobserver.h
PRIVATE_HEADERS += \
	smfactions_p.h \
	smfalbum_p.h \
	smfartists_p.h \
	smfcomment_p.h \
	smfcontact_p.h \
	smfevent_p.h \
	smfgroup_p.h \
	smflocation_p.h \
	smflyrics_p.h \
	smfmusicfingerprint_p.h \
	smfmusicprofile_p.h \
	smfmusicrating_p.h \
	smfpicture_p.h \
	smfpicturealbum_p.h \
	smfplaylist_p.h \
	smfpost_p.h \
	smfsubtitle_p.h \
	smftrackinfo_p.h \
	smfurl_p.h \
	smfobserver.h
SOURCES += \
	smfactions.cpp \
	smfalbum.cpp \
	smfartists.cpp \
	smfcomment.cpp \
	smfcontact.cpp \
	smfevent.cpp \
	smfgroup.cpp \
	smflocation.cpp \
	smflyrics.cpp \
	smfmusicfingerprint.cpp \
	smfmusicprofile.cpp \
	smfmusicrating.cpp \
	smfpicture.cpp \
	smfpicturealbum.cpp \
	smfplaylist.cpp \
	smfpluginutil.cpp \
	smfpost.cpp \
	smfprovider.cpp \
	smfsubtitle.cpp \
	smftrackinfo.cpp \
	smfurl.cpp    
HEADERS += $$PUBLIC_HEADERS \
    $$PRIVATE_HEADERS
symbian: { 
    TARGET.UID3 = 0xEE9F4502
    TARGET.CAPABILITY = NetworkServices \
    	ReadUserData \
        WriteUserData \
        LocalServices \
        UserEnvironment \
        ReadDeviceData \
        WriteDeviceData
    TARGET.EPOCALLOWDLLDATA = 1
    LIBS += -lqjson.dll -lsmfcredmgrclient
    
   	BLD_INF_RULES.prj_exports += smfclientglobal.h
	BLD_INF_RULES.prj_exports += smfglobal.h
	BLD_INF_RULES.prj_exports += smfobserver.h
	BLD_INF_RULES.prj_exports += smfactions.h
	BLD_INF_RULES.prj_exports += smfactions_p.h
	BLD_INF_RULES.prj_exports += smfalbum.h
	BLD_INF_RULES.prj_exports += smfalbum_p.h
	BLD_INF_RULES.prj_exports += smfartists.h
	BLD_INF_RULES.prj_exports += smfartists_p.h
	BLD_INF_RULES.prj_exports += smfcomment.h
	BLD_INF_RULES.prj_exports += smfcomment_p.h
	BLD_INF_RULES.prj_exports += smfcontact.h
	BLD_INF_RULES.prj_exports += smfcontact_p.h
	BLD_INF_RULES.prj_exports += smfevent.h
	BLD_INF_RULES.prj_exports += smfevent_p.h
	BLD_INF_RULES.prj_exports += smfgroup.h
	BLD_INF_RULES.prj_exports += smfgroup_p.h
	BLD_INF_RULES.prj_exports += smflocation.h
	BLD_INF_RULES.prj_exports += smflocation_p.h
	BLD_INF_RULES.prj_exports += smflyrics.h
	BLD_INF_RULES.prj_exports += smflyrics_p.h
	BLD_INF_RULES.prj_exports += smfmusicfingerprint.h
	BLD_INF_RULES.prj_exports += smfmusicfingerprint_p.h
	BLD_INF_RULES.prj_exports += smfmusicprofile.h
	BLD_INF_RULES.prj_exports += smfmusicprofile_p.h
	BLD_INF_RULES.prj_exports += smfmusicrating.h
	BLD_INF_RULES.prj_exports += smfmusicrating_p.h
	BLD_INF_RULES.prj_exports += smfpicture.h
	BLD_INF_RULES.prj_exports += smfpicture_p.h
	BLD_INF_RULES.prj_exports += smfpicturealbum.h
	BLD_INF_RULES.prj_exports += smfpicturealbum_p.h
	BLD_INF_RULES.prj_exports += smfplaylist.h
	BLD_INF_RULES.prj_exports += smfplaylist_p.h
	BLD_INF_RULES.prj_exports += smfpluginutil.h
	BLD_INF_RULES.prj_exports += smfpost.h
	BLD_INF_RULES.prj_exports += smfpost_p.h
	BLD_INF_RULES.prj_exports += smfprovider.h
	BLD_INF_RULES.prj_exports += smfsubtitle.h
	BLD_INF_RULES.prj_exports += smfsubtitle_p.h
	BLD_INF_RULES.prj_exports += smftrackinfo.h
	BLD_INF_RULES.prj_exports += smftrackinfo_p.h
	BLD_INF_RULES.prj_exports += smfurl.h
	BLD_INF_RULES.prj_exports += smfurl_p.h
}