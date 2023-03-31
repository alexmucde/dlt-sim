#! [0]
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets network
INCLUDEPATH    += ../../dlt-sim-main
TARGET          = $$qtLibraryTarget(DLTCanPlugin)
DESTDIR         = ..
#! [0]

EXAMPLE_FILES = DLTCanPlugin.json

SOURCES += \
    DLTCanPlugin.cpp \
    DLTCanPluginform.cpp \
    DLTCanPluginsettingsdialog.cpp
	
HEADERS += \
    DLTCanPlugin.h \
    DLTCanPluginform.h \
    DLTCanPluginsettingsdialog.h

FORMS += \
    DLTCanPluginform.ui \
    DLTCanPluginsettingsdialog.ui

DISTFILES += \
    DLTCanPlugin.json
