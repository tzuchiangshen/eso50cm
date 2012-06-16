#ifndef TELESCOPE_H
#define TELESCOPE_H

#include <QThread>
#include "TheSky6IF.h"
#include "ObsControlIF.h"
#include "Observing.h"
#include "LoggerHelper.h"


class MainController: public QThread
{
    Q_OBJECT

public:
    explicit MainController(QWidget *parent = 0);
    ~MainController();
    //Ui::TelescopeForm *ui;
public slots:
    //void testSlots(bool);

private:
    ObsControlIF *controller;
    TelescopePrx lcu;
    LoggerHelper logger;
};

#endif // TELESCOPE_H
