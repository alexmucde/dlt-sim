#! [0]
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets serialport network
INCLUDEPATH    += ../../dlt-sim-main
TARGET          = $$qtLibraryTarget(DLTPowerPlugin)
DESTDIR         = ..
#! [0]

EXAMPLE_FILES = TemplatePlugin.json

SOURCES += \
    ../../dlt-sim-main/dltminiserver.cpp \
    DLTPowerPlugin.cpp \
    DLTPowerform.cpp \
    DLTPowersettingsdialog.cpp
	
HEADERS += \
    ../../dlt-sim-main/dltminiserver.h \
    DLTPowerPlugin.h \
    DLTPowerform.h \
    DLTPowersettingsdialog.h

FORMS += \
    DLTPowerform.ui \
    DLTPowersettingsdialog.ui

DISTFILES += \
    DLTPowerPlugin.json
