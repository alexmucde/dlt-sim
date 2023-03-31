#! [0]
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets serialport network
INCLUDEPATH    += ../../dlt-sim-main
TARGET          = $$qtLibraryTarget(DLTMultimeterPlugin)
DESTDIR         = ..
#! [0]

EXAMPLE_FILES = DLTMultimeterPlugin.json

SOURCES += \
    ../../dlt-sim-main/dltminiserver.cpp \
    DLTMultimeterPlugin.cpp \
    DLTMultimeterform.cpp \
    DLTMultimetersettingsdialog.cpp
	
HEADERS += \
    ../../dlt-sim-main/dltminiserver.h \
    DLTMultimeterPlugin.h \
    DLTMultimeterform.h \
    DLTMultimetersettingsdialog.h

FORMS += \
    DLTMultimeterform.ui \
    DLTMultimetersettingsdialog.ui

DISTFILES += \
    DLTMultimeterPlugin.json
