#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include "MainController.h"
#include "LCU.h"
#include "Console.h"

namespace Ui {
    class MainWindow;
    class TelescopeForm;
    class encoderForm;
    class offsetForm;
    class statusForm;
}

class MainController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void testSlots(bool);
    void showData(int, OUC::TelescopeData* );
    void showEncData(int,OUC::RawEncoderData*);
    void northButtonPressed();
    void southButtonPressed();
    void eastButtonPressed();
    void westButtonPressed();
    void northButtonReleased();
    void southButtonReleased();
    void eastButtonReleased();
    void westButtonReleased();
    //toolbar 
    void stopTelescope();
    void gotoTarget();
    void parkTelescope();
    void parkTelescopeCap();
    void startTracking();
    void stopTracking();
    //menu
    void openLogsLevelPanel();
    void openLogsPanel();
    //status bar
    void updateTelescopeStatus(TelescopeStatus status);
    void updateObsControlStatus(ProcessStatus status);
    void updateLCUControlStatus(ProcessStatus status);
    void updateTrackingStatus(TrackingStatus status);
    void updateTheSky6Status(ProcessStatus status);


private:
    void createOffsetDocking();
    void createEncoderDocking();
    void createWebcamDocking();
    void createSystemDocking();
    void createLoggerDocking();
    void createCentralWidget();
    void createStatusBar();

    //main windows controller
    MainController *mainController;
    Ui::MainWindow *ui;
    Ui::TelescopeForm *uiTelescope;
    Ui::encoderForm *uiEncoder;
    Ui::offsetForm *uiOffset;
    Ui::statusForm *uiStatus;

    //status bar
    QToolButton *telescopeStatus; //park, park_cap, moving
    QToolButton *trackingStatus;  //tracking on, tracking off
    QToolButton *theSky6Status;   //connected, disconneced
    QToolButton *obsControlStatus; //connected, disconnected
    QToolButton *lcuControlStatus; //connected, disconnected

    //QTextEdit *textEdit1;
    //QTextEdit *textEdit2;
};

#endif // MAINWINDOW_H
