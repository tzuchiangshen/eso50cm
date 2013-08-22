#include "autoguiderpanel.h"

AutoguiderPanel::AutoguiderPanel(QWidget *parent)
	: QMainWindow(parent)
{
	int camera_id = 1;
	ui.setupUi(this);
	proc = new ImageProcessor();
	//proc->connectToCamera();
	connect(ui.pushButton, SIGNAL(released()), this, SLOT(startProcessing()));
	connect(ui.pushButton_2, SIGNAL(released()), proc, SLOT(connectToCamera()));
	connect(ui.btSlewOn, SIGNAL(released()), proc, SLOT(slewOn()));
	connect(ui.btSlewOff, SIGNAL(released()), proc, SLOT(slewOff()));
	connect(ui.btLoop, SIGNAL(released()), proc, SLOT(slewLoop()));
	connect(ui.btTest, SIGNAL(released()), this, SLOT(test()));
	connect(proc,SIGNAL(newFrame(QImage)), this, SLOT(refreshImage(QImage)));
	connect(proc,SIGNAL(newIntensityProfile(QImage)), this, SLOT(refreshIntensityProfile(QImage)));
	connect(proc,SIGNAL(newCorrection(int, int)), this, SLOT(updateCorrection(int, int)));
}

AutoguiderPanel::~AutoguiderPanel()
{

}

void AutoguiderPanel::startProcessing() {
	proc->start();
}

void AutoguiderPanel::refreshImage(QImage img) {
	ui.imgContainer->setPixmap(QPixmap::fromImage(img));
	qDebug() << "main image refreshed.";
}

void AutoguiderPanel::refreshIntensityProfile(QImage img) {
	ui.lbPinholeProfileContainer->setPixmap(QPixmap::fromImage(img));
	qDebug() << "intesity image refreshed.";
}

void AutoguiderPanel::test() {
	proc->test();
	proc->start();
}

void AutoguiderPanel::updateCorrection(int x, int y) {
	char text[100];
	sprintf(text, "x=%d, y=%d", x, y);
	ui.lbCorrection->setText(text);
}