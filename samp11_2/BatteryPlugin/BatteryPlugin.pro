CONFIG      += plugin debug_and_release
TARGET      = $$qtLibraryTarget(tpbatteryplugin)
TEMPLATE    = lib

HEADERS     = tpbatteryplugin.h
SOURCES     = tpbatteryplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

QT += designer

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(tpbattery.pri)
