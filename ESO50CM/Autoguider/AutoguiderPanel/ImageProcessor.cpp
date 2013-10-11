#include "ImageProcessor.h"


using namespace std;
using namespace cv;

ImageProcessor::ImageProcessor(void)
{
	obs = new ObsControlIF(); 
	count = 0;
	threshold = 25;
	XStop = true;
	YStop = true;

	//buffer setup
	
	for(int i=0; i<60; i++) {
		XBuffer[i] = 0;
		YBuffer[i] = 0;
		cout << "XBuffer[" << i << "]=" << XBuffer[i] << endl;
	}
	processedFrameCounter = 0;
	enableAutoThreshold = true;

	connect(this,SIGNAL(newCorrection(int, int)), this, SLOT(sendCorrection(int, int)));
}


ImageProcessor::~ImageProcessor(void)
{
}

const char* ImageProcessor::sayHello(char* name)
{
	
	string result2; 

	result2.append("hellow world ");
	result2.append(name);
	std::cout << "dentro" << result2 << std::endl;

	const char *result = result2.c_str();
	return  result;
}

void ImageProcessor::setVideoInput(int source) {
	cameraId = source;
	qDebug() << "new value for video input: " << cameraId;
}

void ImageProcessor::setPinholePosition(int x, int y) {
	pinhole = Point(x, y);
	qDebug() << "new value for pinhole position: (" << pinhole.x << "," << pinhole.y << ")";
}

void ImageProcessor::setPinholeRadius(int r) {
	pinholeRadius = r;
	qDebug() << "new value for pinhole radius: " << r;
}


void ImageProcessor::setOffsetCorrectionThreshold(int x) {
	offsetCorrectionThreshold = x;
	qDebug() << "new value for offset correction threshold: " << x;
}

void ImageProcessor::setOffsetCorrectionDisableThreshold(int x) {
	offsetCorrectionDisableThreshold = x;
	qDebug() << "new value for offset correction disable threshold: " << x;
}

int ImageProcessor::getOffsetCorrectionThreshold() {
	return offsetCorrectionThreshold;
}

int ImageProcessor::getOffsetCorrectionDisableThreshold() {
	return offsetCorrectionDisableThreshold;
}

void ImageProcessor::setSamplingFramePerSeconds(int frame) {
	samplingFramePerSeconds = frame;
	qDebug() << "new value for sampling frame per seconds: " << frame;
}

void ImageProcessor::setCorrectionFramePerSeconds(int frame) {
	correctionFramePerSeconds = frame;
	qDebug() << "new value for correction frame per seconds: " << frame;
}

void ImageProcessor::setEnableCorrection(bool enable) {
	enableCorrection = enable;
	qDebug() << "new value for enable correction: " << enable;
}

void ImageProcessor::setExposureTime(int time) {
	exposureTime = time;
	cap.set(CV_CAP_PROP_EXPOSURE, time ); 
	qDebug() << "new value for exposure time: " << time;
	double exposure = cap.get(CV_CAP_PROP_EXPOSURE); 
	qDebug() << "read exposure time from camera--------------------> " << exposure;	
}

void ImageProcessor::connectToCamera() {
	qDebug() << "ObsControl: loading ...";
	qDebug() << "ObsControl: connecting ...";
	obs->connect();
	qDebug() << "Ready!";
}

void ImageProcessor::connectToObsControl() {
	qDebug() << "ObsControl: loading ...";
	qDebug() << "ObsControl: connecting ...";
	obs->connect();
	qDebug() << "Ready!";
}

void ImageProcessor::slewOn(string dir) {
	count++;
	qDebug() << "ObsControl: calling slewOn(" << dir.c_str() << ") (" << count << ") ...";
	//string dir("N");
    obs->handset_slew("set", dir);
	//qDebug() << "Ready!";
}

void ImageProcessor::slewOn(string speed, string dir) {
	count++;
	qDebug() << "ObsControl: calling slewOn(" << speed.c_str() << "," << dir.c_str() << ") (" << count << ") ...";
	//string dir("N");
    obs->handset_slew(speed, dir);
	//qDebug() << "Ready!";
}

void ImageProcessor::slewOnNorth() {
	slewOn(string("N"));
}

void ImageProcessor::slewOff() {
	qDebug() << "ObsControl: calling slewOff ...";
	string rate("stop");
    obs->handset_slew(rate, "S");
	qDebug() << "Ready!";
}

void ImageProcessor::slewOff(string dir) {
	qDebug() << "ObsControl: calling slewOff(" << dir.c_str() << ") ...";
	string rate("stop");
    obs->handset_slew(rate, dir);
	qDebug() << "Ready!";
}


void ImageProcessor::slewLoop() {
	qDebug() << "ObsControl: calling slewLoop ...";
    while(1) {
		slewOn(string("N"));
		QThread::sleep(1);
		slewOff();
		QThread::sleep(1);
	}
}

void ImageProcessor::setThreshold(int val) {
	threshold = val;
	cout << "proc: new threshold = " << threshold << endl;
}

void ImageProcessor::setAutoThreshold(bool status) {
	enableAutoThreshold = status;
	cout << "proc: new value for auto threshold = " << enableAutoThreshold << endl;
}

void ImageProcessor::setupVideoSource(int cameraId) {
	cameraId = cameraId;
}

void ImageProcessor::setupVideoSource(string file) {
	//abs path to the video file
	fileName = file;
}

void ImageProcessor::dumpBuffer(int *_array, const char *name) {

	cout << "Correction Buffer:" << name << endl;
	cout << "-----------------------------" << endl;
	for(int i=0; i<60; i++) {
		cout << "[" << i  << "]=(" << _array[i] << ") ";
	}
	
	cout << endl;
	cout << "=========================="  << endl;
}

int ImageProcessor::getMedian(int *_array) {
	int _tmp[BUFFER_SIZE];
	for(int i=0; i<BUFFER_SIZE; i++) {
		_tmp[i] = _array[i];
	}
	std::sort(std::begin(_tmp), std::end(_tmp));
	//dumpBuffer(_tmp, "sorted buffer");
	return _tmp[BUFFER_SIZE/2];
}

void ImageProcessor::openVideoSource() {
	// open video file 
	if(cameraId < 0 ) { 
		//simulation mode, use a video file
		cap = VideoCapture(fileName);
		qDebug() << "Opening video Input: " << fileName.c_str();

	} else {
		cap = VideoCapture(cameraId);
		qDebug() << "Opening video Input: " << cameraId;

		//cap.set(CV_CAP_PROP_EXPOSURE, 10 ); 
		double exposure = cap.get(CV_CAP_PROP_EXPOSURE); 
		qDebug() << "Current exposure time --------------------> " << exposure;
	}
    
    if(!cap.isOpened())  // check if we succeeded
        throw "Error when reading video file";
	
    //get number of frames
    nframes = cap.get(CV_CAP_PROP_FRAME_COUNT);
    fps = cap.get(CV_CAP_PROP_FPS);
    cols = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    rows = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    enc = cap.get(CV_CAP_PROP_FOURCC);
    qDebug() << "number of frames " << nframes << " frames per second: " <<fps << " size: " << cols << "," << rows << "enc: " << enc;
	
	//hard coded position of the pinhole in the image, should be moved to a configuration file. 
    //pinhole = Point(383,252);
	qDebug() << "pinhole position: (" << pinhole.x << "," << pinhole.y << ")";
}

bool ImageProcessor::processFrame() {
	Mat frame;
	Mat gray_image;
	Mat gray_image1;
	Mat fst;
	Mat mask;
	Mat object;
	float th;
    //video processing loop
	if (!cap.grab())
		return false;

	double exposure = cap.get(CV_CAP_PROP_EXPOSURE); 
	qDebug() << "Current exposure time --------------------> " << exposure;
    
	//cap >> frame; // get a new frame from video file
	cap.retrieve(frame);
	cvtColor( frame, gray_image, CV_BGR2GRAY );
	//qDebug() << " Covert image to gray level, channels=" << gray_image.channels() << " type=" << gray_image.type();
    gray_image1.convertTo(gray_image1, CV_32F);

	//median filter application, result to fst
    medianBlur ( gray_image, fst, 5 );

	//filtered image statistics: mean and standard deviation
    cv::Scalar cvMean;
    cv::Scalar cvStddev;
    meanStdDev(fst, cvMean, cvStddev);
    cout << " frame average: " << cvMean[0] << " std: " << cvStddev[0] << endl;
        
    //threshold for mask
	if(enableAutoThreshold) {
		th = cvMean[0] + 3.0*cvStddev[0];
	} else {
		th = threshold;
	}
	cout << "auto=" << enableAutoThreshold << " threshold = " << th << endl;

    //mask definition and calculation
    mask = fst > th;
    bitwise_and(fst, mask, object);

    //Centroid estimation
    Mat object32;
    object.convertTo(object32, CV_32F);
    //Draw the pinhole
    circle(object32, pinhole, pinholeRadius, 0);

	//meshgrid creation
    Mat XX,YY;
    XX.create(rows, cols, CV_32F);
    YY.create(rows, cols, CV_32F);
    for (int i = 0; i < rows; i++) {
        YY.at<float>(i,0) = (float)i;
    }
 
    for (int i = 0; i<cols;i++){
        XX.at<float>(0,i) =  (float)i;
    }

    XX = repeat(XX.row(0),rows,1);
	YY = repeat(YY.col(0),1,cols);
	
    Mat xst = object32.mul(XX);
    Mat yst = object32.mul(YY);

    Scalar gx = sum(xst);
    Scalar gy = sum(yst);
    Scalar sobj = sum(object32);
    gx = gx/sobj;
    gy = gy/sobj;
        
    int cx = (int)gx[0];
    int cy = (int)gy[0];
    
	cout<<" centroid: ("<<cx<<","<<cy<<")" << endl;
    Point center(cx, cy);
	int corrX = center.x - pinhole.x;
	int corrY = center.y - pinhole.y;
	cout << " X Correction: " << corrX << " Y Correction: " << corrY << endl;

	int R = 100;
	Rect roi(pinhole.x-R, pinhole.y-R, 2*R, 2*R);
	//cout << "roi.x=" << roi.x << " row.width=" << roi.width << endl;
	//cout << "roi.y=" << roi.x << " row.height=" << roi.width << endl;
	//cout << "img.cols=" << gray_image.cols << " img.rows=" << gray_image.rows << endl;

	Mat tmp;
	object32.convertTo(tmp, CV_8UC3);

	//draw the centroide 
	Scalar colorcircle;
	colorcircle[2] = 128;
	circle(tmp, center, 3, colorcircle);
	

	Mat finalFrame = Mat(tmp.clone(), roi);
	//zoom in
	Mat imzoom;
	pyrUp(finalFrame, imzoom, Size(finalFrame.cols*2, finalFrame.rows*2));
	QImage img = MatToQImage(imzoom);

	
	//pinhole intensity profile
	Mat chart1;
	Mat Zeross;
	Zeross = Mat::zeros(201, 400, CV_8UC3);
	chart1.create(201, 400, CV_8UC3);
	
	//profile in X 
	Rect roiX(pinhole.x - 40, pinhole.y, 80, 1);
	Mat chartROIx;
	CvScalar redLine = cvScalar(0,0,255);
	chartROIx = gray_image(roiX);
	
	//profile in Y
	Rect roiY(pinhole.x, pinhole.y-40, 1, 80);
	Mat chartROIy;
	CvScalar greenLine = cvScalar(255,0,0);
	chartROIy = gray_image(roiY);
	
	//cout << "gray_image.rows=" << gray_image.rows << " gray_image.cols=" << gray_image.cols << endl;
	for(int ch=0; ch<80; ch++) {
		int ptY = 200 - chartROIx.at<uchar>(0,ch);
		int ptY1 = 200 - chartROIy.at<uchar>(ch,0);
		int ptX = 5*ch;
		circle(chart1, cvPoint(ptX, ptY), 1, redLine);
		circle(chart1, cvPoint(ptX, ptY1), 1, greenLine);
		//imshow("pinhole profile", chart1);
	}
	QImage img2 = MatToQImage(chart1);
	//cout << "chart1.rows=" << chart1.rows << " chart1.cols=" << chart1.cols << endl;
	//cout << "Zeross.rows=" << Zeross.rows << " Zeross.cols=" << Zeross.cols << endl;
	//clear the chart1 container in order to paint the next profile
	bitwise_and(chart1, Zeross, chart1);
	//cout << "llegue" << endl;
	
	
	//chart1.convertTo(tmp, CV_8UC3);
	
	processedFrameCounter++;
	int index = processedFrameCounter % BUFFER_SIZE;
	//cout << " frames: " << processedFrameCounter << " index:" << index << endl;
	XBuffer[index] = corrX;
	YBuffer[index] = corrY;
	//dumpBuffer(XBuffer, "XBuffer");
	//dumpBuffer(YBuffer, "YBuffer");

	cout << "============median (" << getMedian(XBuffer) << "," << getMedian(YBuffer) << ") =============="  << endl;


	//update GUI
	emit newFrame(img);
	emit newIntensityProfile(img2);
	emit newCorrection(corrX, corrY);
	emit newThreshold(th);
	/*if(corrX > 1) {
		slewOn();
	} else {
		slewOff();
	}*/
	return true;
} 

void ImageProcessor::sendCorrection(int x, int y) {
	///* 
	//*  Centroide (Cx, Cy)
	//*  Pinhole (Px, Py)
	//*  x = Cx - Px
	//*  y = Cy - Py
	//*  Cx - Px  > 0 => move East
	//*  Cx - Px  < 0 => move West
	//*  Cy - Py  > 0 => move North
	//*  Cy - Py  < 0 => move South
	//*/

	//verify if it is time to apply the modifications
	//remember that the sampling rate and correction rate is separated.
	//the counter depends in the samplingFramePerSeconds variable. 
	//
	qDebug() << "processedFrameCounter=" << processedFrameCounter; 
	qDebug() << "correctionFramePerSeconds=" << samplingFramePerSeconds;
	
	//normalize the divder
	int divider = (int)samplingFramePerSeconds/correctionFramePerSeconds;
	int n = processedFrameCounter % divider;
	qDebug() << "n=" << n << " divider=" << divider;
	if( n != 0 ) {
		//it is not moment to apply the correction yet, just update the average and return;
		qDebug() << ">>>>>>>>>>>>>>>>>>>>>>>>>>> Calculating correction ...(" << processedFrameCounter << " index: " << n << ")";
		return;
	} 

	qDebug() << "Time to apply the correction, count: " << processedFrameCounter << " index: " << n;

	

	////limit is the hysteresis 
	int limitX = offsetCorrectionThreshold;
	int limitY = offsetCorrectionThreshold;
	int disable = offsetCorrectionDisableThreshold;

	//get the median. 
	int medianX = getMedian(XBuffer);
	int medianY = getMedian(YBuffer);

	qDebug() << "Instant correction are (" << x << "," << y << ")";
	qDebug() << "Differences are (" << medianX - x << "," << medianY - y << ")";
	qDebug() << "Applying the median of the corrections (" << medianX << "," << medianY << ")";

	//                 negative                |0|               positive
	//|-- disable --|-- enable --|-- disable --|0|-- disable --|-- enable --|-- disable --|
	//                (region B)                                 (region A) 

	if(!enableCorrection) {
		qDebug() << " --- Correction disabled ---";
		return;
	} else {
		qDebug() << " --- Correction enabled ---";
	}


	if( x > limitX && x < disable) {
        //move East (region A)
		string speed("Offset");
		string dir("W");
		if(enableCorrection) {
			slewOff(dir);
			slewOn(speed, dir);
			XStop = false;
		}
	} else if (x < -limitX && x > -disable) {
		//move West (region B)
		string speed("Offset");
		string dir("E");
		if(enableCorrection) {
			slewOff(dir);
			slewOn(speed, dir);
			XStop = false;
		}
	} else {
		//disable
		qDebug() << "disable region. X=" << x << " disable=" << disable; 		
		if(!XStop) {
			string dir("E");
			slewOff(dir);
			XStop = true;	
		} else {
			qDebug() << "already in stop";
		}

	}

	if ( y > limitY && y < disable) {
		//move S (region A)
		string speed("Offset");
		string dir("N");
		if(enableCorrection) {
			slewOff(dir);
			slewOn(speed, dir);
			YStop = false;
		}
	} else if ( y < -limitY && y > -disable) {
		//move South (region B)
		string speed("Offset");
		string dir("S");
		if(enableCorrection) {
			slewOff(dir);
			slewOn(speed, dir);
			YStop = false;
		}
	} else {
		//disable
		qDebug() << "Disable region Y=" << y << " limit=" << disable;
		if(!YStop) {
			string dir("S");
			slewOff(dir);
			YStop = true;
		} else {
			qDebug() << "already in stop";
		}
	}
}

void ImageProcessor::setupVideoInput() {
	if(cameraId < 0) {
		if(cameraId == -1) {
			setupVideoSource("c:\\tmp\\eso50cm\\guide1.mp4");
		} else if (cameraId == -2) {
			setupVideoSource("c:\\tmp\\eso50cm\\guide2.mp4");
		} else {
			setupVideoSource("c:\\tmp\\eso50cm\\guide1.mp4");
		}
	}
	//setupVideoSource(0);
	connectToObsControl();
	openVideoSource();
	processFrame();
}

void ImageProcessor::run() {
	//process_main("c:\\tmp\\eso50cm\\guide1.mp4");
	double _interval;
	
	while(1) {
		if(processFrame() == false) {
			break;
		}
		//QThread::msleep(33.3);
		_interval = 1 / (double)samplingFramePerSeconds;
		//max allowed values are 60 frame per seconds => 16 milliseconds between frame 
		if(_interval < 0.016) {
			_interval = 0.016;
		} 
		qDebug() << "frame per seconds: " << samplingFramePerSeconds << "; interval: " << _interval << " s";
		QThread::msleep(_interval*1000);
		//QThread::sleep(5);
	}
	qDebug() << "Thread quit ...";
}

int ImageProcessor::process_main(char* filename) {

	// variables
    double nframes; // number of frames in video file
    double elapsedtime=0;
    double fps;
    Mat frame;
    Mat mask;
    Mat object;
    Mat fst; //filtered image
    Mat gray_image;
    Mat gray_image1;
    Mat gray32;
    Mat grayDisp;
    Mat Zeross;
    Mat chartrgb;
    int rows;//frame rows
    int cols;//frame columns
    int enc;
    int reps = 1;
    float avX = 0;
    float avY = 0;
    float avCx = 0;
    float avCy =0;

	// open video file       
    VideoCapture cap(filename);
    cout << "Opening file: " << filename << endl;
    if(!cap.isOpened())  // check if we succeeded
        throw "Error when reading video file";
	
    //get number of frames
    nframes = cap.get(CV_CAP_PROP_FRAME_COUNT);
    fps = cap.get(CV_CAP_PROP_FPS);
    cols = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    rows = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    enc = cap.get(CV_CAP_PROP_FOURCC);
    cout << "number of frames " << nframes << " frames per second: " <<fps << " size: " << cols << "," << rows << "enc: " << enc << endl;
	//harde coded position of the pinhole in the image
    Point pinhole(383,252);
    
    //Charts
    Mat chart;
    Mat chart1;
    CvScalar greenLine;
    CvScalar redLine;
    CvScalar blueLine;
    CvScalar whiteLine;
    CvScalar black;
    CvFont font;
    
    greenLine = cvScalar(0, 255, 0);
    redLine = cvScalar(0, 0, 255);
    blueLine = cvScalar(255, 0, 0);
    whiteLine = cvScalar(255, 255, 255);
    black = cvScalar(0,0,0);
    cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0, 0, 1, 8);
    
    chart.create(201,512, CV_8U);
    chart1.create(201,400, CV_8UC3);
    Zeross = Mat::zeros(201, 400, CV_8UC3);
    //set display windows
    namedWindow("filtered",COLORMAP_COOL);
    namedWindow("original",1);
    namedWindow("mask",1);
    namedWindow("processing",COLORMAP_HOT);
    namedWindow("number of segmented points",1);
 
    cvMoveWindow("number of segmented points",300, 50);
    
    //meshgrid creation
    Mat XX,YY;
    XX.create(rows, cols, CV_32F);
    YY.create(rows, cols, CV_32F);
    for (int i = 0; i < rows; i++) {
        YY.at<float>(i,0) = (float)i;
    }
    
    for (int i = 0; i<cols;i++){
        XX.at<float>(0,i) =  (float)i;
    }
    XX = repeat(XX.row(0),rows,1);
    YY = repeat(YY.col(0),1,cols);
    
    //video processing loop
    cap >> frame; // get a new frame from video file
    cvtColor( frame, gray_image1, CV_BGR2GRAY );
    gray_image1.convertTo(gray_image1, CV_32F);

	for(int idx=2;idx<=nframes;idx++)
	//for(int idx=2;idx<=3;idx++)
    {
    
        clock_t begin = clock();
        //current frame reading ISSUE: data comes in 3 channels (RGB)!!!
        cout << "Current frame: " << cap.get(CV_CAP_PROP_POS_FRAMES) << " time: " << cap.get(CV_CAP_PROP_POS_MSEC)/1000 << endl;
        cap >> frame; // get a new frame from video file
 
        
        cvtColor( frame, gray_image, CV_BGR2GRAY );
        gray_image.convertTo(gray32, CV_32F);
                
        //median filter application, result to fst
        medianBlur ( gray_image, fst, 5 );

        //filtered image statistics: mean and standard deviation
        cv::Scalar cvMean;
        cv::Scalar cvStddev;
        meanStdDev(fst, cvMean, cvStddev);
        cout << " frame average: " << cvMean[0] << " std: " << cvStddev[0] << endl;
        
        //threshold for mask
        float th = cvMean[0] + 3.0*cvStddev[0];

        //mask definition and calculation
        mask = fst > th;
        //mask = gray_image > th;
        int totalpoints = sum(mask)[0]/1000;
        int posY = 200 - totalpoints;
        int posX = idx % 512;
        line(chart, cvPoint(posX+1,0), cvPoint(posX+1,200),0);
        circle(chart, cvPoint(posX, posY), 1, whiteLine, 1);
        imshow("number of segmented points", chart);
        //object segmentation
        bitwise_and(fst, mask, object);
       
        //Centroid estimation
        Mat object32;
        object.convertTo(object32, CV_32F);
        //Makes pinhole pixels = 0
        circle(object32, pinhole, 8, 0);
        Mat xst = object32.mul(XX);
        Mat yst = object32.mul(YY);
        imshow("filtered",xst);
        
        Scalar gx = sum(xst);
        Scalar gy = sum(yst);
        Scalar sobj = sum(object32);
        gx = gx/sobj;
        gy = gy/sobj;
        
        int cx = (int)gx[0];
        int cy = (int)gy[0];
        
        if (idx==2){
            avX = cx;
            avY = cy;
        }
        
        cout<<" centroid: ("<<cx<<","<<cy<<")" << endl;
        
        //Process time measuremen
        clock_t end = clock();
        
        int corry = cy - pinhole.y;
        int corrx = cx - pinhole.x;
        Rect roiX(pinhole.x-40, pinhole.y,80,1);
        Rect roiY(pinhole.x, pinhole.y-40,1,80);
        Mat chartROIx;
        Mat chartROIy;
        chartROIx = gray_image(roiX);
        chartROIy = gray_image(roiY);
					      
        for(int ch=0; ch<80; ch++){
			
            int ptY = 200 - chartROIx.at<uchar>(0,ch);
			int ptY1 = 200 - chartROIy.at<uchar>(ch,0);
			int ptX = 5*ch;
            //cout<<ptX<<" "<<ptY<<" " << endl;
            circle(chart1, cvPoint(ptX, ptY), 1, redLine);
            circle(chart1, cvPoint(ptX, ptY1), 1, greenLine);
            imshow("pinhole intensity profile", chart1);
        }
        
        bitwise_and(chart1, Zeross, chart1);
        //Adds circle around centroid
        Point center(cx,cy);
        Scalar colorcircle;
        colorcircle[2] = 128;
        circle(frame, pinhole, 8, colorcircle);
        Scalar colorline;
        colorline[0] = 128;
        line(frame, center, pinhole, colorline );
        
        //circle(fst, center, 20, 1);
        //choose area to zoom-in the result
        //Rect roi(300,200,200,100);
        int R = 100;
        Rect roi(pinhole.x-R,pinhole.y-R,2*R,2*R); 

        //objectROI = frame(roi);
        Mat objectROI;
        Mat circROI;
        
        objectROI = frame(roi);

        Mat imzoom;
        cout<<" X correction:"<< center.x - pinhole.x<<" Y correction:"<<center.y - pinhole.y<< endl;
        if((center.x - pinhole.x)==0 && (center.y - pinhole.y==0)){
            cout<< " Center at frame "<< idx << endl;            
        }
        
        //Averaging routine, output to gray_image1
        if (reps ==1){
            gray_image1 = gray32;
        }
        else if(reps < 100){
            add((reps-1)*gray_image1/reps, gray32/reps, gray_image1);            
        } else{
            gray_image1.convertTo(grayDisp, CV_8U);
            circle(grayDisp, pinhole, 8, 4);
            line(grayDisp, center, pinhole, colorline );
            imshow("original", grayDisp);
            reps = 1;
        }
        reps += 1;

		//Zoom in        
        pyrUp( objectROI, imzoom, Size( objectROI.cols*2, objectROI.rows*2 ));
        pyrUp( imzoom, imzoom, Size( imzoom.cols*2, imzoom.rows*2 ));
        //pyrUp( imzoom, imzoom, Size( imzoom.cols*2, imzoom.rows*2 ));
        //pyrUp( imzoom, imzoom, Size( imzoom.cols*2, imzoom.rows*2 ));
        //pyrUp( imzoom, imzoom, Size( imzoom.cols*2, imzoom.rows*2        //IMAGES display
        //imshow("filter", grayimage1);        
        //imshow("mask", mask);
        imshow("processing", imzoom);
              
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        elapsedtime = elapsedtime + elapsed_secs;
        cout << " elapsed time: " << elapsedtime << " delta time: " << elapsed_secs << endl;
        avX = ((idx-1)*avX + center.x)/idx;
        avY = ((idx-1)*avY + center.y)/idx;
      
        if(idx==2){
            avCx = center.x - pinhole.x;
            avCy = center.y - pinhole.y;
        }
        
        avCx = ((idx-1)*avCx + (center.x - pinhole.x))/idx;
        avCy = ((idx-1)*avCy + (center.y - pinhole.y))/idx;
        cout<<" Average centroid X:" << avX << " Y:" << avY << " Average Correction X:" << avCx << " Y:" << avCy << endl;
        //End of the process loop - out condition
        if(waitKey(30) >= 0) break;
    }
    cout<<"---> Final average centroid X:" << avX << " Y:" << avY << " Average Correction X:" << avCx << " Y:" << avCy << endl;
    //record.release();
    // the camera will be deinitialized automatically in VideoCapture destructor
	waitKey(0);
    return 0;
}


