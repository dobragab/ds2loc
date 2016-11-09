#-------------------------------------------------
#
# Project created by QtCreator 2016-11-05T14:33:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ds2loc
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    filecontent.cpp

HEADERS  += mainwindow.h \
    filecontent.h

FORMS    += mainwindow.ui

DISTFILES += \
    .gitignore

QMAKE_CXXFLAGS += -std=c++14
