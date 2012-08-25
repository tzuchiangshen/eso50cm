#ifndef MESSAGETABLEMODEL_H
#define MESSAGETABLEMODEL_H

#include <QAbstractTableModel>

/* ICE includes */
#include <Ice/Application.h>
#include <IceStorm/IceStorm.h>
#include <IceUtil/UUID.h>

/* Logging services includes */
#include <LogPublisher.h>

/* QT includes */
#include <QWidget>
#include <QString>
#include <QList>


#define MAX_NUMBER_OF_MESSAGES 30

using namespace Ice;
using namespace IceStorm;
using namespace Log;
using namespace std;


/** QT Version of a LogMessage **/
typedef struct  {
    unsigned int level;
    double logtimestamp;
    double timestamp;
    QString source;
    QString data;
    QString method;
    int lineNumber;
}  LogMessageQT;

class MessageTableModel : public QAbstractTableModel
{

    int m_rowCount;

public:
    MessageTableModel(QObject *parent);
    void setList( QList<LogMessageQT>  *list, int maxMessages);
    int columnCount ( const QModelIndex& parent = QModelIndex() ) const;
    int rowCount ( const QModelIndex& parent = QModelIndex() ) const;
    QVariant data ( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    QVariant headerData( int section, Qt::Orientation, int role = Qt::DisplayRole ) const;
    void addMessage(LogMessageData message);
protected:
  // QList<QObject*> children( QObject* parent ) const;
   // QString label( const QObject* widget, int column ) const;
private:
    QList<LogMessageQT>  *m_list;
    int m_maxMessages;

};

class MyModel : public QAbstractTableModel
{
public:
    MyModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent= QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void addMessage();
};

#endif // MESSAGETABLEMODEL_H
