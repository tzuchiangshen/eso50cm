#ifndef HSCLI_H
#define HSCLI_H

#include <QMainWindow>
#include <QTimer>
#include <QSocketNotifier>
#include <QtDebug>
#include <QDialog>
#include <QLabel>
#include <QSpinBox>

#include "lcuthread.h"
#include "myRS232.h"
#include "commandthread.h"

namespace Ui {
    class hsCli;
}

class hsCli : public QMainWindow
{
    Q_OBJECT

public:
    explicit hsCli( QWidget *parent = 0 );
    ~hsCli();

private slots:
    //void connectToServer( void );
    //void disconnectFromServer( void );
    //void showData( const QString & data );
    void displayError( int socketError, const QString & message );
    //void processOneThing( void );
    //void cliConnected( void );
    //void cliDisconnected( void );
    //void theSkyMessage( void );

    void handsetMessage( int );
    void alphaMessage( QString message );
    void deltaMessage( QString message );

    void alphaPressed( char );
    void alphaReleased( char );
    void deltaPressed( char );
    void deltaReleased( char );
    void northPressed( void );
    void northReleased( void );
    void southPressed( void );
    void southReleased( void );
    void eastPressed( void );
    void eastReleased( void );
    void westPressed( void );
    void westReleased( void );

    void    connectHSDevice( void );
    void disconnectHSDevice( void );
private:
    Ui::hsCli *ui;

    //lcuThread thread;
    QString currentData;
    QTimer * timer;

    int handset_fd;
    myRS232 * handset;
    QSocketNotifier * handset_notifier;

    commandThread alpha_thread;
    commandThread delta_thread;
};

#endif // HSCLI_H
