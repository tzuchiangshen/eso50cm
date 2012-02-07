#-------------------------------------------------
#
# Project created by QtCreator 2010-10-26T11:54:16
#
# qmake-4.6 -spec macx-g++ qtcli.pro
#-------------------------------------------------

QT       += core gui network

TARGET    = qtcli
TEMPLATE  = app


SOURCES  += main.cpp\
            telescopecli.cpp \
            lcuthread.cpp \
            myRS232.cpp \
    commandthread.cpp

HEADERS  += telescopecli.h \
            lcuthread.h \
            myRS232.h \
    commandthread.h

FORMS    += telescopecli.ui
