QT       += core gui widgets network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = dlt-sim

SOURCES += \
    dltminiserver.cpp \
    main.cpp \
    mainwindow.cpp \
    pluginmodel.cpp \
    settings.cpp

HEADERS += \
    dltminiserver.h \
    mainwindow.h \
    plugininterface.h \
    pluginmodel.h \
    settings.h \
    version.h

FORMS += \
    mainwindow.ui \
    settings.ui

DESTDIR = ..

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../README.md \
    ../settings/DLTSim.xml \
    ../tests/DLTPowerPluginTest.dtr \
    ../tests/DLTRelaisPluginTest.dtr
