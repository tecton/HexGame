QT       += core gui

TARGET = HexGame
TEMPLATE = app

SOURCES += main.cpp\
    gamelocation.cpp \
    gameviewwidget.cpp \
    pixmapitem.cpp \
    gamegraphicsview.cpp

HEADERS  += gamelocation.h \
    gameviewwidget.h \
    pixmapitem.h \
    gamegraphicsview.h

RESOURCES += \
    resource.qrc
