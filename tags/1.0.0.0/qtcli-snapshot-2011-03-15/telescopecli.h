#ifndef TELESCOPECLI_H
#define TELESCOPECLI_H

#include <QMainWindow>
#include <QTimer>
#include <QSocketNotifier>
#include <QtDebug>
#include <QDialog>
#include <QLabel>

#include "lcuthread.h"
#include "myRS232.h"
#include "commandthread.h"
#include "cppContainerServices.h"

namespace Ui {
    class TelescopeCli;
}

class TelescopeCli : public QMainWindow
{
    Q_OBJECT

public:
    explicit TelescopeCli( QWidget *parent = 0 );
    ~TelescopeCli();

private slots:
    void connectToServer( void );
    void disconnectFromServer( void );
    void showData( const QString & data );
    void displayError( int socketError, const QString & message );
    void processOneThing( void );
    void cliConnected( void );
    void cliDisconnected( void );
    void theSkyMessage( void );
    void handsetMessage( void );
    void informationMessage( QString message );

private:
    Ui::TelescopeCli *ui;

    lcuThread thread;
    QString currentData;
    QTimer * timer;

    int thesky_fd;
    myRS232 * thesky;
    QSocketNotifier * thesky_notifier;

    int handset_fd;
    myRS232 * handset;
    QSocketNotifier * handset_notifier;

    commandThread command_tread;
    
    //commandThread handset_command_tread;
    bool m_thesky_waitanswer;

	cppContainerServices *cs;
	LCUPrx lcu;
};

#endif // TELESCOPECLI_H
