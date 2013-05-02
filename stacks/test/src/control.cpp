#include <iostream>
#include <ardrone_autonomy/Navdata.h>
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <cmvision/Blob.h>
#include <cmvision/Blobs.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Empty.h>

//Untuk trajectory 
#include <opencv/cv.h>
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

//Atribut kontrol PID untuk diam
float r_x_old = 0.0;
float r_y_old = 0.0;
float e_x_old = 0.0;
float e_y_old = 0.0;
float e_x_old2 = 0.0;
float e_y_old2 = 0.0;

//Atribut PID untuk tracking blob
float rt_x_old = 0.0;
float rt_y_old = 0.0;
float et_x_old = 0.0;
float et_y_old = 0.0;
float et_x_old2 = 0.0;
float et_y_old2 = 0.0;

//Konstanta PID untuk diam
float kp_x = 0.0003;
float kp_y = 0.0003;
float ki_x = 0;
float ki_y = 0;
float kd_x = 0;
float kd_y = 0;

//Konstanta PID untuk tracking
float kpt_x = 0.0007;
float kpt_y = 0.0007;
float kit_x = 0;
float kit_y = 0;
float kdt_x = 0;
float kdt_y = 0;

//Tipe data untuk gambar
IplImage *cvImage;
CvSize size;
char* mapWindow = "Map Window";

void blobTracking();
void setSpeed(float vx_des, float vy_des);

/** Fungsi yang dipanggil secara callback saat ada Navdata
  * Fungsi ini akan menangkap data pada tipe data Navdata
  */
void navi(const ardrone_autonomy::Navdata& msg){
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


/** Fungsi untuk mengatur kecepatan robot pada bidang horizontal
  * Fungsi ini mengimplementasikan kontrol PID dengan mengatur kecepatan
  * (kemiringan robot) dan feedback dari sensor built in di robot
  */
void setSpeed(float vx_des, float vy_des) {
	geometry_msgs::Twist direction;
  
	float e_x = vx_des - vx;
	float e_y = vy_des - vy;
	direction.linear.x = kp_x * e_x ;
	direction.linear.y = kp_y * e_y  ; 

	if(direction.linear.x > 0.4) direction.linear.x =  0.4;
	else if(direction.linear.x < -0.4) direction.linear.x =  -0.4;
	if(direction.linear.y > 0.4) direction.linear.y = 0.4;
	else if(direction.linear.y < -0.4) direction.linear.y =  -0.4;
  
	ROS_INFO("speed x = %f y = %f \n", direction.linear.x, direction.linear.y);
	velo.publish(direction); 
}

/** Fungsi untuk mtracking objek (blob)
  * Fungsi ini mengimplementasikan kontrol PID dengan mengatur posisi robot
  * terhadap objek dengan feedback data dari kamera
  */
void blobTracking() {
 	geometry_msgs::Twist direction;

	float e_x = IMAGE_MIDDLE_Y - blob_y;
	float e_y = (IMAGE_MIDDLE_X - blob_x);

	direction.linear.x =  kpt_x * e_x ;  
	direction.linear.y =  kpt_y * e_y ;
  
	if(direction.linear.x > 0.4) direction.linear.x =  0.4;
	else if(direction.linear.x < -0.4) direction.linear.x =  -0.4;
  	if(direction.linear.y > 0.4) direction.linear.y = 0.4;
  	else if(direction.linear.y < -0.4) direction.linear.y =  -0.4;

	ROS_INFO("correction tracking x = %f y = %f \n", direction.linear.x, direction.linear.y);
	velo.publish(direction); 		
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

  takeoff = control.advertise<std_msgs::Empty>("/ardrone/takeoff",10);
  landing = control.advertise<std_msgs::Empty>("/ardrone/land",10);
  velo =  control.advertise<geometry_msgs::Twist>("/cmd_vel",10);
  takeoff = control.advertise<std_msgs::Empty>("/ardrone/takeoff",10);

  
  ros::Duration delay(0.1) ;

  //inisialisasi fungsi callback
  nav = control.subscribe("/ardrone/navdata", 10, &navi);
  ros::Subscriber sub = n.subscribe("/blobs", 1000, blobDetection);

  //Infinite loop saat program dijalankan
  ros::Duration rate(0.1);
  while(ros::ok()) {
	 //Baca sensor
     ros::spinOnce();
     ros::spinOnce();
     ros::spinOnce();
     ros::spinOnce();
     
     //Handle jika ada blob (objek)
     if(!(blob_x==0&&blob_y==0)) blobTracking();
     
     //Handle jika tidak ada blob (objek)
     else {
     	setSpeed(0,0);
     }
     
     rate.sleep();
  }

  return 0;
}
