/********************************************************************************
** Form generated from reading ui file 'telescopecli.ui'
**
** Created: Wed May 25 16:08:01 2011
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TELESCOPECLI_H
#define UI_TELESCOPECLI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TelescopeCli
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_4;
    QTabWidget *tab;
    QWidget *telescopeTab;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *siteBox;
    QGridLayout *gridLayout_3;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_13;
    QLabel *label_12;
    QLineEdit *UT_LineEdit;
    QLineEdit *LT_LineEdit;
    QLineEdit *JD_LineEdit;
    QLineEdit *LST_LineEdit;
    QGroupBox *telescopeBox;
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLineEdit *telHALineEdit;
    QLineEdit *telRALineEdit;
    QLineEdit *telDecLineEdit;
    QLabel *label_7;
    QLineEdit *trgHALineEdit;
    QLineEdit *trgRALineEdit;
    QLineEdit *trgDecLineEdit;
    QLabel *label_15;
    QLineEdit *difHALineEdit;
    QLineEdit *difRALineEdit;
    QLineEdit *difDecLineEdit;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_8;
    QLineEdit *telAltLineEdit;
    QLineEdit *telAzLineEdit;
    QWidget *serverTab;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *locationLabel;
    QLineEdit *hostLineEdit;
    QLabel *portLabel;
    QLineEdit *portLineEdit;
    QHBoxLayout *horizontalLayout;
    QPushButton *disconnectButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *connectButton;
    QTextBrowser *view;
    QLabel *infoLabel;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_6;
    QLabel *statusLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TelescopeCli)
    {
        if (TelescopeCli->objectName().isEmpty())
            TelescopeCli->setObjectName(QString::fromUtf8("TelescopeCli"));
        TelescopeCli->resize(536, 520);
        centralWidget = new QWidget(TelescopeCli);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_4 = new QVBoxLayout(centralWidget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setMargin(11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        tab = new QTabWidget(centralWidget);
        tab->setObjectName(QString::fromUtf8("tab"));
        telescopeTab = new QWidget();
        telescopeTab->setObjectName(QString::fromUtf8("telescopeTab"));
        verticalLayout_2 = new QVBoxLayout(telescopeTab);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setMargin(11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        siteBox = new QGroupBox(telescopeTab);
        siteBox->setObjectName(QString::fromUtf8("siteBox"));
        gridLayout_3 = new QGridLayout(siteBox);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setMargin(11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_10 = new QLabel(siteBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_10, 0, 0, 1, 1);

        label_11 = new QLabel(siteBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_11, 0, 1, 1, 1);

        label_13 = new QLabel(siteBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_13, 0, 2, 1, 1);

        label_12 = new QLabel(siteBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_12, 0, 3, 1, 1);

        UT_LineEdit = new QLineEdit(siteBox);
        UT_LineEdit->setObjectName(QString::fromUtf8("UT_LineEdit"));
        UT_LineEdit->setAlignment(Qt::AlignCenter);
        UT_LineEdit->setReadOnly(true);

        gridLayout_3->addWidget(UT_LineEdit, 1, 0, 1, 1);

        LT_LineEdit = new QLineEdit(siteBox);
        LT_LineEdit->setObjectName(QString::fromUtf8("LT_LineEdit"));
        LT_LineEdit->setAlignment(Qt::AlignCenter);
        LT_LineEdit->setReadOnly(true);

        gridLayout_3->addWidget(LT_LineEdit, 1, 1, 1, 1);

        JD_LineEdit = new QLineEdit(siteBox);
        JD_LineEdit->setObjectName(QString::fromUtf8("JD_LineEdit"));
        JD_LineEdit->setAlignment(Qt::AlignCenter);
        JD_LineEdit->setReadOnly(true);

        gridLayout_3->addWidget(JD_LineEdit, 1, 2, 1, 1);

        LST_LineEdit = new QLineEdit(siteBox);
        LST_LineEdit->setObjectName(QString::fromUtf8("LST_LineEdit"));
        LST_LineEdit->setAlignment(Qt::AlignCenter);
        LST_LineEdit->setReadOnly(true);

        gridLayout_3->addWidget(LST_LineEdit, 1, 3, 1, 1);


        verticalLayout_2->addWidget(siteBox);

        telescopeBox = new QGroupBox(telescopeTab);
        telescopeBox->setObjectName(QString::fromUtf8("telescopeBox"));
        gridLayout_4 = new QGridLayout(telescopeBox);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setMargin(11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(70, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 0, 0, 1, 1);

        label_3 = new QLabel(telescopeBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_3, 0, 1, 1, 1);

        label_4 = new QLabel(telescopeBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_4, 0, 2, 1, 1);

        label_5 = new QLabel(telescopeBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(label_5, 0, 3, 1, 1);

        label_6 = new QLabel(telescopeBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_4->addWidget(label_6, 1, 0, 1, 1);

        telHALineEdit = new QLineEdit(telescopeBox);
        telHALineEdit->setObjectName(QString::fromUtf8("telHALineEdit"));
        telHALineEdit->setAlignment(Qt::AlignCenter);
        telHALineEdit->setReadOnly(true);

        gridLayout_4->addWidget(telHALineEdit, 1, 1, 1, 1);

        telRALineEdit = new QLineEdit(telescopeBox);
        telRALineEdit->setObjectName(QString::fromUtf8("telRALineEdit"));
        telRALineEdit->setAlignment(Qt::AlignCenter);
        telRALineEdit->setReadOnly(true);

        gridLayout_4->addWidget(telRALineEdit, 1, 2, 1, 1);

        telDecLineEdit = new QLineEdit(telescopeBox);
        telDecLineEdit->setObjectName(QString::fromUtf8("telDecLineEdit"));
        telDecLineEdit->setAlignment(Qt::AlignCenter);
        telDecLineEdit->setReadOnly(true);

        gridLayout_4->addWidget(telDecLineEdit, 1, 3, 1, 1);

        label_7 = new QLabel(telescopeBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_4->addWidget(label_7, 2, 0, 1, 1);

        trgHALineEdit = new QLineEdit(telescopeBox);
        trgHALineEdit->setObjectName(QString::fromUtf8("trgHALineEdit"));
        trgHALineEdit->setAlignment(Qt::AlignCenter);
        trgHALineEdit->setReadOnly(true);

        gridLayout_4->addWidget(trgHALineEdit, 2, 1, 1, 1);

        trgRALineEdit = new QLineEdit(telescopeBox);
        trgRALineEdit->setObjectName(QString::fromUtf8("trgRALineEdit"));
        trgRALineEdit->setAlignment(Qt::AlignCenter);
        trgRALineEdit->setReadOnly(true);

        gridLayout_4->addWidget(trgRALineEdit, 2, 2, 1, 1);

        trgDecLineEdit = new QLineEdit(telescopeBox);
        trgDecLineEdit->setObjectName(QString::fromUtf8("trgDecLineEdit"));
        trgDecLineEdit->setAlignment(Qt::AlignCenter);
        trgDecLineEdit->setReadOnly(true);

        gridLayout_4->addWidget(trgDecLineEdit, 2, 3, 1, 1);

        label_15 = new QLabel(telescopeBox);
        label_15->setObjectName(QString::fromUtf8("label_15"));

        gridLayout_4->addWidget(label_15, 3, 0, 1, 1);

        difHALineEdit = new QLineEdit(telescopeBox);
        difHALineEdit->setObjectName(QString::fromUtf8("difHALineEdit"));
        difHALineEdit->setAlignment(Qt::AlignCenter);
        difHALineEdit->setReadOnly(true);

        gridLayout_4->addWidget(difHALineEdit, 3, 1, 1, 1);

        difRALineEdit = new QLineEdit(telescopeBox);
        difRALineEdit->setObjectName(QString::fromUtf8("difRALineEdit"));
        difRALineEdit->setAlignment(Qt::AlignCenter);
        difRALineEdit->setReadOnly(true);

        gridLayout_4->addWidget(difRALineEdit, 3, 2, 1, 1);

        difDecLineEdit = new QLineEdit(telescopeBox);
        difDecLineEdit->setObjectName(QString::fromUtf8("difDecLineEdit"));
        difDecLineEdit->setAlignment(Qt::AlignCenter);
        difDecLineEdit->setReadOnly(true);

        gridLayout_4->addWidget(difDecLineEdit, 3, 3, 1, 1);


        verticalLayout_2->addWidget(telescopeBox);

        groupBox = new QGroupBox(telescopeTab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setMargin(11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSpacer_3 = new QSpacerItem(125, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(label_2, 0, 2, 1, 1);

        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 1, 0, 1, 1);

        telAltLineEdit = new QLineEdit(groupBox);
        telAltLineEdit->setObjectName(QString::fromUtf8("telAltLineEdit"));

        gridLayout_2->addWidget(telAltLineEdit, 1, 1, 1, 1);

        telAzLineEdit = new QLineEdit(groupBox);
        telAzLineEdit->setObjectName(QString::fromUtf8("telAzLineEdit"));

        gridLayout_2->addWidget(telAzLineEdit, 1, 2, 1, 1);


        verticalLayout_2->addWidget(groupBox);

        tab->addTab(telescopeTab, QString());
        serverTab = new QWidget();
        serverTab->setObjectName(QString::fromUtf8("serverTab"));
        verticalLayout = new QVBoxLayout(serverTab);
        verticalLayout->setSpacing(6);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        locationLabel = new QLabel(serverTab);
        locationLabel->setObjectName(QString::fromUtf8("locationLabel"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(locationLabel->sizePolicy().hasHeightForWidth());
        locationLabel->setSizePolicy(sizePolicy);

        gridLayout->addWidget(locationLabel, 0, 0, 1, 1);

        hostLineEdit = new QLineEdit(serverTab);
        hostLineEdit->setObjectName(QString::fromUtf8("hostLineEdit"));

        gridLayout->addWidget(hostLineEdit, 0, 1, 1, 1);

        portLabel = new QLabel(serverTab);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(portLabel->sizePolicy().hasHeightForWidth());
        portLabel->setSizePolicy(sizePolicy1);
        portLabel->setMinimumSize(QSize(15, 0));

        gridLayout->addWidget(portLabel, 1, 0, 1, 1);

        portLineEdit = new QLineEdit(serverTab);
        portLineEdit->setObjectName(QString::fromUtf8("portLineEdit"));

        gridLayout->addWidget(portLineEdit, 1, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        disconnectButton = new QPushButton(serverTab);
        disconnectButton->setObjectName(QString::fromUtf8("disconnectButton"));

        horizontalLayout->addWidget(disconnectButton);

        horizontalSpacer = new QSpacerItem(158, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        connectButton = new QPushButton(serverTab);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));

        horizontalLayout->addWidget(connectButton);


        verticalLayout->addLayout(horizontalLayout);

        view = new QTextBrowser(serverTab);
        view->setObjectName(QString::fromUtf8("view"));

        verticalLayout->addWidget(view);

        infoLabel = new QLabel(serverTab);
        infoLabel->setObjectName(QString::fromUtf8("infoLabel"));
        infoLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(infoLabel);

        tab->addTab(serverTab, QString());

        verticalLayout_4->addWidget(tab);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_6);

        statusLabel = new QLabel(centralWidget);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));

        horizontalLayout_2->addWidget(statusLabel);


        verticalLayout_4->addLayout(horizontalLayout_2);

        TelescopeCli->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TelescopeCli);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 536, 22));
        TelescopeCli->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TelescopeCli);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        TelescopeCli->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TelescopeCli);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        TelescopeCli->setStatusBar(statusBar);
#ifndef QT_NO_SHORTCUT
        label_10->setBuddy(UT_LineEdit);
        label_11->setBuddy(LT_LineEdit);
        label_13->setBuddy(JD_LineEdit);
        label_12->setBuddy(LST_LineEdit);
        locationLabel->setBuddy(hostLineEdit);
        portLabel->setBuddy(portLineEdit);
#endif // QT_NO_SHORTCUT

        retranslateUi(TelescopeCli);

        tab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TelescopeCli);
    } // setupUi

    void retranslateUi(QMainWindow *TelescopeCli)
    {
        TelescopeCli->setWindowTitle(QApplication::translate("TelescopeCli", "TelescopeCli", 0, QApplication::UnicodeUTF8));
        siteBox->setTitle(QApplication::translate("TelescopeCli", "Time", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("TelescopeCli", "UT", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("TelescopeCli", "LT", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("TelescopeCli", "JDate", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("TelescopeCli", "LST", 0, QApplication::UnicodeUTF8));
        telescopeBox->setTitle(QApplication::translate("TelescopeCli", "Position", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("TelescopeCli", "HA", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("TelescopeCli", "RA", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("TelescopeCli", "Dec", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("TelescopeCli", "Telescope", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("TelescopeCli", "Target", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("TelescopeCli", "Difference", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("TelescopeCli", "GroupBox", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TelescopeCli", "Alt", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("TelescopeCli", "Az", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("TelescopeCli", "Telescope", 0, QApplication::UnicodeUTF8));
        tab->setTabText(tab->indexOf(telescopeTab), QApplication::translate("TelescopeCli", "Telecope", 0, QApplication::UnicodeUTF8));
        locationLabel->setText(QApplication::translate("TelescopeCli", "URL:", 0, QApplication::UnicodeUTF8));
        hostLineEdit->setText(QApplication::translate("TelescopeCli", "192.168.0.10", 0, QApplication::UnicodeUTF8));
        portLabel->setText(QApplication::translate("TelescopeCli", "Port:", 0, QApplication::UnicodeUTF8));
        portLineEdit->setText(QApplication::translate("TelescopeCli", "4000", 0, QApplication::UnicodeUTF8));
        disconnectButton->setText(QApplication::translate("TelescopeCli", "Disconnect", 0, QApplication::UnicodeUTF8));
        connectButton->setText(QApplication::translate("TelescopeCli", "Connect", 0, QApplication::UnicodeUTF8));
        infoLabel->setText(QApplication::translate("TelescopeCli", "Information", 0, QApplication::UnicodeUTF8));
        tab->setTabText(tab->indexOf(serverTab), QApplication::translate("TelescopeCli", "Server", 0, QApplication::UnicodeUTF8));
        statusLabel->setText(QApplication::translate("TelescopeCli", "Status", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(TelescopeCli);
    } // retranslateUi

};

namespace Ui {
    class TelescopeCli: public Ui_TelescopeCli {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TELESCOPECLI_H
