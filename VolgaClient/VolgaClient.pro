#-------------------------------------------------
#
# Project created by QtCreator 2016-09-30T21:40:12
#
#-------------------------------------------------

QT       += core gui opengl
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Volga
TEMPLATE = app


SOURCES += main.cpp\
        volgamainwindow.cpp \
    renderopenglwidget.cpp

HEADERS  += volgamainwindow.h \
    renderopenglwidget.h

FORMS    += volgamainwindow.ui
