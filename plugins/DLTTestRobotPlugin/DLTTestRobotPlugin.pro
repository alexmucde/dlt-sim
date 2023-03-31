#! [0]
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets network
INCLUDEPATH    += ../../dlt-sim-main
TARGET          = $$qtLibraryTarget(DLTTestRobotPlugin)
DESTDIR         = ..
#! [0]

EXAMPLE_FILES = DLTTestRobotPluginPlugin.json

SOURCES += \
    ../../dlt-sim-main/dltminiserver.cpp \
    DLTTest.cpp \
    DLTTestRobotPlugin.cpp \
    DLTTestRobotPluginform.cpp \
    DLTTestRobotPluginsettingsdialog.cpp
	
HEADERS += \
    ../../dlt-sim-main/dltminiserver.h \
    DLTTest.h \
    DLTTestRobotPlugin.h \
    DLTTestRobotPluginform.h \
    DLTTestRobotPluginsettingsdialog.h

FORMS += \
    DLTTestRobotPluginform.ui \
    DLTTestRobotPluginsettingsdialog.ui

DISTFILES += \
    DLTTestRobotPlugin.json
