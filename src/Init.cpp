#include <cstdlib>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include <math.h>
#include <iostream>
#include <time.h>
#include "properties.h"
#include <windows.h>

using namespace cv;
using namespace std;

int detectLight();
int playVideo();
int getFromCamera();

string videoDir;
string delayAfterVideoFinished;
string delayBeforeVideoStarted;
string delayBeforeCloseWebcam;
string webcamSource;
string delayAfterWebcamCloses;
string ambientBrightnessThreshold;
string isFullScreen;
IplImage* tmpBlackImgPointer;

int main(int argc, char* argv[])
{
	if(initializeProperties(PROP_FILE_PATH) == 0)
	{
		videoDir = findPropertyValueByKey(VIDEO_DIR);
		delayAfterVideoFinished = findPropertyValueByKey(DELAY_AFTER_VIDEO_FINISHED);
		delayBeforeVideoStarted = findPropertyValueByKey(DELAY_BEFORE_VIDEO_STARTED);
		delayBeforeCloseWebcam = findPropertyValueByKey(DELAY_BEFORE_CLOSE_WEBCAM);
		webcamSource = findPropertyValueByKey(WEBCAM_SOURCE);
		delayAfterWebcamCloses = findPropertyValueByKey(DELAY_AFTER_WEBCAM_CLOSES);
		ambientBrightnessThreshold = findPropertyValueByKey(AMBIENT_BRIGHTNESS_THRESHOLD);
		isFullScreen = findPropertyValueByKey(IS_FULL_SCREEN);
		tmpBlackImgPointer = cvCreateImage(cvSize(1920,1080),8,3);
		cvZero(tmpBlackImgPointer);
		while(1)
		{
			if(detectLight()>0)
			{
				cout<<"LightDetected"<<endl;
				Sleep(atoi(delayBeforeVideoStarted.c_str())); // on windows system : #include <windows.h>
				if(playVideo() == 2)
				{
					Sleep(atoi(delayAfterVideoFinished.c_str())); // on windows system : #include <windows.h>
					getFromCamera();
					Sleep(atoi(delayAfterWebcamCloses.c_str())); // on windows system : #include <windows.h>
				}
			}
		}
	}
	else
	{
		cout<<"Property file initialization error !!!"<<endl;
	}
	return 0;
}

int playVideo()
{
	cout<<"Starting to play video"<<endl;
	cout<<"videoDir: " << videoDir<<endl;
	VideoCapture cap(videoDir);
	if (!cap.isOpened())
	{
		 cout << "Cannot open the video file" << endl;
		 return -1;
	}
	double fps = cap.get(CV_CAP_PROP_FPS);
	cout << "Frame per seconds : " << fps << endl;
	cvNamedWindow("OzanCanCicek", CV_WINDOW_NORMAL);
	if(isFullScreen.compare("true") == 0)
	{
		cvSetWindowProperty("OzanCanCicek", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	}
	while(1)
	{
		Mat frame;
		bool bSuccess = cap.read(frame);
		 if (!bSuccess)
		{
				cout << "Cannot read the frame from video file" << endl;
				cout<< "Playing video finished"<<endl;
				cvShowImage("OzanCanCicek",tmpBlackImgPointer);
				return 2;
		}
		IplImage tmp = frame;
		IplImage* tmpFramePointer = &tmp;
		cvShowImage("OzanCanCicek",tmpFramePointer);
		if(waitKey(30) >= 0)
		{
			cout << "escaped by user" << endl;
			break;
		}
	}
	return 0;
}

int getFromCamera()
{
	cout<<"Capturing from webcam started"<<endl;
	VideoCapture cap(atoi(webcamSource.c_str()));
	if(!cap.isOpened())
	{
		cout<<"Webcam error"<<endl;
	    return -1;
	}
	time_t start,end;
	int elapsed; //seconds
	start = time(NULL);
	int _terminate = 1;
	cvNamedWindow("OzanCanCicek", CV_WINDOW_NORMAL);
	if(isFullScreen.compare("true") == 0)
	{
		cvSetWindowProperty("OzanCanCicek", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	}

	while(_terminate)
	{
	    Mat frame;
	    cap >> frame;
	    IplImage tmp = frame;
	    IplImage* tmpFramePointer = &tmp;
	    cvShowImage("OzanCanCicek",tmpFramePointer);
	    if(waitKey(30) >= 0) break;
	    end = time(NULL);
		elapsed = difftime(end,start);
		if(elapsed >=atoi(delayBeforeCloseWebcam.c_str()))
		{
			cout<<"Capturing from webcam finished"<<endl;
			cvShowImage("OzanCanCicek",tmpBlackImgPointer);
			cap.release();
			_terminate = 0;
		}
		if(waitKey(30) >= 0)
		{
			break;
		}
	}
	return 0;
}

int detectLight()
{
	cout<<"Detecting lights started"<<endl;
	VideoCapture cap(atoi(webcamSource.c_str()));
	if(!cap.isOpened())
	{
		cout<<"Camera Error"<<endl;
		return -1;
	}
	for(;;)
	{
		Mat frame;
		Mat greyFrame;
		cap >> frame;
		Mat temp, color[3], lum;
		//
		double brightnessThreshold = atof(ambientBrightnessThreshold.c_str());
		//
		double brightness;
		temp = frame;
		split(temp, color);
		color[0] = color[0] * 0.299;
		color[1] = color[1] * 0.587;
		color[2] = color[2] * 0.114;
		lum = color[0] + color [1] + color[2];
		Scalar summ = sum(lum);
		brightness = summ[0]/((pow(2,8)-1)*frame.rows * frame.cols) * 2;
		if(brightness >= brightnessThreshold)
		{
			cout<<"Detecting lights finished. Light Detected!!!"<<endl;
			cap.release();
			return 1; //light detected
		}
	}
	return 0;
}
