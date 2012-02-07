/********************************************************************************
** Form generated from reading UI file 'hscli.ui'
**
** Created: Wed Jan 26 00:02:05 2011
**      by: Qt User Interface Compiler version 4.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HSCLI_H
#define UI_HSCLI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
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

class Ui_hsCli
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_4;
    QTabWidget *tab;
    QWidget *telescopeTab;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QComboBox *speedComboBox;
    QHBoxLayout *horizontalLayout_6;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *nwPW;
    QPushButton *northPB;
    QPushButton *nePB;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *westPB;
    QPushButton *stopPB;
    QPushButton *eastPB;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *swPB;
    QPushButton *southPB;
    QPushButton *sePB;
    QSpacerItem *horizontalSpacer_12;
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
    QTextBrowser *telescopeView;
    QLabel *infoLabel;
    QWidget *tab_2;
    QVBoxLayout *verticalLayout_5;
    QGridLayout *gridLayout_3;
    QLabel *deviceLabel;
    QLineEdit *deviceLineEdit;
    QLabel *baudsLabel;
    QLineEdit *baudsLineEdit;
    QHBoxLayout *horizontalLayout_7;
    QPushButton *deviceDisconnectPB;
    QSpacerItem *horizontalSpacer_13;
    QPushButton *deviceConnectPB;
    QTextBrowser *deviceView;
    QLabel *deviceInfoLabel;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_6;
    QLabel *statusLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *hsCli)
    {
        if (hsCli->objectName().isEmpty())
            hsCli->setObjectName(QString::fromUtf8("hsCli"));
        hsCli->resize(448, 495);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(hsCli->sizePolicy().hasHeightForWidth());
        hsCli->setSizePolicy(sizePolicy);
        centralWidget = new QWidget(hsCli);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout_4 = new QVBoxLayout(centralWidget);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        tab = new QTabWidget(centralWidget);
        tab->setObjectName(QString::fromUtf8("tab"));
        telescopeTab = new QWidget();
        telescopeTab->setObjectName(QString::fromUtf8("telescopeTab"));
        verticalLayout_3 = new QVBoxLayout(telescopeTab);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        groupBox_2 = new QGroupBox(telescopeTab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        speedComboBox = new QComboBox(groupBox_2);
        speedComboBox->setObjectName(QString::fromUtf8("speedComboBox"));

        gridLayout_2->addWidget(speedComboBox, 0, 0, 1, 1);


        verticalLayout_3->addWidget(groupBox_2);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        groupBox = new QGroupBox(telescopeTab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        nwPW = new QPushButton(groupBox);
        nwPW->setObjectName(QString::fromUtf8("nwPW"));
        nwPW->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(50);
        sizePolicy1.setVerticalStretch(50);
        sizePolicy1.setHeightForWidth(nwPW->sizePolicy().hasHeightForWidth());
        nwPW->setSizePolicy(sizePolicy1);
        nwPW->setMinimumSize(QSize(50, 50));
        nwPW->setMaximumSize(QSize(50, 50));

        horizontalLayout_5->addWidget(nwPW);

        northPB = new QPushButton(groupBox);
        northPB->setObjectName(QString::fromUtf8("northPB"));
        sizePolicy1.setHeightForWidth(northPB->sizePolicy().hasHeightForWidth());
        northPB->setSizePolicy(sizePolicy1);
        northPB->setMinimumSize(QSize(50, 50));
        northPB->setMaximumSize(QSize(50, 50));

        horizontalLayout_5->addWidget(northPB);

        nePB = new QPushButton(groupBox);
        nePB->setObjectName(QString::fromUtf8("nePB"));
        nePB->setEnabled(false);
        sizePolicy1.setHeightForWidth(nePB->sizePolicy().hasHeightForWidth());
        nePB->setSizePolicy(sizePolicy1);
        nePB->setMinimumSize(QSize(50, 50));
        nePB->setMaximumSize(QSize(50, 50));

        horizontalLayout_5->addWidget(nePB);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        westPB = new QPushButton(groupBox);
        westPB->setObjectName(QString::fromUtf8("westPB"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(50);
        sizePolicy2.setVerticalStretch(50);
        sizePolicy2.setHeightForWidth(westPB->sizePolicy().hasHeightForWidth());
        westPB->setSizePolicy(sizePolicy2);
        westPB->setMinimumSize(QSize(50, 50));
        westPB->setMaximumSize(QSize(50, 50));

        horizontalLayout_4->addWidget(westPB);

        stopPB = new QPushButton(groupBox);
        stopPB->setObjectName(QString::fromUtf8("stopPB"));
        sizePolicy1.setHeightForWidth(stopPB->sizePolicy().hasHeightForWidth());
        stopPB->setSizePolicy(sizePolicy1);
        stopPB->setMinimumSize(QSize(50, 50));
        stopPB->setMaximumSize(QSize(50, 50));

        horizontalLayout_4->addWidget(stopPB);

        eastPB = new QPushButton(groupBox);
        eastPB->setObjectName(QString::fromUtf8("eastPB"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(37);
        sizePolicy3.setVerticalStretch(30);
        sizePolicy3.setHeightForWidth(eastPB->sizePolicy().hasHeightForWidth());
        eastPB->setSizePolicy(sizePolicy3);
        eastPB->setMinimumSize(QSize(50, 50));
        eastPB->setMaximumSize(QSize(50, 50));

        horizontalLayout_4->addWidget(eastPB);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        swPB = new QPushButton(groupBox);
        swPB->setObjectName(QString::fromUtf8("swPB"));
        swPB->setEnabled(false);
        sizePolicy1.setHeightForWidth(swPB->sizePolicy().hasHeightForWidth());
        swPB->setSizePolicy(sizePolicy1);
        swPB->setMinimumSize(QSize(50, 50));
        swPB->setMaximumSize(QSize(50, 50));

        horizontalLayout_3->addWidget(swPB);

        southPB = new QPushButton(groupBox);
        southPB->setObjectName(QString::fromUtf8("southPB"));
        sizePolicy1.setHeightForWidth(southPB->sizePolicy().hasHeightForWidth());
        southPB->setSizePolicy(sizePolicy1);
        southPB->setMinimumSize(QSize(50, 50));
        southPB->setMaximumSize(QSize(50, 50));

        horizontalLayout_3->addWidget(southPB);

        sePB = new QPushButton(groupBox);
        sePB->setObjectName(QString::fromUtf8("sePB"));
        sePB->setEnabled(false);
        sizePolicy1.setHeightForWidth(sePB->sizePolicy().hasHeightForWidth());
        sePB->setSizePolicy(sizePolicy1);
        sePB->setMinimumSize(QSize(50, 50));
        sePB->setMaximumSize(QSize(50, 50));

        horizontalLayout_3->addWidget(sePB);


        verticalLayout_2->addLayout(horizontalLayout_3);


        horizontalLayout_6->addWidget(groupBox);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_12);


        verticalLayout_3->addLayout(horizontalLayout_6);

        tab->addTab(telescopeTab, QString());
        serverTab = new QWidget();
        serverTab->setObjectName(QString::fromUtf8("serverTab"));
        verticalLayout = new QVBoxLayout(serverTab);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        locationLabel = new QLabel(serverTab);
        locationLabel->setObjectName(QString::fromUtf8("locationLabel"));
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(locationLabel->sizePolicy().hasHeightForWidth());
        locationLabel->setSizePolicy(sizePolicy4);

        gridLayout->addWidget(locationLabel, 0, 0, 1, 1);

        hostLineEdit = new QLineEdit(serverTab);
        hostLineEdit->setObjectName(QString::fromUtf8("hostLineEdit"));

        gridLayout->addWidget(hostLineEdit, 0, 1, 1, 1);

        portLabel = new QLabel(serverTab);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(portLabel->sizePolicy().hasHeightForWidth());
        portLabel->setSizePolicy(sizePolicy5);
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
        disconnectButton->setEnabled(false);

        horizontalLayout->addWidget(disconnectButton);

        horizontalSpacer = new QSpacerItem(158, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        connectButton = new QPushButton(serverTab);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        connectButton->setEnabled(false);

        horizontalLayout->addWidget(connectButton);


        verticalLayout->addLayout(horizontalLayout);

        telescopeView = new QTextBrowser(serverTab);
        telescopeView->setObjectName(QString::fromUtf8("telescopeView"));

        verticalLayout->addWidget(telescopeView);

        infoLabel = new QLabel(serverTab);
        infoLabel->setObjectName(QString::fromUtf8("infoLabel"));
        infoLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(infoLabel);

        tab->addTab(serverTab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        verticalLayout_5 = new QVBoxLayout(tab_2);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        deviceLabel = new QLabel(tab_2);
        deviceLabel->setObjectName(QString::fromUtf8("deviceLabel"));
        sizePolicy4.setHeightForWidth(deviceLabel->sizePolicy().hasHeightForWidth());
        deviceLabel->setSizePolicy(sizePolicy4);

        gridLayout_3->addWidget(deviceLabel, 0, 0, 1, 1);

        deviceLineEdit = new QLineEdit(tab_2);
        deviceLineEdit->setObjectName(QString::fromUtf8("deviceLineEdit"));

        gridLayout_3->addWidget(deviceLineEdit, 0, 1, 1, 1);

        baudsLabel = new QLabel(tab_2);
        baudsLabel->setObjectName(QString::fromUtf8("baudsLabel"));
        sizePolicy5.setHeightForWidth(baudsLabel->sizePolicy().hasHeightForWidth());
        baudsLabel->setSizePolicy(sizePolicy5);
        baudsLabel->setMinimumSize(QSize(15, 0));

        gridLayout_3->addWidget(baudsLabel, 1, 0, 1, 1);

        baudsLineEdit = new QLineEdit(tab_2);
        baudsLineEdit->setObjectName(QString::fromUtf8("baudsLineEdit"));

        gridLayout_3->addWidget(baudsLineEdit, 1, 1, 1, 1);


        verticalLayout_5->addLayout(gridLayout_3);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        deviceDisconnectPB = new QPushButton(tab_2);
        deviceDisconnectPB->setObjectName(QString::fromUtf8("deviceDisconnectPB"));
        deviceDisconnectPB->setEnabled(false);

        horizontalLayout_7->addWidget(deviceDisconnectPB);

        horizontalSpacer_13 = new QSpacerItem(158, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_13);

        deviceConnectPB = new QPushButton(tab_2);
        deviceConnectPB->setObjectName(QString::fromUtf8("deviceConnectPB"));
        deviceConnectPB->setEnabled(true);

        horizontalLayout_7->addWidget(deviceConnectPB);


        verticalLayout_5->addLayout(horizontalLayout_7);

        deviceView = new QTextBrowser(tab_2);
        deviceView->setObjectName(QString::fromUtf8("deviceView"));

        verticalLayout_5->addWidget(deviceView);

        deviceInfoLabel = new QLabel(tab_2);
        deviceInfoLabel->setObjectName(QString::fromUtf8("deviceInfoLabel"));
        deviceInfoLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(deviceInfoLabel);

        tab->addTab(tab_2, QString());

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

        hsCli->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(hsCli);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 448, 22));
        hsCli->setMenuBar(menuBar);
        mainToolBar = new QToolBar(hsCli);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        hsCli->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(hsCli);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        hsCli->setStatusBar(statusBar);
#ifndef QT_NO_SHORTCUT
        locationLabel->setBuddy(hostLineEdit);
        portLabel->setBuddy(portLineEdit);
        deviceLabel->setBuddy(hostLineEdit);
        baudsLabel->setBuddy(portLineEdit);
#endif // QT_NO_SHORTCUT

        retranslateUi(hsCli);

        tab->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(hsCli);
    } // setupUi

    void retranslateUi(QMainWindow *hsCli)
    {
        hsCli->setWindowTitle(QApplication::translate("hsCli", "TelescopeCli", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("hsCli", "Speed", 0, QApplication::UnicodeUTF8));
        speedComboBox->clear();
        speedComboBox->insertItems(0, QStringList()
         << QApplication::translate("hsCli", "set", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("hsCli", "Guide", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("hsCli", "Offset", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("hsCli", "stop", 0, QApplication::UnicodeUTF8)
        );
        groupBox->setTitle(QApplication::translate("hsCli", "Direction", 0, QApplication::UnicodeUTF8));
        nwPW->setText(QString());
        northPB->setText(QApplication::translate("hsCli", "N", 0, QApplication::UnicodeUTF8));
        nePB->setText(QString());
        westPB->setText(QApplication::translate("hsCli", "W", 0, QApplication::UnicodeUTF8));
        stopPB->setText(QApplication::translate("hsCli", "X", 0, QApplication::UnicodeUTF8));
        eastPB->setText(QApplication::translate("hsCli", "E", 0, QApplication::UnicodeUTF8));
        swPB->setText(QString());
        southPB->setText(QApplication::translate("hsCli", "S", 0, QApplication::UnicodeUTF8));
        sePB->setText(QString());
        tab->setTabText(tab->indexOf(telescopeTab), QApplication::translate("hsCli", "Pad", 0, QApplication::UnicodeUTF8));
        locationLabel->setText(QApplication::translate("hsCli", "URL:", 0, QApplication::UnicodeUTF8));
        hostLineEdit->setText(QApplication::translate("hsCli", "192.168.0.106", 0, QApplication::UnicodeUTF8));
        portLabel->setText(QApplication::translate("hsCli", "Port:", 0, QApplication::UnicodeUTF8));
        portLineEdit->setText(QApplication::translate("hsCli", "4000", 0, QApplication::UnicodeUTF8));
        disconnectButton->setText(QApplication::translate("hsCli", "Disconnect", 0, QApplication::UnicodeUTF8));
        connectButton->setText(QApplication::translate("hsCli", "Connect", 0, QApplication::UnicodeUTF8));
        infoLabel->setText(QApplication::translate("hsCli", "Information", 0, QApplication::UnicodeUTF8));
        tab->setTabText(tab->indexOf(serverTab), QApplication::translate("hsCli", "Telescope", 0, QApplication::UnicodeUTF8));
        deviceLabel->setText(QApplication::translate("hsCli", "Device", 0, QApplication::UnicodeUTF8));
        deviceLineEdit->setText(QApplication::translate("hsCli", "/dev/ttyUSB0", 0, QApplication::UnicodeUTF8));
        baudsLabel->setText(QApplication::translate("hsCli", "BAUDS", 0, QApplication::UnicodeUTF8));
        baudsLineEdit->setText(QApplication::translate("hsCli", "57600", 0, QApplication::UnicodeUTF8));
        deviceDisconnectPB->setText(QApplication::translate("hsCli", "Disconnect", 0, QApplication::UnicodeUTF8));
        deviceConnectPB->setText(QApplication::translate("hsCli", "Connect", 0, QApplication::UnicodeUTF8));
        deviceInfoLabel->setText(QApplication::translate("hsCli", "Information", 0, QApplication::UnicodeUTF8));
        tab->setTabText(tab->indexOf(tab_2), QApplication::translate("hsCli", "HS Device", 0, QApplication::UnicodeUTF8));
        statusLabel->setText(QApplication::translate("hsCli", "Status", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class hsCli: public Ui_hsCli {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HSCLI_H
