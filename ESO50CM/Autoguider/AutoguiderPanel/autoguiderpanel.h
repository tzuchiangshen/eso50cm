#ifndef AUTOGUIDERPANEL_H
#define AUTOGUIDERPANEL_H

#include <QtWidgets/QMainWindow>
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
	public slots:
		void startProcessing();
		void refreshImage(QImage img);
		void refreshIntensityProfile(QImage img);
		void test();
		void updateCorrection(int x, int y);

};

#endif // AUTOGUIDERPANEL_H
