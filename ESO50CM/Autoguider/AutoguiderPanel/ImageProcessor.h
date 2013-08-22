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
#include <MatToQImage.h>

using namespace cv;

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
		void setupVideoSource(string file);
		void setupVideoSource(int cameraId);
		void openVideoSource();
		bool processFrame();
		

	private:
		long count;
		int cameraId;
		string fileName;
		double nframes;
		double fps;
        int cols;
		int rows;
		int enc;
		Point pinhole;
		VideoCapture cap;

	signals:
		void updateStatisticsInGUI(double *x, double *y);
        void newFrame(const QImage &frame);
		void newIntensityProfile(const QImage &frame);
		void newCorrection(int x, int y);
	public slots:
		void connectToCamera();	
		void slewOn();
		void slewOff();
		void slewLoop();
		void test();
		//void sendCorrection(int x, int y);
};

