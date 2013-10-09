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
		void setPinholePosition(int x, int y);
		void setPinholeRadius(int r);
		void setOffsetCorrectionThreshold(int x);
		void setOffsetCorrectionDisableThreshold(int x);
		int getOffsetCorrectionThreshold();
		int getOffsetCorrectionDisableThreshold();
		void setFramePerSeconds(int frame);
		void setEnableCorrection(bool enable);
		void setVideoInput(int source);
		void setExposureTime(int time);


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
		int offsetCorrectionThreshold;
		int offsetCorrectionDisableThreshold;
		int framePerSeconds;
		int pinholeRadius;
		bool enableCorrection;
		int threshold;
		bool enableAutoThreshold;
		int exposureTime;

	signals:
		void updateStatisticsInGUI(double *x, double *y);
        void newFrame(const QImage &frame);
		void newIntensityProfile(const QImage &frame);
		void newCorrection(int x, int y);
		void newThreshold(int val);

	public slots:
		void connectToCamera();	
		void connectToObsControl();
		void slewOn(string dir);
		void slewOn(string speed, string dir);
		void slewOnNorth();
		void slewOff();
		void slewOff(string dir);
		void slewLoop();
		void setupVideoInput();
		void sendCorrection(int x, int y);
		void setThreshold(int val);
		void setAutoThreshold(bool status);

};

