#-------------------------------------------------
#
# Project created by QtCreator 2017-01-18T16:50:56
#
#-------------------------------------------------

QT       += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TCAS
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    broadcaster.cpp \
    scenemanager.cpp \
    sceneitems.cpp

HEADERS  += widget.h \
    broadcaster.h \
    scenemanager.h \
    sceneitems.h

FORMS    += widget.ui

DISTFILES +=

RESOURCES += \
    MyResources.qrc
