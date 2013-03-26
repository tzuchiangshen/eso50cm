#ifndef LOGLEVELPANEL_H
#define LOGLEVELPANEL_H
#include "LoggerHelper.h"
#include <QtGui/QWidget>
#include <QString>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QHash>
#include <QTimer>

namespace Ui
{
    class LogLevelPanel;
}



class SourceLevel : public QWidget {
    Q_OBJECT

    public slots:
        void setDiscardLevel(int level);
    signals:
        void levelChanged(QString source,int level);
    public:
        SourceLevel(QString srcName,int currentLvl,QWidget *parent = 0);
        void setupUI(const int initialLevel);
        QString source;
        QHBoxLayout *horizontalLayout;
        QCheckBox *cb;
        QLabel *sourceName;
        QLabel *currentLevel;
        QComboBox *levelSelection;

};


class LogLevelPanel : public QWidget
{
    Q_OBJECT
public slots:
    void setAllDiscardLevels();
    void setDiscardLevel(QString source,int level);
    void markAll();
    void unmarkAll();
    void refreshSources();

public:
    LogLevelPanel(QWidget *parent = 0);
    ~LogLevelPanel();
    QHash<QString, SourceLevel*> sourcesList;
    QSpacerItem *verticalSpacer;

    void getDiscardLevel();
private:
    Ui::LogLevelPanel *ui;
    LoggerHelper logger;
    QTimer *timer;
};



#endif // LOGLEVELPANEL_H
