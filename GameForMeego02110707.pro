#-------------------------------------------------
#
# Project created by QtCreator 2011-07-07T08:24:31
#
#-------------------------------------------------

QT       += core gui

TARGET = GameForMeego02110707
TEMPLATE = app
target.path=/usr/local/bin
INSTALLS=target


SOURCES += main.cpp\
        mainwindow.cpp \
    gamelocation.cpp \
    gameviewwidget.cpp \
    pixmapitem.cpp

HEADERS  += mainwindow.h \
    gamelocation.h \
    gameviewwidget.h \
    pixmapitem.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
