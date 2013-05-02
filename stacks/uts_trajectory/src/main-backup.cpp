#include <iostream>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "cmvision/Blob.h"
#include "cmvision/Blobs.h"
#include "ardrone_brown/Navdata.h"
#include "std_msgs/Empty.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Empty.h"


//Untuk trajectory 
#include <ros/ros.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/CvBridge.h>
#include <sensor_msgs/Image.h>

#define IMAGE_WIDTH 176
#define IMAGE_HEIGHT 144
#define IMAGE_MIDDLE_X 88
#define IMAGE_MIDDLE_Y 72


using namespace cmvision;

/**
  * Atribut subscriber, publisher dan pesan (message)
  */
ros::Publisher landing; 
ros::Publisher velo; 
ros::Publisher takeoff; 
ros::Subscriber nav;    
std_msgs::Empty empty;

//Kecepatan actual (sebenarnya)
float vx;
float vy;
float vz;

//Posisi blob
int blob_x;
int blob_y;

//Tipe data untuk gambar
IplImage *mapImage;
CvSize imageSize = cvSize(800, 600); 
char* mapWindow = "Map Window";

void blobTracking();
void setSpeed(float vx_des, float vy_des);

/** Fungsi yang dipanggil secara callback saat ada Navdata
  * Fungsi ini akan menangkap data pada tipe data Navdata
  */
void navi(const ardrone_brown::Navdata& msg){
	vx = msg.vx;
	vy = msg.vy;
	vz = msg.vz;
}

/** Fungsi yang dipanggil secara callback saat ada input gambar
  * Fungsi ini akan menangkap (subscribe) blob pada gambar jika ada
  */
void blobDetection(cmvision::Blobs blobs)
{
	std::vector<Blob> b = blobs.blobs; 
	ROS_INFO("==== incoming ==== \n");
    
	int indexChoosen = 0;
	int area = 0;
 
	if(b.size() > 0) {
		for (int k = 0; k < b.size(); k++) {
			if(b[k].area > area ) {
				indexChoosen = k;
				area =  b[k].area;
      		}
		}
		blob_x = b[indexChoosen].x;
		blob_y = b[indexChoosen].y; 
        ROS_INFO("x = %d : y = %d ",blob_x, blob_y);
	}
	else {
		blob_x = 0;
		blob_y = 0;
	}
}

void initGambar(void) {
    //mapImage    = cvCreateImage(cvSize(800, 600), IPL_DEPTH_8U, 3);
	mapImage	  = cvLoadImage("lena1.png", CV_LOAD_IMAGE_GRAYSCALE);
	cvNamedWindow(mapWindow, CV_WINDOW_AUTOSIZE);
	//frame = mapImage;
	//for(int y=0; y<mapImage->height; y++ ) {
    //    uchar* ptr = (uchar*) (
    //    mapImage->imageData + y * mapImage->widthStep);
    //    for(int x=0; x<mapImage->width; x++ ) {
    //        ptr[3*x] = 255;
    //        ptr[3*x+1] = 255;
    //       ptr[3*x+2] = 255;         
    //    }
    //}
    
    
    //Make grid
    //int x1 = 0, x2=799, y1=0, y2=0;
    
    //while (y2 < 600) {
	//	cvLine(mapImage, cvPoint(x1,y1), cvPoint(x2,y2), CV_RGB(0,0,255), 5, 8, 0);
	//	y2+=10;
	//}
	//cvCircle(mapImage, cvPoint(cvRound(188),cvRound(72)),
    //                                3, CV_RGB(0,255,0), -1, 8, 0 );
	cvShowImage(mapWindow, mapImage);
	if((cvWaitKey(1) & 255) == 27)return;
	//cvShowImage(mapWindow, frame);
	//cv::imshow(mapWindow,  mapImage);
}

/**
 * Fungsi main : tempat program pertama kali dijalankan
 * 
 */
int main(int argc, char **argv)
{
  //Inisialisasi 
  ros::init(argc, argv, "uts_node");

  //Inisialisasi variable
  ros::NodeHandle control, n;
  ROS_INFO("Frankenstein \n");  	

  velo =  control.advertise<geometry_msgs::Twist>("/cmd_vel",10);
  
  ros::Duration delay(0.1) ;

  //inisialisasi fungsi callback
  nav = control.subscribe("/ardrone/navdata",10,&navi);
  ros::Subscriber sub = n.subscribe("/blobs", 1000, blobDetection);

  //Infinite loop saat program dijalankan
  ros::Duration rate(0.1);
  initGambar();
  while(ros::ok()) {
	 //Baca sensor
     ros::spinOnce();
     ros::spinOnce();
     
     //Handle jika ada blob (objek)
     if(!(blob_x==0&&blob_y==0)) {
	 
	 }
     //Handle jika tidak ada blob (objek)
     else {
     	
     }
     
     rate.sleep();
  }
  return 0;
}
