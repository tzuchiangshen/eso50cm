#include "autoguiderpanel.h"

AutoguiderPanel::AutoguiderPanel(QWidget *parent)
	: QMainWindow(parent)
{
	int camera_id = 1;
	ui.setupUi(this);

	//configuration file
	qDebug() << "path=" << QApplication::applicationDirPath();
	m_confFile = QApplication::applicationDirPath() + "/autoguider.ini";
	proc = new ImageProcessor();
	loadConfiguration();
	
	//proc->connectToCamera();
	//connect(ui.pushButton, SIGNAL(released()), this, SLOT(startProcessing()));
	connect(ui.btConnect, SIGNAL(released()), proc, SLOT(connectToCamera()));
	connect(ui.btSlewOn, SIGNAL(released()), this, SLOT(slewEnable()));
	connect(ui.btSlewOff, SIGNAL(released()), this, SLOT(slewDisable()));
	//connect(ui.btLoop, SIGNAL(released()), proc, SLOT(slewLoop()));
	connect(ui.btProcessFrame, SIGNAL(released()), this, SLOT(setup()));
	connect(proc,SIGNAL(newFrame(QImage)), this, SLOT(refreshImage(QImage)));
	connect(proc,SIGNAL(newIntensityProfile(QImage)), this, SLOT(refreshIntensityProfile(QImage)));
	connect(proc,SIGNAL(newCorrection(int, int)), this, SLOT(updateCorrection(int, int)));

	//configuration panels
	connect(ui.btVideoInputApply, SIGNAL(released()), this, SLOT(updateVideoInput()));
	connect(ui.btPinholeApply, SIGNAL(released()), this, SLOT(updatePinhole()));
	connect(ui.btPinholeRadiusApply, SIGNAL(released()), this, SLOT(updatePinholeRadius()));
	connect(ui.btOffsetCorrectionThresholdApply, SIGNAL(released()), this, SLOT(updateOffsetCorrectionThreshold()));
	connect(ui.btFramePerSecondsApply, SIGNAL(released()), this, SLOT(updateFramePerSeconds()));
}

AutoguiderPanel::~AutoguiderPanel()
{

}

void AutoguiderPanel::loadConfiguration() {
	QString _sValue;
	int _iValue; 
	int pinholeX;
	int pinholeY;

	QSettings settings(m_confFile, QSettings::IniFormat);
	settings.beginGroup("Main");

	//video input
	_iValue = settings.value("video_input", "").toInt();
	proc->setVideoInput(_iValue);
	_sValue = QString::number(_iValue);
	ui.txtVideoInput->setText(_sValue);
	//Pinhole X
	_iValue = settings.value("pinhole.x", "").toInt();
	pinholeX = _iValue;

	_sValue = QString::number(_iValue);
	ui.txtPinholeX->setText(_sValue);
	//qDebug() << "pinhole.x = " << _value;

	//Pinhole Y
	_iValue = settings.value("pinhole.y", "").toInt();
	pinholeY = _iValue;
	
	_sValue = QString::number(_iValue);
	ui.txtPinholeY->setText(_sValue);
	//qDebug() << "pinhole.y = " << _value;
	proc->setPinholePosition(pinholeX, pinholeY);

	//pinhole radius
	_iValue = settings.value("pinhole.r", "").toInt();
	_sValue = QString::number(_iValue);
	proc->setPinholeRadius(_iValue);
	ui.txtPinholeRadius->setText(_sValue);

	//Offset threshold
	_iValue = settings.value("threshold", "").toInt();
	_sValue = QString::number(_iValue);
	proc->setOffsetCorrectionThreshold(_iValue);
	ui.txtOffsetCorrectionThreshold->setText(_sValue);

	//Frame Per Seconds
	_iValue = settings.value("frame_per_second", "").toInt();
	_sValue = QString::number(_iValue);
	proc->setFramePerSeconds(_iValue);
	ui.txtFramePerSeconds->setText(_sValue);


}

void AutoguiderPanel::updateVideoInput() {
	QString _sValue;

	_sValue = ui.txtVideoInput->text();
	proc->setVideoInput(_sValue.toInt());
}

void AutoguiderPanel::updatePinhole() {
	QString _sValue;
	int x;
	int y;

	_sValue = ui.txtPinholeX->text();
	x = _sValue.toInt();

	_sValue = ui.txtPinholeY->text();
	y = _sValue.toInt();

	proc->setPinholePosition(x,y);
}

void AutoguiderPanel::updatePinholeRadius() {
	QString _sValue;

	_sValue = ui.txtPinholeRadius->text();
	proc->setPinholeRadius(_sValue.toInt());
}

void AutoguiderPanel::updateOffsetCorrectionThreshold() {
	QString _sValue;

	_sValue = ui.txtOffsetCorrectionThreshold->text();
	proc->setOffsetCorrectionThreshold(_sValue.toInt());
}

void AutoguiderPanel::updateFramePerSeconds() {
	QString _sValue;
	int _frame;

	_sValue = ui.txtFramePerSeconds->text();
	_frame = _sValue.toInt();

	if(_frame < 1 || _frame > 60 ) {
		_frame = 10;
		qDebug() << " frame per seconds is out of limit (1<=frame<=60), use default: 10 ";
		ui.txtFramePerSeconds->setText("10");
	} 
	proc->setFramePerSeconds(_frame);
}



void AutoguiderPanel::startProcessing() {
	proc->start();
}

void AutoguiderPanel::refreshImage(QImage img) {
	ui.imgContainer->setPixmap(QPixmap::fromImage(img));
	//qDebug() << "main image refreshed.";
}

void AutoguiderPanel::refreshIntensityProfile(QImage img) {
	ui.lbPinholeProfileContainer->setPixmap(QPixmap::fromImage(img));
	//qDebug() << "intesity image refreshed.";
}

void AutoguiderPanel::setup() {
	proc->setupVideoInput();
	proc->start();
}

void AutoguiderPanel::updateCorrection(int x, int y) {
	char text[100];
	sprintf(text, "x=%d, y=%d", x, y);
	ui.lbCorrection->setText(text);

	int limit = proc->getOffsetCorrectionThreshold();
	//qDebug() << "!!!!!!!!!!offsetCorrectionThreshold = " << limit;
	int limitX = limit;
	int limitY = limit;
	string axisX;
	string axisY;
	int slewX = 0;
	int slewY = 0;
	char text2[100];

	if( x > limitX) {
        //move East
		axisX = "E";
		slewX = 1;
	} else if (x < -limitX) {
		//move West
		axisX = "W";
		slewX = 1;
	} else {
		axisX = "";
		slewX = 0;
	}

	if ( y > limitY) {
		//move N
        axisY = "S";
		slewY = 1;
	} else if ( y < -limitY) {
		//move South
		axisY = "N";
		slewY = 1;
	} else {
		axisY = "";
		slewY = 0;
	}

	if(slewX == 1 && slewY == 1) 
		sprintf(text2, "%s %s", axisY.c_str(), axisX.c_str());
	else if (slewX == 1 && slewY == 0) 
		sprintf(text2, "-,%s", axisX.c_str());
	else if (slewX == 0 && slewY == 1) 
		sprintf(text2, "%s,-", axisY.c_str());
	else 
		sprintf(text2, "%s", "-,-");

	ui.lbSlewStatus->setText(text2);

}

void AutoguiderPanel::slewEnable() {
	proc->setEnableCorrection(true);
}

void AutoguiderPanel::slewDisable() {
	proc->setEnableCorrection(false);
}
