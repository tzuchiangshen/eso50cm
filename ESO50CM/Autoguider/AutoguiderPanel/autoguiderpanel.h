#ifndef AUTOGUIDERPANEL_H
#define AUTOGUIDERPANEL_H

#include <QtWidgets/QMainWindow>
#include <QSettings>
#include <QApplication>

#include "ui_autoguiderpanel.h"
#include "ImageProcessor.h"

class AutoguiderPanel : public QMainWindow
{
	Q_OBJECT

	public:
		AutoguiderPanel(QWidget *parent = 0);
		~AutoguiderPanel();
		
	private:
		Ui::AutoguiderPanelClass ui;
		ImageProcessor *proc;
		QString m_confFile;
		bool m_scale;
		void loadConfiguration();

		
	public slots:
		void startProcessing();
		void refreshImage(QImage img);
		void refreshIntensityProfile(QImage img);
		void setup();
		void slewEnable();
		void slewDisable();
		void updateCorrection(int x, int y);
		void updateVideoInput();
		void updatePinhole();
		void updatePinholeRadius();
		void updateOffsetCorrectionThreshold();
		void updateOffsetCorrectionDisableThreshold();
		void updateSamplingFramePerSeconds();
		void updateCorrectionFramePerSeconds();
		void scaleImage(bool);
		void updateThreshold(int val);
		void updateThresholdDuringAuto(int val);
		void enableAutoThreshold(bool);
		void updateExposureTime();
		

};

#endif // AUTOGUIDERPANEL_H
