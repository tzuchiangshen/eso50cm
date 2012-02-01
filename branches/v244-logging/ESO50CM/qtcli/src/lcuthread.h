#ifndef LCUTHREAD_H
#define LCUTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtNetwork>
#include <QString>


class lcuThread : public QThread
{
    Q_OBJECT

public:
    lcuThread(QObject *parent = 0);
    ~lcuThread();

    void requestNewData( const QString & hostName, quint16 port );
    void run( void );
    void old_run( void );
    void quitRun( void );

signals:
    void newData( const QString & data );
    void error( int socketError, const QString & message );
    void runConnected( void );
    void runDisconnected( void );

private:
    QString hostName;
    quint16 port;
    QMutex mutex;
    QWaitCondition cond;
    bool m_quit;
};

#endif // LCUTHREAD_H
