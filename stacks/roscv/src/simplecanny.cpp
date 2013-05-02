#include <ros/ros.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cv_bridge/CvBridge.h>
#include <cvaux.h>
#include <math.h>
#include <cxcore.h>
#include <highgui.h>
#include <cvblob.h>

using namespace std;
using namespace cv;
using namespace cvb;

namespace enc = sensor_msgs::image_encodings;

static const char WINDOW[] = "Image window";

class simplecanny {
	ros::NodeHandle nh_;
	ros::NodeHandle n;
	ros::Publisher pub;
	image_transport::ImageTransport it_;
	image_transport::Subscriber image_sub_; //image subscriber
	image_transport::Publisher image_pub_; //image publisher(we subscribe to ardrone image_raw)
	std_msgs::String msg;
	IplImage* img;
	IplImage* frame; //Original Image
	IplImage* hsvframe; //Image in HSV color space
	IplImage* labelImg; //Image Variable for blobs
	IplImage* threshy; //Threshold image of red color
	unsigned int result; //Blob result
	int panjang;
	int lebar;

public:
	simplecanny() :
			it_(nh_) {

		image_sub_ = it_.subscribe("/ardrone/image_raw", 1,
		&simplecanny::imageCb, this);
		image_pub_ = it_.advertise("/arcv/Image", 1);
		
	}

	~simplecanny() {
		cv::destroyWindow(WINDOW);
	}

	void imageCb(const sensor_msgs::ImageConstPtr& msg) {
		
		//Structure to hold blobs
		CvBlobs blobs;
		
		sensor_msgs::CvBridge bridge; //we need this object bridge for facilitating conversion from ros-img to opencv
		
		Mat temp = bridge.imgMsgToCv(msg, "bgr8"); //image being converted from ros to opencv using cvbridge
		
		img = new IplImage(temp);;

		// Get image size(Height, Width)
		CvSize imgSize = cvGetSize(img);

		frame = cvCreateImage(imgSize, 8, 3);   //Original Image
		hsvframe = cvCreateImage(imgSize, 8, 3); //Image in HSV color space
		labelImg = cvCreateImage(imgSize, IPL_DEPTH_LABEL, 1); //Image Variable for blobs
		threshy = cvCreateImage(imgSize, 8, 1); //Threshold image of red color
		
		cvResize(img, frame, CV_INTER_LINEAR);

		//Flipping the frame
		cvFlip(frame, frame, 1);

		//Changing the color space
		cvCvtColor(frame, hsvframe, CV_BGR2HSV);

		//Thresholding the frame for red
		cvInRangeS(hsvframe, cvScalar(154, 75, 128), cvScalar(255, 255, 255),
				threshy);

		//Dilate image
		cvDilate(hsvframe, hsvframe, NULL, 4);

		//Filtering the frame
		cvSmooth(threshy, threshy, CV_MEDIAN, 7, 7);

		//Finding the blobs
		result = cvLabel(threshy, labelImg, blobs);

		//Rendering the blobs
		cvRenderBlobs(labelImg, blobs, frame, frame);

		//Filtering the blobs
		cvFilterByArea(blobs, 60, 500);

		for (CvBlobs::const_iterator it = blobs.begin(); it != blobs.end();
				++it) {
			panjang = it->second->maxy - it->second->miny;
			lebar = it->second->maxx - it->second->minx;

			std::cout << "Luas Blob = " << (panjang * lebar) << std::endl;
		}

		cvShowImage("red_object_tracking", frame);

		cvWaitKey(2); // wait for 2 milliseconds

		// Release All image
		cvReleaseImage(&frame);
		cvReleaseImage(&hsvframe);
		cvReleaseImage(&labelImg);
		cvReleaseImage(&threshy);
		
	}
};

int main(int argc, char** argv) {
	ros::init(argc, argv, "simple_canny");
	simplecanny ic;
	ros::spin();

	return 0;
}
