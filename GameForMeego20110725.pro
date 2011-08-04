#-------------------------------------------------
#
# Project created by QtCreator 2011-07-25T08:37:52
#
#-------------------------------------------------

QT       += core gui

TARGET = GameForMeego20110725
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ball.cpp \
    corecontroller.cpp \
    rules.cpp \
    sixtyonegameboardinfo.cpp \
    gamemath.cpp \
    basicpainter.cpp \
    gesturecontroller.cpp \
    swapclassicgame.cpp \
    effectpainter.cpp \
    mainwidget.cpp \
    mainmenuitems.cpp \
    rotatepuzzlegame.cpp \
    mainmenuwidget.cpp \
    puzzlemenuitems.cpp \
    puzzlemenuwidget.cpp \
    stagemenuwidget.cpp \
    stagemenuitems.cpp \
    puzzlegameinit.cpp

HEADERS  += mainwindow.h \
    ball.h \
    corecontroller.h \
    abstractrule.h \
    rules.h \
    abstractgameboardinfo.h \
    sixtyonegameboardinfo.h \
    connections.h \
    gesture.h \
    gamemath.h \
    basicpainter.h \
    gesturecontroller.h \
    abstractpixmapwidget.h \
    swapclassicgame.h \
    effectpainter.h \
    mainwidget.h \
    abstractitem.h \
    mainmenuitems.h \
    rotatepuzzlegame.h \
    mainmenuwidget.h \
    puzzlemenuitems.h \
    puzzlemenuwidget.h \
    stagemenuwidget.h \
    stagemenuitems.h \
    puzzlegameinit.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
