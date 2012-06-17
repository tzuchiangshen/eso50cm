#ifndef TELESCOPE_H
#define TELESCOPE_H

#include <QThread>
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
    MainController();
    ~MainController();
public slots:
    void testSlots(bool);

private:

    TelescopePrx lcu;
    LoggerHelper logger;
};

#endif // TELESCOPE_H
