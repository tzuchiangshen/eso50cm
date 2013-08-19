#pragma once

#include <string>
#include <iostream>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// Qt
#include <QtCore/QTime>
#include <QtCore/QThread>
#include <QDebug>

#include <ObsControlIF.h>

class ImageProcessor : public QThread
{
	Q_OBJECT
	public:
		ImageProcessor(void);
		~ImageProcessor(void);
		const char * sayHello(char *name);
		int ImageProcessor::process_main(char*filename);
		ObsControlIF *obs;
	protected:
		void run();

	private:
		long count;
	signals:
		void updateStatisticsInGUI(double *x, double *y);
	public slots:
		void connectToCamera();	
		void slewOn();
		void slewOff();
		void slewLoop();
};

