#-------------------------------------------------
#
# Project created by QtCreator 2012-06-13T13:18:58
#
#-------------------------------------------------

QT       += core gui webkit

TARGET = Console
TEMPLATE = app


SOURCES += main.cpp\
    ObsControlIF.cpp \
    MainController.cpp \
    MainWindow.cpp \
    TheSky6IF.cpp \
    AMI_Observing_moveToTargetImpl.cpp \
    AMI_Observing_parkTelescopeCapImpl.cpp \
    AMI_Observing_parkTelescope.cpp \
    LoggerIF.cpp \
    messagetablemodel.cpp

HEADERS  += \
    ObsControlIF.h \
    MainController.h \
    MainWindow.h \
    TheSky6IF.h \
    Console.h \
    AMI_Observing_moveToTargetImpl.h \
    AMI_Observing_parkTelescopeImpl.h \
    AMI_Observing_parkTelescopeCapImpl.h \
    LoggerIF.h \
    messagetablemodel.h



FORMS    += mainwindow.ui \
    telescope.ui \
    encoder.ui \
    offset.ui \
    status.ui \
    logpanel.ui

RESOURCES += \
    Console.qrc


LIBS += -L/eso50cm/SWROOT/lib/ -lLoggerCore -lObservingImpl -lLCUImpl -lIce -lIceUtil -lIceStorm -lLoggerCore
INCLUDEPATH += /eso50cm/SWROOT/include \
               ../Observing/include/
DEPENDPATH +=  /eso50cm/SWROOT/include
