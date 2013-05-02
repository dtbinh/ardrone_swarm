#include "ros/ros.h"
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
	


	::rotX = navigationData->rotX;
	::rotY = navigationData->rotY;
	::rotZ = navigationData->rotZ;
}

/**
  * Fungsi utama untuk menjalankan node distance_tracker
  */
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
	ros::Publisher distancePub = n.advertise<geometry_msgs::Pose>("/drone_pos", 1000);
	
	float x = 0; 
	float y = 0;


	/** Loop utama untuk menghitung jejak ARDrone per 1 detik. */
	while (ros::ok()) {
		
		geometry_msgs::Pose targetPose;
		geometry_msgs::Point point2D;
		
		/** Perhitungan RotX * RotY * RotZ */
		float baris1[3] = { (cos(::rotX) * cos(::rotZ)), 
			(-cos(::rotY) * sin(::rotZ)) + (sin(::rotY) * sin(::rotX) * cos(::rotZ)),
			(sin(::rotY) * sin(::rotZ)) + (cos(::rotY) * sin(::rotX) * cos(::rotZ))};
		
		float baris2[3] = { (cos(::rotX) * sin(::rotZ)), 
			(cos(::rotY) * cos(::rotZ)) + (sin(::rotY) * sin(::rotX) * sin(::rotZ)),
			(-sin(::rotY) * cos(::rotZ)) + (cos(::rotY) * sin(::rotX) * sin(::rotZ))};
			
		float baris3[3] = { -sin(rotX), 
			sin(rotY) * cos(rotX),
			cos(rotY) * cos(rotX)};
		
		float velocity[3] = {::vx, ::vy, ::vz};
		
		/** Hasil kecepatan absolut. */
		float velocityX = (baris1[0] * velocity[0]) + (baris1[1] * velocity[1]) + (baris1[2] * velocity[2]);
		float velocityY = (baris2[0] * velocity[0]) + (baris2[1] * velocity[1]) + (baris2[2] * velocity[2]);
		float velocityZ = (baris3[0] * velocity[0]) + (baris3[1] * velocity[1]) + (baris3[2] * velocity[2]);
		
		ros::Duration time(1);
		time.sleep();
		
		x = x + (velocityX);
		y = y + (velocityY);
		//ROS_INFO("Posisi X = %f meter, Posisi Y = %f meter", velocityX, velocityY);
		ROS_INFO("Posisi X = %f meter, Posisi Y = %f meter", x, y);
		/** Inisialisasi data topik */
		point2D.x = x;
		point2D.y = y;
		point2D.z = 0;
		targetPose.position = point2D;

		//ROS_INFO("Posisi X = %f meter, Posisi Y = %f meter", x, y);
		
		distancePub.publish(targetPose);
		
		ros::spinOnce();
	}
	
	
	return 0;
}*/


