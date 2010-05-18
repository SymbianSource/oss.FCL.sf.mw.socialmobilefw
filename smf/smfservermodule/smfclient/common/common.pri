INCLUDEPATH += common \
    ./

PUBLIC_HEADERS += \
	common/smfalbum.h \
	common/smfartists.h \
	common/smfcomment.h \
	common/smfcontact.h \
	common/smfevent.h \
	common/smfgroup.h \
	common/smflocation.h \
	common/smflyrics.h \
	common/smfmusicfingerprint.h \
	common/smfmusicprofile.h \
	common/smfmusicrating.h \
	common/smfpicture.h \
	common/smfplaylist.h \
	common/smfpluginutil.h \
	common/smfpost.h \
	common/smfprovider.h \
	common/smfsubtitle.h \
	common/smftrackinfo.h \
	common/smfclientglobal.h \
	common/smfglobal.h \
	common/smfobserver.h

PRIVATE_HEADERS += \
	common/smfalbum_p.h \
	common/smfartists_p.h \
	common/smfcomment_p.h \
	common/smfcontact_p.h \
	common/smfevent_p.h \
	common/smfgroup_p.h \
	common/smflocation_p.h \
	common/smflyrics_p.h \
	common/smfmusicfingerprint_p.h \
	common/smfmusicprofile_p.h \
	common/smfmusicrating_p.h \
	common/smfpicture_p.h \
	common/smfplaylist_p.h \
	common/smfpost_p.h \
	common/smfsubtitle_p.h \
	common/smftrackinfo_p.h \
	common/smfobserver.h

SOURCES += \
	common/smfalbum.cpp \
	common/smfartists.cpp \
	common/smfcomment.cpp \
	common/smfcontact.cpp \
	common/smfevent.cpp \
	common/smfgroup.cpp \
	common/smflocation.cpp \
	common/smflyrics.cpp \
	common/smfmusicfingerprint.cpp \
	common/smfmusicprofile.cpp \
	common/smfmusicrating.cpp \
	common/smfpicture.cpp \
	common/smfplaylist.cpp \
	common/smfpluginutil.cpp \
	common/smfpost.cpp \
	common/smfprovider.cpp \
	common/smfsubtitle.cpp \
	common/smftrackinfo.cpp


symbian: {
	BLD_INF_RULES.prj_exports += common/smfclientglobal.h
	BLD_INF_RULES.prj_exports += common/smfglobal.h
	BLD_INF_RULES.prj_exports += common/smfobserver.h
	BLD_INF_RULES.prj_exports += common/smfalbum.h
	BLD_INF_RULES.prj_exports += common/smfalbum_p.h
	BLD_INF_RULES.prj_exports += common/smfartists.h
	BLD_INF_RULES.prj_exports += common/smfartists_p.h
	BLD_INF_RULES.prj_exports += common/smfcomment.h
	BLD_INF_RULES.prj_exports += common/smfcomment_p.h
	BLD_INF_RULES.prj_exports += common/smfcontact.h
	BLD_INF_RULES.prj_exports += common/smfcontact_p.h
	BLD_INF_RULES.prj_exports += common/smfevent.h
	BLD_INF_RULES.prj_exports += common/smfevent_p.h
	BLD_INF_RULES.prj_exports += common/smfgroup.h
	BLD_INF_RULES.prj_exports += common/smfgroup_p.h
	BLD_INF_RULES.prj_exports += common/smflocation.h
	BLD_INF_RULES.prj_exports += common/smflocation_p.h
	BLD_INF_RULES.prj_exports += common/smflyrics.h
	BLD_INF_RULES.prj_exports += common/smflyrics_p.h
	BLD_INF_RULES.prj_exports += common/smfmusicfingerprint.h
	BLD_INF_RULES.prj_exports += common/smfmusicfingerprint_p.h
	BLD_INF_RULES.prj_exports += common/smfmusicprofile.h
	BLD_INF_RULES.prj_exports += common/smfmusicprofile_p.h
	BLD_INF_RULES.prj_exports += common/smfmusicrating.h
	BLD_INF_RULES.prj_exports += common/smfmusicrating_p.h
	BLD_INF_RULES.prj_exports += common/smfpicture.h
	BLD_INF_RULES.prj_exports += common/smfpicture_p.h
	BLD_INF_RULES.prj_exports += common/smfplaylist.h
	BLD_INF_RULES.prj_exports += common/smfplaylist_p.h
	BLD_INF_RULES.prj_exports += common/smfpluginutil.h
	BLD_INF_RULES.prj_exports += common/smfpost.h
	BLD_INF_RULES.prj_exports += common/smfpost_p.h
	BLD_INF_RULES.prj_exports += common/smfprovider.h
	BLD_INF_RULES.prj_exports += common/smfsubtitle.h
	BLD_INF_RULES.prj_exports += common/smfsubtitle_p.h
	BLD_INF_RULES.prj_exports += common/smftrackinfo.h
	BLD_INF_RULES.prj_exports += common/smftrackinfo_p.h
}
