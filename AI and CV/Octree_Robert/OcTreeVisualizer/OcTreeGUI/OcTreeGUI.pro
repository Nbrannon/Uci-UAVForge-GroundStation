#-------------------------------------------------
#
# Project created by QtCreator 2014-11-23T14:10:52
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OcTreeGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    geometryengine.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    geometryengine.h

FORMS    += mainwindow.ui

RESOURCES += \
        shaders.qrc \
        textures.qrc

