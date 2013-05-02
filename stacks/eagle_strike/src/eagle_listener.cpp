#include "ros/ros.h"
#include "cmvision/Blobs.h"
#include "cmvision/Blob.h"
#include "ardrone_autonomy/Navdata.h"
#include "std_msgs/Empty.h"
#include "geometry_msgs/Twist.h"
#include <algorithm>

/** Konstanta Kp untuk Hover. */
#define KPX 0.006
#define KPY 0.006

/** Konstanta Kp untuk Blob. */
#define KPblobX 0.002
#define KPblobY 0.002

/** Titik tengah kamera bawah. */
#define CENTERX 88
#define CENTERY 72


/** Ukuran kamera bawah: 176 x 144 pixels.
  * Titik tengah kamera bawah: (88, 72)
  * 
  * Ukuran kamera depan: 640 x 480 pixels.
  * 
  * Kita akan menggunakan perbedaan titik tengah kamera bawah dengan 
  * titik blob untuk memandu navigasi. Setelah target ketemu, ARDrone hovering.
  * 
  * Kelompok IV (SandalTerbang) Robotika 2012.
  * 
  */

int blobX = 0;
int blobY = 0;
float vx = 0;
float vy = 0;
bool blobExist = false;
int distanceX = 0;
int distanceY = 0;

/**
  * Fungsi untuk mengembalikan data blob yang terdeteksi.
  * Blob: elemen warna. Lihat cmvision/color_spec.txt.
  */
void blobCallback(const cmvision::Blobs::ConstPtr& blobResult)
{
	std::vector<cmvision::Blob> blobData = blobResult->blobs;
	std::vector<cmvision::Blob>::size_type size = blobData.size();
	
	if (size > 0) {
		::blobExist = true;
		cmvision::Blob targetBlob = blobData.at(0);
		::blobX = targetBlob.x;
		::blobY = targetBlob.y;
		
		::distanceX = CENTERX - targetBlob.x;
		::distanceY = CENTERY - targetBlob.y;
		
		//~ ROS_INFO("Perbedaan jarak x: [%d]", ::distanceX);
		//~ ROS_INFO("Perbedaan jarak y: [%d]", ::distanceY);
	} else {
		::blobExist = false;
	}
}

/**
  * Fungsi untuk mengembalikan data navigasi ARDrone.
  */
void navdataCallback(const ardrone_brown::Navdata::ConstPtr& navigationData)
{
	::vx = navigationData->vx / 10;
	::vy = navigationData->vy / 10;
}

/** Hanya menggunakan P dan konsep PID dasar. */
float computePIDX(float v_des, float v_act) {
	
	float e_func = v_des - v_act;
	float r_mot = KPX * (e_func);
	r_mot = std::min(r_mot, 0.5f);
	r_mot = std::max(r_mot, -0.5f);
	ROS_INFO("RMOTX = %f", r_mot);
	return r_mot;
}

/** Hanya menggunakan P dan konsep PID dasar. */
float computePIDY(float v_des, float v_act) {
	
	float e_func = v_des - v_act;
	float r_mot = KPY * (e_func);
	r_mot = std::min(r_mot, 0.5f);
	r_mot = std::max(r_mot, -0.5f);
	ROS_INFO("RMOTY = %f", r_mot);
	return r_mot;
}

/** Hanya menggunakan P dan konsep PID dasar. */
float computePIDblobX(float v_des, float v_act) {
	
	float e_func = v_des - v_act;
	float r_mot = KPblobX * (e_func);
	
	ROS_INFO("\nX, Blob, err = %f, %f, %f", v_des, v_act, e_func);
	
	r_mot = std::min(r_mot, 0.5f);
	r_mot = std::max(r_mot, -0.5f);
	
	ROS_INFO("RMOTX = %f", r_mot);
	return r_mot;
}

/** Hanya menggunakan P dan konsep PID dasar. */
float computePIDblobY(float v_des, float v_act) {
	
	float e_func = v_des - v_act;
	float r_mot = KPblobY * (e_func);
	
	ROS_INFO("\nX, Blob, err = %f, %f, %f", v_des, v_act, e_func);
	
	r_mot = std::min(r_mot, 0.5f);
	r_mot = std::max(r_mot, -0.5f);
	
	ROS_INFO("RMOTY = %f", r_mot);
	return r_mot;
}


/**
  * Fungsi utama untuk menjalankan node eagle_listener.
  */
int main(int argc, char **argv)
{

	/**
	  * Inisialisasi node baru yang bernama "eagle_listener" dan terhubung 
	  * dengan argumen command-line.
	  */
	ros::init(argc, argv, "eagle_listener");

	/**
	 * Inisialisasi node master untuk menghubungkan node eagle_listener 
	 * dengan node lainnya yang terhubung dengan ROS.
	 */
	ros::NodeHandle n;

	/** Subscribe untuk mendapatkan data pendeteksian warna. */
	ros::Subscriber blobSub = n.subscribe("/blobs", 10, blobCallback);
	/** Subscribe untuk mendapatkan data navigasi ARDrone. */
	ros::Subscriber navdataSub = n.subscribe("/ardrone/navdata", 10, navdataCallback);
	
	/** Publish untuk membuat ARDrone takeoff. */
	ros::Publisher takeoff = n.advertise<std_msgs::Empty>("/ardrone/takeoff", 10);
	/** Publish untuk membuat ARDrone bergerak sesuai instruksi. */
	ros::Publisher pilot = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
	
	/** Prosedur takeoff untuk ARDrone. */
	ROS_INFO("Takeoff will commence soon!");
	ROS_INFO("Beware of people nearby ARDrone !!!");
	std_msgs::Empty empty;
	takeoff.publish(empty);
	
	/** 4 detik adalah waktu yang dibutuhkan oleh AR Drone untuk hovering. */
	ros::Duration t(4);
	t.sleep();
	
	/** Takeoff dilakukan beberapa kali agar pesan sampai kepada ARDrone. */
	takeoff.publish(empty);	
	takeoff.publish(empty);	
	takeoff.publish(empty);	
	takeoff.publish(empty);	
	takeoff.publish(empty);	
	
	/** Variabel yang akan berisi instruksi utama untuk menggerakkan ARDrone. */
	geometry_msgs::Twist instruksi;
	
	ros::Rate r(5);
	ros::Duration q(6);
	q.sleep();
	ROS_INFO("End of sleep.");
	
	/** 
	  * Pengaturan sudut ARDrone agar menghasilkan kecepatan yang sesuai 
	  * ketika mendekati blob.
	  */
	
	bool refresh = false;
	
	/** Loop untuk menentukan instruksi yang akan menggerakkan ARDrone. */
	
	while (ros::ok()) {
		
		instruksi.angular.x = 0;
		instruksi.angular.y = 0;
		instruksi.angular.z = 0;
		instruksi.linear.z = 0;
		
		/** Jika blob tidak ada atau ARDRone memasuki area */
		if (!::blobExist) {
			
			/** PID untuk mempertahankan posisi. */
			ROS_INFO("Tidak ada blob, diam.");
			
			/** Menggunakan PID dengan parameter kecepatan. (hover) */
			instruksi.linear.x = computePIDX(0.0, ::vx);
			instruksi.linear.y = computePIDY(0.0, ::vy);
			
		} else {
			
			/** PID untuk memperkecil jarak. */
			ROS_INFO("Mendekati posisi blob... ");
			
			/** Menggunakan PID dengan parameter posisi. (mendekati target) */
			instruksi.linear.x = computePIDblobX(CENTERY, ::blobY);
			instruksi.linear.y = computePIDblobY(CENTERX, ::blobX);
			
		}
		
		/** Publish instruksi (mengirim) ke ARDrone. */
		pilot.publish(instruksi);
		
		/** Pemanggilan callback untuk mengambil data dari topik yang disubscribe. */
		ros::spinOnce();
		ros::spinOnce();
		ros::spinOnce();
		
	}
	
	return 0;
}