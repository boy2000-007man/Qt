#-------------------------------------------------
#
# Project created by QtCreator 2013-08-29T15:45:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KDE-RSMT
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    drawwidget.cpp \
    sdk.cpp

HEADERS  += mainwindow.h \
    drawwidget.h \
    sdk.h \
    version.h

FORMS    += mainwindow.ui
