#include <opencv2/opencv.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

void loadAVI() {
	CvCapture* capture = cvCaptureFromAVI( "guide.avi" );
	if(capture) {
		int videoFps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
		cout << "fps=" << videoFps << endl;
		cvNamedWindow("video",0);		
		//cvGrabFrame(capture);
		cout << "antes" << endl;
		IplImage* iplImg = cvQueryFrame( capture );
		cvShowImage("video", iplImg);
		cvWaitKey(1000);
	} else {
		cout << "error reading avi" << endl;
	}
}

void loadCAM() {
	Mat img;
	VideoCapture cap(0);

	//CvCapture* capture = cvCaptureFromCAM( CV_CAP_DSHOW );
	while (true)
	{
		cap >> img;

		Mat edges(Mat::zeros(100, 100, CV_8U));
		cvtColor(img, edges, CV_BGR2GRAY);

		Canny(edges, edges, 30, 60);

		imshow("window label", edges);
		waitKey(1);
	}
}

void loadPNG() {
	IplImage* img = cvLoadImage("0990.png");
	if(!img) {
		cout << "failed loading image" << endl;		
	} 

	cvNamedWindow("eso50cm autoguiding");
	cvShowImage("eso50cm autoguiding", img);
	cvWaitKey(0);
	cvReleaseImage(&img);
}


//int main()
//{
//    loadPNG();
//    return 0;
//}


int main(int argc, char** argv)
{

	//Test for file input
	if (argc <= 1)
	{
		cout << "Usage: " << argv[0] << " <Filename>" << endl;
		exit(1);
	}
	// open video file       
	VideoCapture cap(argv[1]);
	//VideoCapture cap("c:\tmp\eso50cm\guide2.mp4");

	cout<<"Opening file: "<<argv[1]<<"\n";
	if(!cap.isOpened())  // check if we succeeded
		throw "Error when reading steam_avi";
	// variables

	double nframes; // number of frames in video file
	double elapsedtime=0;
	double fps;

	Mat mask;
	Mat object;
	Mat fst; //filtered image
	Mat gray_image;
	Mat gray_image1;
	Mat gray32;
	Mat grayDisp;
	int rows;//frame rows
	int cols;//frame columns
	int enc;
	int reps = 1;
	float avX = 0;
	float avY = 0;
	float avCx = 0;
	float avCy =0;
	//get number of frames
	nframes = cap.get(CV_CAP_PROP_FRAME_COUNT);
	fps = cap.get(CV_CAP_PROP_FPS);
	cols = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	rows = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	enc = static_cast<int>(cap.get(CV_CAP_PROP_FOURCC));
	std::cout << "number of frames "<< nframes<<" frames per second: "<<fps<<" size: "<<cols<<","<<rows<<"enc: "<<enc<<"\n";
	cv::Size s;
	s.height = 800; //rows;
	s.width = 800; //cols;
	
	VideoWriter writer;
	const string source      = argv[1];
	string::size_type pAt = source.find_last_of('.');
	const string NAME = source.substr(0, pAt) + "-hola" + ".avi";
	int codec = 1684633187; //cinepak
	writer.open(NAME, 1684633187, fps, s, true); 
	if(!writer.isOpened()) {
		cout << "failed to create " << NAME << endl; 
		return 0;
	}else {
		cout << "LISTO!!!!!!!, filename:" << NAME << endl;
	}
	
	Point pinhole(383,252);

	//set display windows
	namedWindow("filter",COLORMAP_COOL);
	namedWindow("original",1);
	namedWindow("mask",1);
	namedWindow("object",COLORMAP_HOT);
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
	Mat frame = Mat::ones(640, 480,CV_BGR2GRAY);
	try {
		cap >> frame; // get a new frame from video file
		cvtColor( frame, gray_image1, CV_BGR2GRAY );
		gray_image1.convertTo(gray_image1, CV_32F);
	} catch(cv::Exception) {
		cout << "cv exceptions captured." << endl;
		return 0;
	}
	
	for(int idx=2;idx<=nframes;idx++)
	{
		clock_t begin = clock();
		//current frame reading ISSUE: data comes in 3 channels (RGB)!!!
		std::cout <<"Current frame: "<< cap.get(CV_CAP_PROP_POS_FRAMES)<<" time: "<<cap.get(CV_CAP_PROP_POS_MSEC)/1000<<" ";
		cap >> frame; // get a new frame from video file
		
		cvtColor( frame, gray_image, CV_BGR2GRAY );
		gray_image.convertTo(gray32, CV_32F);

		if (reps ==1){
			gray_image1 = gray32;
		}
		else if(reps < 100){
			add((reps-1)*gray_image1/reps, gray32/reps, gray_image1);

		}
		else{
			gray_image1.convertTo(grayDisp, CV_8U);
			circle(grayDisp, pinhole, 8, 4);
			imshow("original", grayDisp);
			reps = 1;
		}
		reps += 1;
		//median filter application, result to fst
		medianBlur ( gray_image, fst, 5 );

		//filtered image statistics: mean and standard deviation
		cv::Scalar cvMean;
		cv::Scalar cvStddev;
		meanStdDev(fst, cvMean, cvStddev);
		cout<<"frame average: "<<cvMean[0]<<" std: "<<cvStddev[0];

		//threshold for mask
		float th = cvMean[0] + 3.0*cvStddev[0];

		//mask definition and calculation
		mask = fst > th;
		//object segmentation
		bitwise_and(fst, mask, object);
		//object.convertTo(object, CV_16U);

		//object.convertTo(object, CV_32F);
		
		//Centroid estimation
		Mat object32;
		object.convertTo(object32, CV_32F);
		Mat xst = object32.mul(XX);
		Mat yst = object32.mul(YY);
		imshow("filter",xst);

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

		cout<<" centroid: ("<<cx<<","<<cy<<")";

		//Process time measurement
		clock_t end = clock();

		//Adds circle around centroid
		Point center(cx,cy);
		circle(frame, pinhole, 8, 1);
		line(frame, center, pinhole, 1);
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
		cout<<" X correction:"<< center.x - pinhole.x<<" Y correction:"<<center.y - pinhole.y<<"\n";
		if((center.x - pinhole.x)==0 && (center.y - pinhole.y==0)){
			cout<< "Center at frame "<< idx;

		}

		pyrUp( objectROI, imzoom, Size( objectROI.cols*2, objectROI.rows*2 ));
		pyrUp( imzoom, imzoom, Size( imzoom.cols*2, imzoom.rows*2 ));
		//pyrUp( imzoom, imzoom, Size( imzoom.cols*2, imzoom.rows*2 ));
		//pyrUp( imzoom, imzoom, Size( imzoom.cols*2, imzoom.rows*2 ));
		//pyrUp( imzoom, imzoom, Size( imzoom.cols*2, imzoom.rows*2        //IMAGES display
		//imshow("filter", grayimage1);

		//imshow("mask", mask);
		imshow("object", imzoom);

		cout << "writing the frame " << imzoom.size()  <<  " to out.mp4" << endl;
		writer << imzoom;

		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		elapsedtime = elapsedtime + elapsed_secs;
		cout<<" elapsed time: "<< elapsedtime<<" delta time: "<<elapsed_secs<<"\n";
		avX = ((idx-1)*avX + center.x)/idx;
		avY = ((idx-1)*avY + center.y)/idx;

		if(idx==2){
			avCx = center.x - pinhole.x;
			avCy = center.y - pinhole.y;
		}

		avCx = ((idx-1)*avCx + (center.x - pinhole.x))/idx;
		avCy = ((idx-1)*avCy + (center.y - pinhole.y))/idx;
		cout<<"Average centroid X:"<< avX<<" Y:"<< avY<< " Average Correction X:"<< avCx << " Y:"<<avCy<<"\n";
		//End of the process loop - out condition
		if(waitKey(30) >= 0) break;
	}
	cout<<"Average centroid X:"<< avX<<" Y:"<< avY<< " Average Correction X:"<< avCx << " Y:"<<avCy<<"\n";
	//record.release();
	// the camera will be deinitialized automatically in VideoCapture destructor
	return 0;
}
