#ifndef LOGGERIF_H
#define LOGGERIF_H

/* QT includes */

//#include <QtGui/QWidget>
//#include <QLabel>
//#include <QComboBox>
//#include <QCheckBox>
//#include <QHBoxLayout>
#include <QHash>
#include <QTimer>
#include <QThread>
//#include <QSortFilterProxyModel>

/* C++/STL includes */
#include <string>
#include <iostream>


#include "messagetablemodel.h"
#include "ui_logpanel.h"

//namespace Ui
//{
//    class LogPanel;
//}


//class LogPanel : public QWidget
//{
//    Q_OBJECT
//public slots:
//    void setProxyFilter( int );
//    void searchNext();
//    void searchPrevious();
//    void clearStatus();
//public:
//    LogPanel(QWidget *parent = 0);
//    QList<LogMessageQT> *list;
//    ~LogPanel();
//    QSpacerItem *verticalSpacer;
//    MessageTableModel *model;
//    QSortFilterProxyModel *proxyModel;
//private:
//    Ui::LogPanel *ui;


//};

class LogCapturerI : public LogPublisher {
  public:
    Ui::LogPanel *gui;
    MessageTableModel *model;
    LogCapturerI(Ui::LogPanel *w) { gui=w; }
    LogCapturerI(Ui::LogPanel *w, MessageTableModel *_model) {
        gui=w;
        model = _model;
    }


    void logEvent(const LogMessageData &message, const Ice::Current& c);
};

//class Subscriber : public Application  {

//  public:
//    Ui::LogPanel *gui;
//    MessageTableModel *model;
//    Subscriber(Ui::LogPanel *mw) {gui=mw;}
//    Subscriber(Ui::LogPanel *mw, MessageTableModel *model) {
//        gui=mw;
//        model = model;
//    }


//    TopicManagerPrx get_topic_manager() {
//        PropertiesPtr properties = communicator()->getProperties();

//        const string prop_key = "IceStormAdmin.TopicManager.Default";
//        string strproxy = properties->getProperty(prop_key);

//        if (strproxy.empty()) {
//            cerr << appName() << ": property `" << prop_key << "' not set" << endl;
//            return 0;
//        }

//        cout << "Using IceStorm in '" << strproxy << "' " << endl;

//        ObjectPrx base = communicator()->stringToProxy(strproxy);
//        return TopicManagerPrx::checkedCast(base);
//    }

//    virtual int run(int argc, char* argv[]) {

//        cout << "argc=" << argc << endl;
//        cout << "argv=" << *argv << endl;

//        TopicManagerPrx topic_mgr = get_topic_manager();
//        if (!topic_mgr) {
//            cerr << appName() << ": invalid proxy" << endl;
//            return EXIT_FAILURE;
//        }

//        // Create the servant to receive the events.
//        ObjectAdapterPtr adapter = communicator()->createObjectAdapter("LogPublisher.Subscriber");
//        ObjectPtr servant = new LogCapturerI(gui);
//        cout << ">>>>>>>>>>>>>>>>>>>>> llegue!" << endl;

//        // Add a Servant for the Ice Object.
//        ObjectPrx base = adapter->addWithUUID(servant);
//        TopicPrx topic;

//        try {
//            topic = topic_mgr->retrieve("LogTopic");
//            QoS qos;
//            topic->subscribeAndGetPublisher(qos, base);
//        }
//        catch(const NoSuchTopic& e){
//            cerr << appName() << ": " << e << " name: " << e.name << endl;
//            return EXIT_FAILURE;
//        }

//        cout << "Waiting for events... " << base << endl;
//        adapter->activate();
//        shutdownOnInterrupt();
//        communicator()->waitForShutdown();
//        topic->unsubscribe(base);
//        return EXIT_SUCCESS;
//    }
//};

class Subscriber: public QThread {

    public:
        Ice::CommunicatorPtr ic;
        Ui::LogPanel *gui;
        MessageTableModel *model;
        int argc;
        char **argv;
        Subscriber(Ui::LogPanel *mw) {gui=mw;}
        Subscriber(Ui::LogPanel *mw, MessageTableModel *_model, int _argc, char** _argv) {
            gui=mw;
            model = _model;
            argc = _argc;
            argv = _argv;
        }


        TopicManagerPrx get_topic_manager(Ice::CommunicatorPtr ic) {
            PropertiesPtr properties = ic->getProperties();

            const string prop_key = "IceStormAdmin.TopicManager.Default";
            string strproxy = properties->getProperty(prop_key);

            if (strproxy.empty()) {
                cerr << ": property `" << prop_key << "' not set" << endl;
                return 0;
            }

            cout << "Using IceStorm in '" << strproxy << "' " << endl;

            ObjectPrx base = ic->stringToProxy(strproxy);
            return TopicManagerPrx::checkedCast(base);
        }

        void run() {
            cout << " todavia veo al model=" << model << endl;
            _run(argc, argv, model);
        }

        virtual int _run(int argc, char** argv, MessageTableModel *_model) {

            cout << "argc=" << argc << endl;
            cout << "argv=" << *argv << endl;

            ic = Ice::initialize(argc, argv);

            TopicManagerPrx topic_mgr = get_topic_manager(ic);
            if (!topic_mgr) {
                cerr << ": invalid proxy" << endl;
                return EXIT_FAILURE;
            }

            // Create the servant to receive the events.
            ObjectAdapterPtr adapter = ic->createObjectAdapter("LogPublisher.Subscriber");
            ObjectPtr servant = new LogCapturerI(gui, _model);
            cout << ">>>>>>>>>>>>>>>>>>>>> dentro del subscriber._run()! model=" << _model << endl;

            // Add a Servant for the Ice Object.
            ObjectPrx base = adapter->addWithUUID(servant);
            TopicPrx topic;

            try {
                topic = topic_mgr->retrieve("LogTopic");
                QoS qos;
                topic->subscribeAndGetPublisher(qos, base);
            }
            catch(const NoSuchTopic& e){
                cerr << ": " << e << " name: " << e.name << endl;
                return EXIT_FAILURE;
            }

            cout << "Waiting for events... " << base << endl;
            adapter->activate();
            //shutdownOnInterrupt();
            ic->waitForShutdown();
            topic->unsubscribe(base);
            return EXIT_SUCCESS;
        }
    };



//    Ui::LogPanel *gui;
//    MessageTableModel *model;
//    Subscriber(Ui::LogPanel *mw) {gui=mw;}
//    Subscriber(Ui::LogPanel *mw, MessageTableModel *model) {
//        gui=mw;
//        model = model;
//    }


//    TopicManagerPrx get_topic_manager() {
//        PropertiesPtr properties = communicator()->getProperties();

//        const string prop_key = "IceStormAdmin.TopicManager.Default";
//        string strproxy = properties->getProperty(prop_key);

//        if (strproxy.empty()) {
//            cerr << appName() << ": property `" << prop_key << "' not set" << endl;
//            return 0;
//        }

//        cout << "Using IceStorm in '" << strproxy << "' " << endl;

//        ObjectPrx base = communicator()->stringToProxy(strproxy);
//        return TopicManagerPrx::checkedCast(base);
//    }

//    virtual int run(int argc, char* argv[]) {

//        cout << "argc=" << argc << endl;
//        cout << "argv=" << *argv << endl;

//        TopicManagerPrx topic_mgr = get_topic_manager();
//        if (!topic_mgr) {
//            cerr << appName() << ": invalid proxy" << endl;
//            return EXIT_FAILURE;
//        }

//        // Create the servant to receive the events.
//        ObjectAdapterPtr adapter = communicator()->createObjectAdapter("LogPublisher.Subscriber");
//        ObjectPtr servant = new LogCapturerI(gui);
//        cout << ">>>>>>>>>>>>>>>>>>>>> llegue!" << endl;

//        // Add a Servant for the Ice Object.
//        ObjectPrx base = adapter->addWithUUID(servant);
//        TopicPrx topic;

//        try {
//            topic = topic_mgr->retrieve("LogTopic");
//            QoS qos;
//            topic->subscribeAndGetPublisher(qos, base);
//        }
//        catch(const NoSuchTopic& e){
//            cerr << appName() << ": " << e << " name: " << e.name << endl;
//            return EXIT_FAILURE;
//        }

//        cout << "Waiting for events... " << base << endl;
//        adapter->activate();
//        shutdownOnInterrupt();
//        communicator()->waitForShutdown();
//        topic->unsubscribe(base);
//        return EXIT_SUCCESS;
//    }
//};

#endif // LOGGERIF_H
