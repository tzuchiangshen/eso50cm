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

};

#endif // AUTOGUIDERPANEL_H
