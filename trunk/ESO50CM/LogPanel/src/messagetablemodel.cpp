#include "messagetablemodel.h"

#include <time.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <QDateTime>


MessageTableModel::MessageTableModel(QObject *parent) :
    QAbstractTableModel(parent),  m_rowCount(0)
{
}

void MessageTableModel::setList( QList<LogMessageQT>  *list, int maxMessages)
{
    m_list=list;
    m_maxMessages=maxMessages;

}

int MessageTableModel::columnCount( const QModelIndex& /* parent */ ) const
{
    return 4;
}

int MessageTableModel::rowCount( const QModelIndex& parent ) const
{
    return m_rowCount;
}
//QDateTime timestampToString(double timestamp)
QString timestampToString(double timestamp)
{

    time_t rawtime=(time_t)timestamp;
    struct tm * timeinfo;
    char buffer [80];
    timeinfo = localtime ( &rawtime );
    strftime (buffer,80,"%Y-%m-%dT%H:%M:%S",timeinfo);
    stringstream ss (stringstream::in | stringstream::out);
    ss << buffer << "." << setfill('0') << setw(3) <<  (int)((timestamp-rawtime)*1000);
   // QDateTime time;
   //  return time.fromString(ss.str().c_str(),"yyyy-MM-ddThh:mm:ss.zzz");
    return ss.str().c_str();
}

QVariant MessageTableModel::data( const QModelIndex& index, int role ) const
{

  /**
        Paiting the backgroud according to the message level
 **/
   if (role == Qt::BackgroundRole &&  index.isValid()){
      if (index.column() == 1) {
        switch( m_list->at(index.row()).level ){
                case 0:
                case 1:
                case 2:
                case 3:
                case 4: return qVariantFromValue(QColor(Qt::white));
                case 5: return qVariantFromValue(QColor(Qt::yellow));
                case 6: return qVariantFromValue(QColor(Qt::red));
                default: return qVariantFromValue(QColor(Qt::white));
        }
      }
    }
   if ( !index.isValid() || role == Qt::ToolTipRole ){
        QString tooltip=timestampToString(m_list->at(index.row()).timestamp)+QString(": Line number: ")+ QString::number(m_list->at(index.row()).lineNumber)+QString(", Method: ")+QString(m_list->at(index.row()).method)+QString(" Log: ")+QString(m_list->at(index.row()).data);
        return QVariant(tooltip);
    }

   if ( !index.isValid() || role != Qt::DisplayRole )
        return QVariant();
    switch(index.column()){

        case 0: return timestampToString(m_list->at(index.row()).logtimestamp);
        //case 0: return timestampToString(m_list->at(index.row()).timestamp);
        case 1:
            switch( m_list->at(index.row()).level ){
                case 0: return "Finest";
                case 1: return "Finer";
                case 2: return "Fine";
                case 3: return "Config";
                case 4: return "Info";
                case 5: return "Warning";
                case 6: return "Severe";
                default: return "unknow";
            }
        case 2: return m_list->at(index.row()).source;
        case 3: return m_list->at(index.row()).data;
        case 4: return m_list->at(index.row()).method;
        case 5: return m_list->at(index.row()).lineNumber;
    }
    return QVariant();

}



/*QList<QObject*> MessageTableModel::children( QObject* parent ) const
{
    return parent->children();
}*/

QVariant MessageTableModel::headerData( const int section, const Qt::Orientation orient , const int role ) const
{
    if ( orient != Qt::Horizontal || role != Qt::DisplayRole )
        return QAbstractItemModel::headerData( section, orient, role );

    switch ( section ) {
        case 0: return "Logged at";
        //case 0: return "Log sent at";
        case 1: return "Level";
        case 2: return "Source";
        case 3: return "Message";
        case 4: return "Method";
        case 5: return "Line Number";
    }

    return QVariant();
}


void MessageTableModel::addMessage(LogMessageData message)
{
    LogMessageQT messageQT;
    messageQT.data=QString(message.data.c_str());
    messageQT.level=message.level;
    messageQT.lineNumber=message.lineNumber;
    messageQT.logtimestamp=message.logtimestamp;
    messageQT.method=QString(message.method.c_str());
    messageQT.source=QString(message.source.c_str());
    messageQT.timestamp=message.timestamp;
    // if the list reached its max capacity, we will start removing iterms
    beginInsertRows(QModelIndex(), m_rowCount, m_rowCount);
    m_list->push_back(messageQT);
    m_rowCount++;
    endInsertRows();
    if (m_list->size() >= m_maxMessages){
        // if the list reached its max capacity, we will start removing iterms
        beginRemoveRows(QModelIndex(), 0, 0);
        m_list->removeFirst();
        m_rowCount--;
        endRemoveRows();
    }
}

