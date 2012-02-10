#include "loglevelpanel.h"
#include "ui_loglevelpanel.h"
#include <string>
#include <vector>


using Log::StringsVector;


LogLevelPanel::LogLevelPanel(QWidget *parent)
    : QWidget(parent), ui(new Ui::LogLevelPanel), logger("LogLevelPanel",false)
{
    sourcesList=QHash<QString, SourceLevel*>();
    ui->setupUi(this);
    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->sourceWidgetLayou->addItem(verticalSpacer);

    connect(ui->selectAll,SIGNAL(clicked()),this,SLOT(markAll()));
    connect(ui->unselectAll,SIGNAL(clicked()),this,SLOT(unmarkAll()));
    connect(ui->applyBt,SIGNAL(clicked()),this,SLOT(setAllDiscardLevels()));
    refreshSources();
    timer=new QTimer(this);
    timer->start(2000); // updating ever 3 sec
    connect(timer,SIGNAL(timeout()),this,SLOT(refreshSources()));
}

LogLevelPanel::~LogLevelPanel()
{
    delete ui;
}

void LogLevelPanel::markAll()
{
 QHash<QString, SourceLevel*>::iterator i;
 for (i = sourcesList.begin(); i != sourcesList.end(); ++i)
     i.value()->cb->setChecked(true);
}

void LogLevelPanel::unmarkAll()
{
 QHash<QString, SourceLevel*>::iterator i;
 for (i = sourcesList.begin(); i != sourcesList.end(); ++i)
     i.value()->cb->setChecked(false);
}


void LogLevelPanel::refreshSources()
{

    StringsVector newSources=logger.getSources(sourcesList.size());

    if (newSources.size()>0){
        ui->sourceWidgetLayou->removeItem(verticalSpacer);
        // Add the new sources
        for (unsigned int i=0; i<newSources.size(); i++){
            SourceLevel *p=new SourceLevel(QString(((std::string)newSources[i]).c_str()),logger.getDiscardLevel((std::string)newSources[i]),ui->scrollArea);
            ui->sourceWidgetLayou->addWidget(p,0,Qt::AlignTop);
            sourcesList.insert(QString(((std::string)newSources[i]).c_str()),p);
            connect(sourcesList[QString(((std::string)newSources[i]).c_str())],SIGNAL(levelChanged(QString,int)),this,SLOT(setDiscardLevel(QString,int)));
        }
        ui->sourceWidgetLayou->addItem(verticalSpacer);
        ui->scrollArea->repaint();
    }

}
void LogLevelPanel::setDiscardLevel(QString source,int level)
{
    //cout << "LLP::setDiscardLevel: Called set discard level. Source: " << source.toStdString() << ". Level: "<< level << endl;
    logger.setDiscardLevel((Log::LogLevel)level,source.toStdString());
}
void LogLevelPanel::setAllDiscardLevels()
{
    int level=ui->comboBox->currentIndex();
    QHash<QString, SourceLevel*>::iterator i;
    for (i = sourcesList.begin(); i != sourcesList.end(); ++i)
         if(i.value()->cb->isChecked())
        {
            //cout << "LLP::setAllDiscardLevels: Called set discard level. Source: " << i.value()->source.toStdString() << ". Level: "<< level << endl;
            i.value()->levelSelection->setCurrentIndex(level);
        }
}


SourceLevel::SourceLevel(QString srcName,int currentLvl,QWidget *parent)
        : QWidget(parent)
{
        source=srcName;
        setupUI(currentLvl);
};

void SourceLevel::setupUI(const int initialLevel) {
        this->setObjectName(source+QString("SourceLevel"));

        /* We get the high and size of the word 'Warning' */
        QFontMetrics metrics(font());
        QRect messageRect1 = metrics.boundingRect(tr("Warning"));


        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        //sizePolicy1.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
        this->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(this);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setMargin(5);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        cb = new QCheckBox(this);
        cb->setObjectName(QString::fromUtf8("cb"));

        horizontalLayout->addWidget(cb);

        sourceName = new QLabel(this);
        sourceName->setObjectName(QString::fromUtf8("sourceName"));        
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        //sizePolicy2.setHorizontalStretch(0);
        //sizePolicy2.setVerticalStretch(0);
        //sizePolicy2.setHeightForWidth(sourceName->sizePolicy().hasHeightForWidth());

        sourceName->setSizePolicy(sizePolicy2);
        sourceName->setFrameShape(QFrame::StyledPanel);
        sourceName->setText(source);
        horizontalLayout->addWidget(sourceName);

        currentLevel = new QLabel(this);
        currentLevel->setObjectName(QString::fromUtf8("currentLevel"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);

        //sizePolicy3.setHorizontalStretch(0);
        //sizePolicy3.setVerticalStretch(0);
        //sizePolicy3.setHeightForWidth(currentLevel->sizePolicy().hasHeightForWidth());
        currentLevel->setSizePolicy(sizePolicy3);
        currentLevel->setFrameShape(QFrame::StyledPanel);

        horizontalLayout->addWidget(currentLevel);

        levelSelection = new QComboBox(this);
        levelSelection->setObjectName(QString::fromUtf8("levelSelection"));
        levelSelection->addItem("Finest");
        levelSelection->addItem("Finer");
        levelSelection->addItem("Fine");
        levelSelection->addItem("Config");
        levelSelection->addItem("Info");
        levelSelection->addItem("Warning");
        levelSelection->addItem("Severe");
        levelSelection->setCurrentIndex(initialLevel);

        horizontalLayout->addWidget(levelSelection);

        currentLevel->setText(levelSelection->currentText());
        connect(levelSelection,SIGNAL(currentIndexChanged(int)),this,SLOT(setDiscardLevel(int)));
    }

    void SourceLevel::setDiscardLevel(int level)
    {
        currentLevel->setText(levelSelection->currentText());
        //cout << "SourceLevel::setDiscardLevel: Called set discard level. Source: " << source.toStdString() << ". Level: "<< level << endl;
        emit levelChanged(source,level);

    }
