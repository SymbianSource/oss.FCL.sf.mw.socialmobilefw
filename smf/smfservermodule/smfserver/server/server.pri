INCLUDEPATH += server \
    ./

PUBLIC_HEADERS += \
	server/smfserver.h 

PRIVATE_HEADERS += \
	server/smfserverqt_p.h \
	server/smfserversymbian_p.h 

SOURCES += \
	server/smfserver.cpp \
	server/smfserversymbian.cpp 
