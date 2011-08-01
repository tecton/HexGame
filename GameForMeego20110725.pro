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
    mainmenu.cpp \
    mainmenuitems.cpp

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
    mainmenu.h \
    abstractitem.h \
    mainmenuitems.h

FORMS    += mainwindow.ui

RESOURCES += \
    resource.qrc
