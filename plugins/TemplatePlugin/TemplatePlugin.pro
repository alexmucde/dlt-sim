#! [0]
TEMPLATE        = lib
CONFIG         += plugin
QT             += widgets network
INCLUDEPATH    += ../../dlt-sim-main
TARGET          = $$qtLibraryTarget(TemplatePlugin)
DESTDIR         = ..
#! [0]

EXAMPLE_FILES = TemplatePlugin.json

SOURCES += \
    TemplatePlugin.cpp \
    Templateform.cpp \
    Templatesettingsdialog.cpp
	
HEADERS += \
    TemplatePlugin.h \
    Templateform.h \
    Templatesettingsdialog.h

FORMS += \
    Templateform.ui \
    Templatesettingsdialog.ui

DISTFILES += \
    TemplatePlugin.json
