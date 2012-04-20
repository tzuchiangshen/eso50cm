#include "logpanel.h"
#include "ui_logpanel.h"
#include <string>
#include <vector>




LogPanel::LogPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::LogPanel)
{
    ui->setupUi(this);
    //verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //ui->sourceWidgetLayou->addItem(verticalSpacer);
    list=new QList<LogMessageQT>;
    model=new MessageTableModel(this);
    model->setList(list,100000);  //   <--- MAX NUMBER OF LOGS!!
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setFilterKeyColumn(1);
    setProxyFilter(2); // default value for this is info
    connect(ui->cbFilter,SIGNAL(currentIndexChanged(int)),this,SLOT(setProxyFilter(int)));
    ui->tableView->setModel(proxyModel);
    ui->tableView->setColumnWidth(0,175);
    ui->tableView->setColumnWidth(1,80);
    ui->tableView->setColumnWidth(2,140);
    ui->tableView->setColumnWidth(3,600);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    proxyModel->sort(0, Qt::DescendingOrder);
}
void LogPanel::setProxyFilter( int filter)
{
    switch(filter) {
      case 0:
            proxyModel->setFilterRegExp("(^Severe$)"); return;
      case 1:
            proxyModel->setFilterRegExp("(^Severe$|^Warning$)"); return;
      case 2:
            proxyModel->setFilterRegExp("(^Severe$|^Warning$|^Info$)"); return;
      case 3:
            proxyModel->setFilterRegExp("(^Severe$|^Warning$|^Info$|^Config$)"); return;
      case 4:
            proxyModel->setFilterRegExp("(^Severe$|^Warning$|^Info$|^Config$|^Fine$)"); return;
      case 5:
            proxyModel->setFilterRegExp("(^Severe$|^Warning$|^Info$|^Config$|^Fine$|^Finer$)"); return;
      case 6:
            proxyModel->setFilterRegExp("(^Severe$|^Warning$|^Info$|^Config$|^Fine$|^Finer$|^Finest$)"); return;
    }
}
LogPanel::~LogPanel()
{
    delete ui;
    delete model;
    delete list;
}


void LogCapturerI::logEvent(const LogMessageData &message, const Ice::Current& c) {
    if (gui) {
        gui->model->addMessage(message);
   }
}


