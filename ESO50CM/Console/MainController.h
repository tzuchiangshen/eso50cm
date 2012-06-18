#ifndef TELESCOPE_H
#define TELESCOPE_H

#include <QThread>
#include <QSocketNotifier>
#include "TheSky6IF.h"
#include "ObsControlIF.h"
#include "Observing.h"
#include "LoggerHelper.h"
#include "MainWindow.h"

class MainController: public QThread
{
    Q_OBJECT

public:
    ObsControlIF *obsControl;
    TheSky6IF *theSky6;
    MainController();
    ~MainController();
public slots:
    void testSlots(bool);
    void theSkyMessage( void );

private:

    TelescopePrx lcu;
    LoggerHelper logger;
    int theSky6FD;
    QSocketNotifier * theSkyNotifier;
    bool theSky6WaitAnswer;
};

#endif // TELESCOPE_H
