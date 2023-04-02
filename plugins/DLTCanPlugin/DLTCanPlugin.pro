#! [0]
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets network serialport
INCLUDEPATH    += ../../dlt-sim-main
TARGET          = $$qtLibraryTarget(DLTCanPlugin)
DESTDIR         = ..
#! [0]

EXAMPLE_FILES = DLTCanPlugin.json

SOURCES += \
    ../../dlt-sim-main/dltminiserver.cpp \
    DLTCanPlugin.cpp \
    DLTCanPluginform.cpp \
    DLTCanPluginsettingsdialog.cpp
	
HEADERS += \
    ../../dlt-sim-main/dltminiserver.h \
    DLTCanPlugin.h \
    DLTCanPluginform.h \
    DLTCanPluginsettingsdialog.h

FORMS += \
    DLTCanPluginform.ui \
    DLTCanPluginsettingsdialog.ui

DISTFILES += \
    DLTCanPlugin.json
