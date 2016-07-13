#-------------------------------------------------
#
# Project created by QtCreator 2016-06-06T11:23:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = firstopencv
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    car.cpp

HEADERS  += mainwindow.h \
    car.h

FORMS    += mainwindow.ui

LIBS += `pkg-config opencv --libs`
