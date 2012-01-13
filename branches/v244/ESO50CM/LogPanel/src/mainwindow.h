#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//#include <dbstuff.h>   // hacked for development
#include <QtGui/QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <string>
#include <Ice/Application.h>
#include <IceStorm/IceStorm.h>
#include <IceUtil/UUID.h>
#include "LogPublisher.h"

using namespace std;
using namespace Ice;
using namespace IceStorm;
using namespace Log;

using std::string;
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addLog(int level, double timestamp, int sourceId,string data);
    void addLog(int level, double timestamp, string source,string data);

private:
    Ui::MainWindow *ui;
//    MySQLCon db;  // Hacked for development
    void logEvent(const LogMessageData &message, const Ice::Current& c);
    string getTimeString(double timestamp);
    string getSourceDesc(int sourceId);
    string getLevelDesc(int level);
    string levelDesc[10];
    int levelDescToNumber(QString level);

private slots:
    void on_searchButton_clicked();
    void on_actionSave_logs_triggered();
    void on_detailsTable_cellClicked(int row, int column);
    void on_logTable_cellClicked(int row, int column);
    void on_dbConnection_clicked(bool checked);
    void on_logLevelFilter_currentIndexChanged(int index);
};

class SimpleClientI : public LogPublisher {
  public:
    MainWindow *gui;
    SimpleClientI(MainWindow *w) { gui=w; }
    void logEvent(const LogMessageData &message, const Ice::Current& c) {
        if (gui)
	    gui->addLog(message.level, message.timestamp, message.source,message.message);
    }
};

class Subscriber : public Application  {

  public:
    MainWindow *gui;
    Subscriber(MainWindow *mw) {gui=mw;}
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

public:
	virtual int run(int argc, char* argv[]) {

		TopicManagerPrx topic_mgr = get_topic_manager();
		if (!topic_mgr) {
			cerr << appName() << ": invalid proxy" << endl;
			return EXIT_FAILURE;
		}

		// Create the servant to receive the events.
		ObjectAdapterPtr adapter = communicator()->createObjectAdapter("LogPublisher.Subscriber");
		ObjectPtr servant = new SimpleClientI(gui);

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



#endif // MAINWINDOW_H
