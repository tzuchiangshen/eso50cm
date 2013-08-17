#pragma once

#include <string>
#include <iostream>
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>



class ImageProcessor 
{
public:
	ImageProcessor(void);
	~ImageProcessor(void);
	const char * sayHello(char *name);
	int ImageProcessor::process_main(char*filename);
};

