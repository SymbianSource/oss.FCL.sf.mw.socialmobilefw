INCLUDEPATH += server \
    ./

PUBLIC_HEADERS += \
	server/smfserver.h 

SOURCES += server/smfserver.cpp

symbian {
    PRIVATE_HEADERS += server/smfserversymbian_p.h 
    SOURCES += server/smfserversymbian.cpp
} else {
    PRIVATE_HEADERS += server/smfserverqt_p.h \
                       server/smfserverqtsession.h
    SOURCES += server/smfserverqt.cpp \
               server/smfserverqtsession.cpp
}
