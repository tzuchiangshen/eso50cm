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

}

AutoguiderPanel::~AutoguiderPanel()
{

}

void AutoguiderPanel::startProcessing() {
	proc->start();
}

