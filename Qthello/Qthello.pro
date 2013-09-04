#-------------------------------------------------
#
# Project created by QtCreator 2013-09-03T09:28:47
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Qthello
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chessboard.cpp \
    ipconverter.cpp

HEADERS  += mainwindow.h \
    chessboard.h \
    chess.h \
    ipconverter.h

FORMS    += mainwindow.ui
