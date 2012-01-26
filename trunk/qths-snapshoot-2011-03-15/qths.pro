#-------------------------------------------------
#
# Project created by QtCreator 2010-10-26T11:54:16
#
# qmake-4.6 -spec macx-g++ qths.pro
#-------------------------------------------------

QT       += core gui network

TARGET    = qths
TEMPLATE  = app


SOURCES  += main.cpp\
            hscli.cpp \
            lcuthread.cpp \
            myRS232.cpp \
            commandthread.cpp

HEADERS  += hscli.h \
            lcuthread.h \
            myRS232.h \
            commandthread.h

FORMS    += hscli.ui
