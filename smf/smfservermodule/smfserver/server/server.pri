INCLUDEPATH += server \
    ./

PUBLIC_HEADERS += \
	server/smfserver.h \
	server/smfsettingshandler.h

SOURCES += server/smfserver.cpp \
           server/smfsettingshandler.cpp

symbian { 
	#Settings handler is currently for only CR keys, QSettings will be provided later
    PRIVATE_HEADERS += server/smfserversymbian_p.h #\
                       #server/smfsettingshandler_symbian.h
    SOURCES += server/smfserversymbian.cpp #\
               #server/smfsettingshandler_symbian.cpp
} else {
    PRIVATE_HEADERS += server/smfserverqt_p.h \
        server/smfserverqtsession.h
    SOURCES += server/smfserverqt.cpp \
        server/smfserverqtsession.cpp
}
