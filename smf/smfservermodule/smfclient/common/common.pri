INCLUDEPATH += $$PWD

PUBLIC_HEADERS += \
	$$PWD/smfalbum.h \
	$$PWD/smfartists.h \
	$$PWD/smfcomment.h \
	$$PWD/smfcontact.h \
	$$PWD/smfevent.h \
	$$PWD/smfgroup.h \
	$$PWD/smflocation.h \
	$$PWD/smflyrics.h \
	$$PWD/smfmusicfingerprint.h \
	$$PWD/smfmusicprofile.h \
	$$PWD/smfmusicrating.h \
	$$PWD/smfpicture.h \
	$$PWD/smfplaylist.h \
	$$PWD/smfpluginutil.h \
	$$PWD/smfpost.h \
	$$PWD/smfprovider.h \
	$$PWD/smfsubtitle.h \
	$$PWD/smftrackinfo.h \
	$$PWD/smfclientglobal.h \
	$$PWD/smfglobal.h \
	$$PWD/smfobserver.h

PRIVATE_HEADERS += \
	$$PWD/smfalbum_p.h \
	$$PWD/smfartists_p.h \
	$$PWD/smfcomment_p.h \
	$$PWD/smfcontact_p.h \
	$$PWD/smfevent_p.h \
	$$PWD/smfgroup_p.h \
	$$PWD/smflocation_p.h \
	$$PWD/smflyrics_p.h \
	$$PWD/smfmusicfingerprint_p.h \
	$$PWD/smfmusicprofile_p.h \
	$$PWD/smfmusicrating_p.h \
	$$PWD/smfpicture_p.h \
	$$PWD/smfplaylist_p.h \
	$$PWD/smfpost_p.h \
	$$PWD/smfsubtitle_p.h \
	$$PWD/smftrackinfo_p.h \
	$$PWD/smfobserver.h

SOURCES += \
	$$PWD/smfalbum.cpp \
	$$PWD/smfartists.cpp \
	$$PWD/smfcomment.cpp \
	$$PWD/smfcontact.cpp \
	$$PWD/smfevent.cpp \
	$$PWD/smfgroup.cpp \
	$$PWD/smflocation.cpp \
	$$PWD/smflyrics.cpp \
	$$PWD/smfmusicfingerprint.cpp \
	$$PWD/smfmusicprofile.cpp \
	$$PWD/smfmusicrating.cpp \
	$$PWD/smfpicture.cpp \
	$$PWD/smfplaylist.cpp \
	$$PWD/smfpluginutil.cpp \
	$$PWD/smfpost.cpp \
	$$PWD/smfprovider.cpp \
	$$PWD/smfsubtitle.cpp \
	$$PWD/smftrackinfo.cpp

symbian: {
	BLD_INF_RULES.prj_exports += $$PWD/smfclientglobal.h
	BLD_INF_RULES.prj_exports += $$PWD/smfglobal.h
	BLD_INF_RULES.prj_exports += $$PWD/smfobserver.h
	BLD_INF_RULES.prj_exports += $$PWD/smfalbum.h
	BLD_INF_RULES.prj_exports += $$PWD/smfalbum_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smfartists.h
	BLD_INF_RULES.prj_exports += $$PWD/smfartists_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smfcomment.h
	BLD_INF_RULES.prj_exports += $$PWD/smfcomment_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smfcontact.h
	BLD_INF_RULES.prj_exports += $$PWD/smfcontact_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smfevent.h
	BLD_INF_RULES.prj_exports += $$PWD/smfevent_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smfgroup.h
	BLD_INF_RULES.prj_exports += $$PWD/smfgroup_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smflocation.h
	BLD_INF_RULES.prj_exports += $$PWD/smflocation_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smflyrics.h
	BLD_INF_RULES.prj_exports += $$PWD/smflyrics_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smfmusicfingerprint.h
	BLD_INF_RULES.prj_exports += $$PWD/smfmusicfingerprint_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smfmusicprofile.h
	BLD_INF_RULES.prj_exports += $$PWD/smfmusicprofile_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smfmusicrating.h
	BLD_INF_RULES.prj_exports += $$PWD/smfmusicrating_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smfpicture.h
	BLD_INF_RULES.prj_exports += $$PWD/smfpicture_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smfplaylist.h
	BLD_INF_RULES.prj_exports += $$PWD/smfplaylist_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smfpluginutil.h
	BLD_INF_RULES.prj_exports += $$PWD/smfpost.h
	BLD_INF_RULES.prj_exports += $$PWD/smfpost_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smfprovider.h
	BLD_INF_RULES.prj_exports += $$PWD/smfsubtitle.h
	BLD_INF_RULES.prj_exports += $$PWD/smfsubtitle_p.h
	BLD_INF_RULES.prj_exports += $$PWD/smftrackinfo.h
	BLD_INF_RULES.prj_exports += $$PWD/smftrackinfo_p.h
}
