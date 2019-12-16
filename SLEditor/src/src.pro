TEMPLATE = app
TARGET = $$PROJECT
DESTDIR = ..

DEFINES += QT_DEPRECATED_WARNINGS

QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

RESOURCES += resources.qrc
#RC_FILE = icon.rc
