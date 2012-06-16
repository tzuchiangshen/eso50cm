#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include "MainController.h"

namespace Ui {
    class MainWindow;
    class TelescopeForm;
    class encoderForm;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void testSlots(bool);

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

    QTextEdit *textEdit1;
    QTextEdit *textEdit2;
};

#endif // MAINWINDOW_H
