#include <ros/ros.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include "std_msgs/String.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cvaux.h>
#include <cxcore.h>
#include <highgui.h>
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"
#include "ardrone_autonomy/Navdata.h"
#include <math.h>
#include <sstream>

/** 
  * Mengukur dan melacak jarak yang telah ditempuh oleh ARDrone
  * melalui topik /drone_pos yang telah dipublish.
  * 
  * Kelompok IV (SandalTerbang) Robotika 2012
  * 
  */

/** Kecepatan relatif terhadap kemiringan ARDrone. */
float vx = 0;
float vy = 0;
float vz = 0;

/** Besar sudut rotasi yang digunakan ARDrone. */
float rotX = 0;
float rotY = 0;
float rotZ = 0;

/** Callback untuk mengambil navdata dari ARDrone. */
void navdataCallback(const ardrone_autonomy::Navdata::ConstPtr& navigationData)
{
    /** X = banking, Y = pitch, Z = yaw (putar datar) */
    ::vx = navigationData->vx;
    ::vy = navigationData->vy;
    ::vz = navigationData->vz;


    // rotX = Roll;
    // rotY = Pitch;
    // rotZ = Yaw;

    ::rotX = navigationData->rotX;
    ::rotY = navigationData->rotY;
    ::rotZ = navigationData->rotZ;
}

int main(int argc, char **argv) 
{
 
    /**
      * Inisialisasi node baru yang bernama "distance_tracker" dan terhubung 
      * dengan argumen command-line.
      */
    ros::init(argc, argv, "distance_tracker");
    
    ros::NodeHandle n;
    
    /** Subscribe terhadap navdata. */
    ros::Subscriber navdataSub = n.subscribe("/ardrone/navdata", 10, navdataCallback);
    /** Publish hasil perhitungan pelacakan ARDrone. */
    //ros::Publisher distancePub = n.advertise<geometry_msgs::Pose>("/drone_pos", 1000);

    // Map
    IplImage *map = cvCreateImage(cvSize(500, 500), IPL_DEPTH_8U, 3);
    cvZero(map);
 
    // Position matrix
    cv::Mat P = cv::Mat::zeros(3, 1, CV_64FC1);
    
    cv::Mat temp = cv::Mat::zeros(3, 1, CV_64FC1);

    float x = 0;
    float y = 0;  
    /** Loop utama untuk menghitung jejak ARDrone per 1 detik. */
    while (ros::ok()) {
        
        //geometry_msgs::Pose targetPose;
        //geometry_msgs::Point point2D;
 
        // Rotation matrices
        double _RX[] = {        1.0,       0.0,        0.0,
                                0.0, cos(::rotX), -sin(::rotX),
                                0.0, sin(::rotX),  cos(::rotX)};
        double _RY[] = { cos(::rotY),       0.0,  sin(::rotY),
                                0.0,       1.0,        0.0,
                        -sin(::rotY),       0.0,  cos(::rotY)};
        double _RZ[] = {   cos(::rotZ), -sin(::rotZ),        0.0,
                           sin(::rotZ),  cos(::rotZ),        0.0,
                                0.0,       0.0,        1.0};
        cv::Mat RX(3, 3, CV_64FC1, _RX);
        cv::Mat RY(3, 3, CV_64FC1, _RY);
        cv::Mat RZ(3, 3, CV_64FC1, _RZ);
 
        // Time
       static double last = ros::Time::now().toSec();
       double dt = (ros::Time::now().toSec() - last);
       last = ros::Time::now().toSec();
        
        //static int last = cv::getTickCount();
        //double dt = (cv::getTickCount() - last) / cv::getTickFrequency();
        //last = cv::getTickCount();

        // Local movement
        double _M[] = {::vx, ::vy, ::vz};
        cv::Mat M(3, 1, CV_64FC1, _M);
 
        // Dead reckoning
        temp = RZ * RY * RX * M;
        P = P + temp;
        
        x = x + dt * ::rotZ * P.at<double>(0,0);
        y = y + dt * ::rotZ * P.at<double>(1,0);

        ros::Duration time(1);
        time.sleep();
       
        // Position (x, y, z)
        double pos[3] = {P.at<double>(0,0), P.at<double>(1,0), P.at<double>(2,0)};
        ROS_INFO("X = %f , Y = %f, Z = %f, Waktu = %f ", pos[0], pos[1], pos[2], dt);
        //ROS_INFO("X = %f , Y = %f, Z = %f, Waktu = %f ", x, y);
        
        // Display the image
        cvDrawCircle(map, cvPoint(-pos[1]*30.0 + map->width/2, -pos[0]*30.0 + map->height/2), 2, CV_RGB(255,0,0));
        cvShowImage("map", map);
        ros::spinOnce();
    }
 
    cvReleaseImage(&map);
 
    return 0;
}