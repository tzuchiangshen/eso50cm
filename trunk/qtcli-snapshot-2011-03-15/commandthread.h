#ifndef COMMANDTHREAD_H
#define COMMANDTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QtNetwork>
#include <QString>


class commandThread : public QThread
{
    Q_OBJECT

public:
    commandThread(QObject *parent = 0);
    ~commandThread();

    void requestNewConnetion( const QString & hostName, quint16 port, QString command );
    void run( void );
    void quitRun( void );
    void wakeUp( void );

signals:
    void newData( const QString & data );
    void error( int socketError, const QString & message );
    void runConnected( void );
    void runDisconnected( void );

private:
    QString m_hostName;
    quint16 m_port;
    QMutex mutex;
    QWaitCondition cond;
    bool m_quit;
    QString m_command;
};

#endif // COMMANDTHREAD_H
