#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Empty.h"
#include "sensor_msgs/Image.h"

#include "common/CRawImage.h"

#include "control/app.h"
#include "common/CRawImage.h"
#include "control/CImageClient.h"

#include "ar_drone2/Navdata.h"


bool landed;
int imageWidth,imageHeight;

float saturation(float a,float b)
{
	if (a > b) return b;
	if (a < -b) return -b;
	return a;
}

//--------------callbacks section -----------------------


void cmdCallback(const geometry_msgs::Twist &msg){
   int32_t yaw,pitch,roll,height;

   yaw = saturation(msg.angular.x,33000);
   roll = saturation(msg.angular.y,33000);
   pitch = saturation(msg.angular.z,33000);
   height = saturation(msg.linear.z,33000);

   //	fprintf(stdout,"Angle request: %d %d %d %d ",pitch,roll,height,yaw);
   at_set_radiogp_input(roll,pitch,height,yaw);

}

void takeoffCallback(const  std_msgs::Empty &msg){
   if (landed){
      usleep(100000);
      at_ui_reset();
      usleep(200000);
      at_trim();
      usleep(200000);
      fprintf(stdout,"Taking off");
      at_ui_pad_start_pressed();
      usleep(100000);
      at_comwdg();
      landed = false;
   }

}

void landingCallback(const std_msgs::Empty  &msg){
   if (landed ==false){
      usleep(100000);
      at_ui_pad_start_pressed();
      usleep(100000);
      landed = true;
   }

}

void resetCallback(const std_msgs::Empty &msg){
}



int main(int argc, char **argv)
{
   ros::init(argc, argv, "ar_drone2");

   ros::NodeHandle n;

   landed = true;
   imageWidth = 640;
   imageHeight = 368;

   //establishing connection with ar_drone2
   appInit();
   CImageClient *client = new CImageClient();
   client->connectServer(WIFI_MYKONOS_IP,"5555");
   CRawImage *image = new CRawImage(imageWidth,imageHeight);
   client->run(image);

   ros::Subscriber sub0 = n.subscribe("cmd_vel", 1000, cmdCallback);
   ros::Subscriber sub1 = n.subscribe("takeoff", 1000, takeoffCallback);
   ros::Subscriber sub2 = n.subscribe("landing", 1000, landingCallback);
   ros::Subscriber sub3 = n.subscribe("reset", 1000, resetCallback);
   ros::Publisher img_publisher = n.advertise<sensor_msgs::Image>("image_raw",10);
   client->setPublisher(&img_publisher);
   ros::Publisher navdata_publisher = n.advertise<ar_drone2::Navdata>("navdata",10);
   nav_publisher = & navdata_publisher;
      ros::spin();
   appDeinit();
   return 0;
}
