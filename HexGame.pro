#-------------------------------------------------
#
# Project created by QtCreator 2011-07-25T08:37:52
#
#-------------------------------------------------

QT       += core gui
#QT       += core gui phonon

TARGET = HexGame
TEMPLATE = app


SOURCES += main.cpp\
    ball.cpp \
    corecontroller.cpp \
    rules.cpp \
    sixtyonegameboardinfo.cpp \
    gamemath.cpp \
    basicpainter.cpp \
    gesturecontroller.cpp \
    effectpainter.cpp \
    mainwidget.cpp \
    mainmenuitems.cpp \
    rotatepuzzlegame.cpp \
    mainmenuwidget.cpp \
    puzzlemenuitems.cpp \
    puzzlemenuwidget.cpp \
    stagemenuwidget.cpp \
    stagemenuitems.cpp \
    puzzlegameinit.cpp \
    thirtysevengameboardinfo.cpp \
    gamecommonitems.cpp \
    gamerecord.cpp \
    pixmapoperations.cpp \
    classicgamewidget.cpp \
    othergameinit.cpp \
    resetwidget.cpp \
    endlessgamewidget.cpp \
    gameoverwidget.cpp \
    pausewidget.cpp \
    timinggamewidget.cpp \
    helpwidget.cpp \
    publicgamesounds.cpp \
    achievementitem.cpp \
    statistic.cpp \
    achievements.cpp \
    achievementwidget.cpp \
    puzzlegamerecord.cpp \
    helpmainpartwidget.cpp \
    helpwidget2.cpp \
    helpitem.cpp \
    nextstagewidget.cpp \
    soundplayer.cpp \
    twoplayertiminggamewidget.cpp

HEADERS  += ball.h \
    corecontroller.h \
    abstractrule.h \
    rules.h \
    abstractgameboardinfo.h \
    sixtyonegameboardinfo.h \
    connections.h \
    gamemath.h \
    basicpainter.h \
    gesturecontroller.h \
    abstractpixmapwidget.h \
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
    puzzlegameinit.h \
    thirtysevengameboardinfo.h \
    gamecommonitems.h \
    gamerecord.h \
    pixmapoperations.h \
    classicgamewidget.h \
    othergameinit.h \
    resetwidget.h \
    endlessgamewidget.h \
    gameoverwidget.h \
    pausewidget.h \
    timinggamewidget.h \
    helpwidget.h \
    publicgamesounds.h \
    config.h \
    achievementitem.h \
    statistic.h \
    achievements.h \
    achievementwidget.h \
    puzzlegamerecord.h \
    helpmainpartwidget.h \
    helpwidget2.h \
    helpitem.h \
    nextstagewidget.h \
    soundplayer.h \
    twoplayertiminggamewidget.h

RESOURCES += \
    puzzlegames.qrc \
    sounds.qrc \
    stageitemsimages.qrc \
    puzzlemenuitemsimages.qrc \
    mainmenuitemsimages.qrc \
    hintimages.qrc \
    helpimages.qrc \
    gamecommonitemsimages.qrc \
    buttonsimages.qrc \
    bonusimages.qrc \
    ballsimages.qrc \
    backgroundsimages.qrc \
    achievementimages.qrc

#LIBS += -lgstreamer-0.10

#INCLUDEPATH +=/usr/include/glib-2.0/ \
#             /usr/include/glib-2.0/glib/ \
#             /usr/include/gstreamer-0.10/ \
#             /usr/lib/glib-2.0/include/ \
#             /usr/include/libxml2/
