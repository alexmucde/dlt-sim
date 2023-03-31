#! [0]
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets serialport network
INCLUDEPATH    += ../../dlt-sim-main
TARGET          = $$qtLibraryTarget(DLTRelaisPlugin)
DESTDIR         = ..
#! [0]

EXAMPLE_FILES = dltrelaisplugin.json

SOURCES += \
    ../../dlt-sim-main/dltminiserver.cpp \
    DLTRelaisPlugin.cpp \
    DLTRelaisform.cpp \
    DLTRelaissettingsdialog.cpp
	
HEADERS += \
    ../../dlt-sim-main/dltminiserver.h \
    DLTRelaisPlugin.h \
    DLTRelaisform.h \
    DLTRelaissettingsdialog.h

FORMS += \
    DLTRelaisform.ui \
    DLTRelaissettingsdialog.ui

DISTFILES += \
    dltrelaisplugin.json
