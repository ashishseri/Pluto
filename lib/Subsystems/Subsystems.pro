#-------------------------------------------------
#
# Project created by QtCreator 2016-11-11T21:40:27
#
#-------------------------------------------------

QT       += core

TARGET = Subsystems
TEMPLATE = lib
CONFIG += staticlib

SOURCES += subsystems.cpp

HEADERS += subsystems.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
