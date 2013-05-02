#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "std_msgs/Empty.h"
#include <SDL/SDL.h>

typedef struct{
	int axis[6];
	bool buttons[11];
}TJoyState;


int main(int argc, char **argv)
{
   SDL_Joystick* joystick;
   TJoyState joy,lastJoy;
   float pitch,roll,yaw,height=0;
   ros::init(argc, argv, "ar_drone_teleop");

   ros::NodeHandle n;
   ros::Publisher cmd_publisher = n.advertise<geometry_msgs::Twist>("cmd_vel",10);
   ros::Publisher tk_publisher = n.advertise<std_msgs::Empty>("takeoff",10);
   ros::Publisher ln_publisher = n.advertise<std_msgs::Empty>("landing",10);
   std_msgs::Empty empty;
   SDL_Init(SDL_INIT_JOYSTICK);
	joystick = SDL_JoystickOpen(0);

	fprintf(stdout,"Joystick with %i axes, %i buttons and %i hats initialized.\n",SDL_JoystickNumAxes(joystick),SDL_JoystickNumButtons(joystick),SDL_JoystickNumHats(joystick));
        ros::Rate loop_rate(10);
        while (ros::ok()) {
      SDL_JoystickUpdate();
      for (int i = 0;i<6;i++){
         joy.axis[i] = SDL_JoystickGetAxis (joystick, i);
         if (fabs(joy.axis[i]) < 20) joy.axis[i] = 0;
      }
      for (int i = 0;i<11;i++){
         joy.buttons[i+1] =  SDL_JoystickGetButton (joystick,i);
      }
      if (joy.buttons[6]){
         roll	= joy.axis[2];	
         pitch 	= joy.axis[0];
         yaw 	= joy.axis[1];
         height = joy.axis[3];
      }
      if (joy.buttons[5] && lastJoy.buttons[5] == false) tk_publisher.publish(empty);
      if (joy.buttons[7] && lastJoy.buttons[7] == false) ln_publisher.publish(empty);
      //for (int i = 1;i<5;i++){
      //  if (joy.buttons[i] && lastJoy.buttons[i]==false) heli->switchCamera(i-1);
      //  }
      lastJoy = joy;
      geometry_msgs::Twist msg;
      msg.angular.x = roll;
      msg.angular.y = pitch;
      msg.angular.z = yaw;
      msg.linear.z = height;
      cmd_publisher.publish(msg);
      ros::spinOnce();
      loop_rate.sleep();
   }
}


