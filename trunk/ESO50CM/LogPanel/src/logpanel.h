#ifndef LOGPANEL_H
#define LOGPANEL_H

/* QT includes */

#include <QtGui/QWidget>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QHash>
#include <QTimer>
#include <QThread>
#include <QSortFilterProxyModel>

/* C++/STL includes */
#include <string>
#include <iostream>


#include "messagetablemodel.h"

namespace Ui
{
    class LogPanel;
}


class LogPanel : public QWidget
{
    Q_OBJECT
public slots:
    void setProxyFilter( int );
    void searchNext();
    void searchPrevious();
    void clearStatus();
public:
    LogPanel(QWidget *parent = 0);
    QList<LogMessageQT> *list;
    ~LogPanel();
    QSpacerItem *verticalSpacer;
    MessageTableModel *model;
    QSortFilterProxyModel *proxyModel;
private:
    Ui::LogPanel *ui;


};

class LogCapturerI : public LogPublisher {
  public:
    LogPanel *gui;
    LogCapturerI(LogPanel *w) { gui=w; }
    void logEvent(const LogMessageData &message, const Ice::Current& c);
};

class Subscriber : public Application  {

  public:
    LogPanel *gui;
    Subscriber(LogPanel *mw) {gui=mw;}
        TopicManagerPrx get_topic_manager() {
                PropertiesPtr properties = communicator()->getProperties();

                const string prop_key = "IceStormAdmin.TopicManager.Default";
                string strproxy = properties->getProperty(prop_key);

                if (strproxy.empty()) {
                        cerr << appName() << ": property `" << prop_key << "' not set" << endl;
                        return 0;
                }

                cout << "Using IceStorm in '" << strproxy << "' " << endl;

                ObjectPrx base = communicator()->stringToProxy(strproxy);
                return TopicManagerPrx::checkedCast(base);
        }
        virtual int run(int argc, char* argv[]) {

                TopicManagerPrx topic_mgr = get_topic_manager();
                if (!topic_mgr) {
                        cerr << appName() << ": invalid proxy" << endl;
                        return EXIT_FAILURE;
                }

                // Create the servant to receive the events.
                ObjectAdapterPtr adapter = communicator()->createObjectAdapter("LogPublisher.Subscriber");
                ObjectPtr servant = new LogCapturerI(gui);

                // Add a Servant for the Ice Object.
                ObjectPrx base = adapter->addWithUUID(servant);
                TopicPrx topic;

                try {
                        topic = topic_mgr->retrieve("LogTopic");
                        QoS qos;
                        topic->subscribeAndGetPublisher(qos, base);
                }
                catch(const NoSuchTopic& e){
                        cerr << appName() << ": " << e << " name: " << e.name << endl;
                        return EXIT_FAILURE;
                }

                cout << "Waiting for events... " << base << endl;
                adapter->activate();
                shutdownOnInterrupt();
                communicator()->waitForShutdown();
                topic->unsubscribe(base);
                return EXIT_SUCCESS;
        }
};


#endif // LOGPANEL_H
