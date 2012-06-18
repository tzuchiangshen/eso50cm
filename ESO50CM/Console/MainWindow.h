#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include "MainController.h"
#include "LCU.h"

namespace Ui {
    class MainWindow;
    class TelescopeForm;
    class encoderForm;
    class offsetForm;
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


private:
    void createOffsetDocking();
    void createEncoderDocking();
    void createWebcamDocking();
    void createSystemDocking();
    void createLoggerDocking();
    void createCentralWidget();

    //main windows controller
    MainController *mainController;
    Ui::MainWindow *ui;
    Ui::TelescopeForm *uiTelescope;
    Ui::encoderForm *uiEncoder;
    Ui::offsetForm *uiOffset;

    QTextEdit *textEdit1;
    QTextEdit *textEdit2;
};

#endif // MAINWINDOW_H
