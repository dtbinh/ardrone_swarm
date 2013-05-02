/*
 *  QUT cyphy AR.Drone pose estimation ROS node.
 *  Copyright (C) 2012, CYPHY lab
 *  Inkyu Sa <i.sa@qut.edu.au>
 *
 *  http://wiki.qut.edu.au/display/cyphy
 *
 *
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



#include "ardrone_pose_estimation.h"

pose_esti::pose_esti(ros::NodeHandle nh): nh_(nh),ts(0.005*WINDOW_SIZE)
{
    ROS_INFO ("pose esti creator");
    nav_sub = nh_.subscribe<ardrone_pose_estimation::Navdata> ("ardrone/navdata", 1, &pose_esti::navCallback, this);
	kbdSubscriber = nh_.subscribe ("keycode", 1, &pose_esti::kbdCallback, this);
	takeoff_sub = nh_.subscribe("/ardrone/takeoff", 1, &pose_esti::takeoffCallback,this);
	
	vslam_pose_sub = nh_.subscribe("/vslam/pose", 1, &pose_esti::vslam_poseCallback,this);

	vslam_states_sub = nh_.subscribe("/vslam/states", 1, &pose_esti::vslam_statesCallback,this);

	cout<<"This node name is "<<ros::this_node::getName().c_str()<<endl;
		
	cmd_pub = nh_.advertise<geometry_msgs::Twist>("/pose/cmd_vel", 1);
	cmd_yaw_pub = nh_.advertise<geometry_msgs::Twist>("/pose/yaw_cmd_vel", 1);
	pose_pub = nh_.advertise<ardrone_pose_estimation::Pose>("/ardrone/pose", 1); 
	new_pose_pub = nh_.advertise<ardrone_pose_estimation::Pose>("/ardrone/new_pose", 1); 
	
	goal_pub = nh_.advertise<std_msgs::Float32MultiArray>("/ardrone/goal", 1);
	ptam_init_pub = nh_.advertise<std_msgs::String>("/vslam/cmd", 1);
	init=false;
	world_frame="/world";
	//base_frame ="/base_link";
	imu_frame ="/imu";
	base_kf_frame = "/base_kf";
	cur_pose.x=0;
	cur_pose.y=0;
	cur_pose.z=0;
	
	prev_pose.x=0;
	prev_pose.y=0;
	prev_pose.z=0;

	goal_pose.x=0;
	goal_pose.y=0;
	goal_pose.z=DEFAULT_HEIGHT;
	goal_pose.pitch=0;
	goal_pose.roll=0;
	goal_pose.yaw=0;

	take_off=false;
	pick_takeoff_time=false;
    pick_init_yaw=false;
	init_yaw=0;
	plus_2_minus=false;
	minus_2_plus=false;
	pick_ptam_yaw_init_time=0;
	

	//P,I,D, I_MAX , I_MIN
	pid_z.initPid(1.6873 , 1.0492,0.40858  , 0.01 , -0.01 );
    
	pid_yaw.initPid(0.011 , 0.00058 , 0.01 , 0.005 , -0.005 );
	
	pos_gain[P_GAIN]=0.23;
	pos_gain[I_GAIN]=0.01;
	//pos_gain[D_GAIN]=0.18;
	pos_gain[D_GAIN]=0.26;
	
	pos_gain[I_MAX]=0.5;
	pos_gain[I_MIN]=-0.5;
	
	vel_gain[P_GAIN]=0.2;
	vel_gain[I_GAIN]=0.0;
	vel_gain[D_GAIN]=0.12;
	vel_gain[I_MAX]=0.1;
	vel_gain[I_MIN]=-0.1;
	
	pid_pos_x.initPid(pos_gain[P_GAIN] , pos_gain[I_GAIN] , pos_gain[D_GAIN] , pos_gain[I_MAX] , pos_gain[I_MIN] );
	pid_vel_x.initPid(vel_gain[P_GAIN] , vel_gain[I_GAIN] , vel_gain[D_GAIN] , vel_gain[I_MAX] , vel_gain[I_MIN] );

	pid_pos_y.initPid(pos_gain[P_GAIN] , pos_gain[I_GAIN] , pos_gain[D_GAIN] , pos_gain[I_MAX] , pos_gain[I_MIN] );
	pid_vel_y.initPid(vel_gain[P_GAIN] , vel_gain[I_GAIN] , vel_gain[D_GAIN] , vel_gain[I_MAX] , vel_gain[I_MIN] );

	median_vel(0)=0;
	median_vel(1)=0;
	median_flag=0;

	cout<<"ts = "<<ts<<endl;
	invertable=0;
	ptam_init_commanded=0;
	ptam_init_try_cnt=0;
	vslam_states.data="PTAM_uninitialized";

	dynamic_reconfigure::Server<ardrone_pose_estimation::onlinegainConfig>::CallbackType f = boost::bind(&pose_esti::reconfigureCB,this, _1, _2);
	srv.setCallback(f);
	


	fp_vel=fopen("vel.data","w");
    fprintf(fp_vel,"time,mea_vx,mea_vy,esti_vx,esti_vy,med_vx,med_vy\n");
}


pose_esti::~pose_esti()
{

    ROS_INFO ("Destroying pose esti");
}

double pose_esti::deg2rad(double deg) {
	return deg * (M_PI / 180);
}

double pose_esti::rad2deg(double rad) {
	return rad * (180 / M_PI);	
}

void pose_esti::kbdCallback(const cyphy_kbd_drone::KeyCode& msg)
{
	char data=msg.data;

		 switch(data)
		 {
		 
		      case KEYCODE_SPACEBAR:
				break;
	 	      case KEYCODE_ESC:	 

				cmd_vel.linear.x  = 0;
				cmd_vel.linear.y  = 0;
				cmd_vel.linear.z  = 0;
				cmd_vel.angular.x = 0;
				cmd_vel.angular.y = 0;
				cmd_vel.angular.z = 0;
	 	        break;
				
	 		  case KEYCODE_a:
				break;

			  case 'g':
				goal_pose.z +=HEIGHT_TICK;
				if(goal_pose.z>MAX_HEIGHT) goal_pose.z=MAX_HEIGHT;
				cout<<"Goal Z = "<<goal_pose.z<<endl;
				break;
			  case KEYCODE_q:
 
				break;
			  case 'b':
			  	goal_pose.z-=HEIGHT_TICK;
			  	if(goal_pose.z<MIN_HEIGHT) goal_pose.z=MIN_HEIGHT;
				cout<<"Goal Z = "<<goal_pose.z<<endl;
				break;
				
			  case 'v':

				if(goal_pose.yaw>=MAX_YAW) goal_pose.yaw=MAX_YAW;
				else goal_pose.yaw +=YAW_TICK;
				cout<<"Goal yaw = "<<goal_pose.yaw<<endl;
				
				break;
 
				break;
			  case 'n':
			  	if(goal_pose.yaw<=MIN_YAW) goal_pose.yaw=MIN_YAW;
				else goal_pose.yaw-=YAW_TICK;
				cout<<"Goal yaw = "<<goal_pose.yaw<<endl;
				break;
				
			  case 'i':
			  	ptam_init_commanded=true;
				cout<<"Trying PTAM initialization"<<endl;
			  break;
				
			  case KEYCODE_r:
				break;

			  case KEYCODE_9:
			  	

				break;

			  case KEYCODE_e:

				break;
 			  case KEYCODE_c:

				break;

			  case KEYCODE_t:
				break;

			  case '1':
			  	goal_pose.x=0;
				goal_pose.y=0;
				cout<<"goal x = "<<goal_pose.x<<"  goal y = "<<goal_pose.y<<endl;
			  	break;
			  case '2':
			  	goal_pose.x=0;
				goal_pose.y=-1.0;
				cout<<"goal x = "<<goal_pose.x<<"  goal y = "<<goal_pose.y<<endl;
			  	break;
			  case '3':
			  	goal_pose.x=1.0;
				goal_pose.y=-1.0;
				cout<<"goal x = "<<goal_pose.x<<"  goal y = "<<goal_pose.y<<endl;
			  	break;
			case '4':
			  	goal_pose.x=1.0;
				goal_pose.y=0;
				cout<<"goal x = "<<goal_pose.x<<"  goal y = "<<goal_pose.y<<endl;
			  	break;


			  	
		 }//swich
		 goal_msg.data.push_back(goal_pose.x);
		 goal_msg.data.push_back(goal_pose.y);
		 goal_msg.data.push_back(goal_pose.z);
		 goal_msg.data.push_back(goal_pose.yaw);
		 goal_pub.publish(goal_msg);
		 goal_msg.data.clear();
		 
}

void pose_esti::takeoffCallback(const std_msgs::Empty &msg)
{
	cout<<"Taking off"<<endl;
	take_off=true;
}

void pose_esti::vslam_poseCallback(const geometry_msgs::PoseWithCovarianceStampedPtr &msg)
{
	yaw_now=ros::Time::now();
   ptam_pose = *msg;
	if( pick_ptam_yaw_init_time && (vslam_states.data=="PTAM_initialized"))
	{
		//ROS_INFO("%f %f %f\n",ptam_pose.pose.pose.position.x,ptam_pose.pose.pose.position.y,ptam_pose.pose.pose.position.z);
		tf::Quaternion temp;
		//
		tf::Quaternion q(0,0,0,0);
		tf::quaternionMsgToTF(ptam_pose.pose.pose.orientation, temp);
		tfScalar ptam_yaw, ptam_pitch, ptam_roll;
		tf::Matrix3x3(temp).getRPY(ptam_roll, ptam_pitch, ptam_yaw);

      
		double cmd_temp_yaw;
	    double ptam_yaw_temp = rad2deg(-(ptam_yaw+M_PI/2));
		//double ptam_yaw_temp = rad2deg(ptam_yaw);
		PTAM_Yaw=ptam_yaw_temp;
   
		cmd_temp_yaw=pid_yaw.updatePid(ptam_yaw_temp-goal_pose.yaw,yaw_now-yaw_past);
		cout<<"pid output yaw = "<<cmd_temp_yaw<<endl;
		cmd_yaw.linear.x=0;
		cmd_yaw.linear.y=0;
		cmd_yaw.linear.z=0;
		cmd_yaw.angular.x=0;
		cmd_yaw.angular.y=0;
		cmd_yaw.angular.z=cmd_temp_yaw;
		cmd_yaw_pub.publish(cmd_yaw);
		cout<<"imu yaw = "<<rad2deg(yaw_test)<<endl<<
			  "ptam yaw= "<<ptam_yaw_temp<<endl;
	}
	else
	{
		yaw_past = yaw_now;
		pick_ptam_yaw_init_time=1;
	}
}

void pose_esti::vslam_statesCallback(const std_msgs::String &msg)
{
   //cout<<"vslam_statesCallback "<<msg.data<<endl;
   if(msg.data=="PTAM_initialized")
   {
       vslam_states.data = "PTAM_initialized";
	   //cout<<"PTAM!!!!! initialized"<<endl;
	   goal_pose.z = DEFAULT_HEIGHT;
   }
}




void pose_esti::navCallback(const ardrone_pose_estimation::NavdataConstPtr& nav_msg)
{
   std_msgs::String str;
   navdata = *nav_msg;
   now = ros::Time::now();

   if(ptam_init_commanded && vslam_states.data =="PTAM_uninitialized")
   {
     goal_pose.z+=HEIGHT_TICK;
	 str.data = "ptam_initial";
	 ptam_init_pub.publish(str);
   }

   
   if(take_off&& !pick_takeoff_time)
   {
	   cout<<"Pick takeoff time"<<endl;
	   takeoff_start = now;
	   pick_takeoff_time=true;
	   
   	}

   if(!pick_init_yaw)
   {
	   pick_init_yaw=true;
	   init_yaw = deg2rad(nav_msg->rotZ);
	   kalman.init();
	   cout<<"init_yaw = "<<init_yaw<<endl;
   	}
	   

   double pitch,roll,vx,vy,z,vx_medi,vy_medi,vx_esti,vy_esti;

   if(init)
   {
	    //Radian
	    //yaw = deg2rad(nav_msg->rotZ)-init_yaw;
		cur_measured_yaw = deg2rad(nav_msg->rotZ);
		
		pitch = deg2rad(nav_msg->rotY);
		roll = deg2rad(nav_msg->rotX);
		vx = nav_msg->vx;
		vy = nav_msg->vy;
		z = nav_msg->altd;
		cur_pose.z = nav_msg->altd; //metre
		
		measure_vel(0) = vx;
		measure_vel(1) = vy;
		double comps_yaw;
		ros::Duration nav_ts = now-past;
		ts = nav_ts.toSec();
		//cout<<"ts = "<<ts<<endl;

	    comps_yaw=compensate_yaw(cur_measured_yaw,init_yaw);
		yaw_test=comps_yaw;
		median_vel=kalman.median(measure_vel,&median_flag);
		
		
		cur_pose.x = prev_pose.x + ts*(cos(comps_yaw)*vx-sin(comps_yaw)*vy);  //metre
		cur_pose.y = prev_pose.y + ts*(sin(comps_yaw)*vx+cos(comps_yaw)*vy);  //metre

		
		

	    if(median_flag)
		{
			vx_medi= median_vel(0);
			vy_medi= median_vel(1);

			measure << ptam_pose.pose.pose.position.x,
				       ptam_pose.pose.pose.position.y,
				       median_vel(0),
				       median_vel(1);
			esti=kalman.process(measure,comps_yaw,&invertable);
			
			if(invertable)
			{
				vx_esti=esti(2);
				vy_esti=-esti(3);
				fprintf(fp_vel,"%f,%f,%f,%f,%f,%f,%f\n",median_now.toSec(),vx,vy,esti(2),esti(3),median_vel(0),median_vel(1));
				if(vslam_states.data=="PTAM_initialized") publish_kf_transform();
				
		// Taking off to hovering takes roughly 4 sec after sending 
		// takeoff command.
		
		//if( now.toSec() - takeoff_start.toSec() > 4 && pick_takeoff_time)
		if(1)
		{
			double cmd_temp_z;
			cmd_temp_z=pid_z.updatePid(z-goal_pose.z,now-past);

					////////////////////////////////
					//     Yaw control
					////////////////////////////////


					/////////////////////////////////////////////////////
					//     Horizontal velocity and position control
					/////////////////////////////////////////////////////

					double cmd_temp_vx=0;
					double cmd_temp_x=0;
					double cmd_temp_vy=0;
					double cmd_temp_y=0;
					
					pid_pos_x.setGains(pos_gain[P_GAIN], pos_gain[I_GAIN], pos_gain[D_GAIN], pos_gain[I_MAX], pos_gain[I_MIN]);  //p , i , d, i_max, i_min
					pid_pos_y.setGains(pos_gain[P_GAIN], pos_gain[I_GAIN], pos_gain[D_GAIN], pos_gain[I_MAX], pos_gain[I_MIN]);  //p , i , d, i_max, i_min
					pid_vel_x.setGains(vel_gain[P_GAIN], vel_gain[I_GAIN], vel_gain[D_GAIN], vel_gain[I_MAX], vel_gain[I_MIN]);  //p , i , d, i_max, i_min
					pid_vel_y.setGains(vel_gain[P_GAIN], vel_gain[I_GAIN], vel_gain[D_GAIN], vel_gain[I_MAX], vel_gain[I_MIN]);  //p , i , d, i_max, i_min
					

					cmd_temp_x=pid_pos_x.updatePid(esti(0)-goal_pose.x,now-past);
					cmd_temp_y=pid_pos_y.updatePid(esti(1)-goal_pose.y,now-past);

			
			if(cmd_temp_z >= 1)
				cmd_vel.linear.z=1;
			else if (cmd_temp_z <=-1)
				cmd_vel.linear.z=-1;
			else 
				cmd_vel.linear.z  = cmd_temp_z;
			
			cmd_vel.angular.x = 0;
			cmd_vel.angular.y = 0;
			//cmd_vel.angular.z = 0;

	        //Temporaly disable x,y controllers.
			//cmd_vel.linear.x=0;
			//cmd_vel.linear.y=0;

			if(vslam_states.data=="PTAM_initialized")
			{
				if(cmd_temp_x >= 1)
					cmd_vel.linear.x=1;
				else if (cmd_temp_x <=-1)
					cmd_vel.linear.x=-1;
				else 
				cmd_vel.linear.x=cmd_temp_x;
					


				if(cmd_temp_y >= 1)
					cmd_vel.linear.y=1;
				else if (cmd_temp_y <=-1)
					cmd_vel.linear.y=-1;
				else 
				cmd_vel.linear.y=cmd_temp_y;
			}


			if(vslam_states.data=="PTAM_uninitialized")
			{
				ROS_INFO("HERE2");
				cmd_vel.linear.x=0;
				cmd_vel.linear.y=0;
				cmd_vel.angular.x = 0;
				cmd_vel.angular.y = 0;
				cmd_vel.angular.z = 0;
			}

			cmd_pub.publish(cmd_vel);
		}

		//Before taking off, only sending null commands to the drone.
		else
		{
			ROS_INFO("HERE1");
			cmd_vel.linear.x  =0;
			cmd_vel.linear.y  = 0;
			cmd_vel.angular.x = 0;
			cmd_vel.angular.y = 0;
			cmd_vel.angular.z = 0;
			cmd_pub.publish(cmd_vel);

		}
			
		//rotx(pi)
		tf::Matrix3x3 wRb(1,0,0,
						0,-1,0,
						0,0,-1);

		//
	    tf::Quaternion btq;
		tf::Matrix3x3 temp = wRb.transpose();
		temp.getRotation(btq);
			
		tf::Transform world_to_base;
		world_to_base.setIdentity();
		
		//
		tf::Quaternion q(0,0,0,0);

		//Yaw,Pitch,Roll order
		//q.setEulerZYX(comps_yaw,pitch,roll);	
		q.setEulerZYX(comps_yaw,0,0);	
		
		world_to_base.setRotation(btq*q);

		tf::Vector3 origin;
		origin.setValue (cur_pose.x,
				           cur_pose.y,
				           cur_pose.z);
		world_to_base.setOrigin (origin);

		tf::StampedTransform world_to_base_tf(world_to_base, nav_msg->header.stamp, world_frame, imu_frame);
		tf_broadcaster.sendTransform (world_to_base_tf);

		tf::Vector3 t = world_to_base.getOrigin();
		
		publish_pose(t.x(),t.y(),t.z(),roll,pitch,comps_yaw);
		publish_new_pose(esti(0),esti(1),cur_pose.z,roll,pitch,PTAM_Yaw);
		prev_pose = cur_pose;
		past = now;
			//median_past = median_now;
		past_measured_yaw = cur_measured_yaw;
			median_flag=0;
   } //if(invertable)
		} //if(median_flag)
   } //if(init)

   // At the very first time we don't know the samping time,
   // Skip one frame.
   else 
   {
	   past = now;
	   init=true;
	   past_measured_yaw = deg2rad(nav_msg->rotZ);
   }
   
   
   //ROS_INFO("navCallback");
   

   
}

// All input and output arguments are radian.
double pose_esti::compensate_yaw(double measured_yaw,double init_yaw)
{
		double comp_yaw=0;;
		if((rad2deg(past_measured_yaw)>=179.9) && (rad2deg(measured_yaw)<=-179.9))
		plus_2_minus=true;

		if((rad2deg(past_measured_yaw)<=-179.9) && (rad2deg(measured_yaw)>=179.9))
		minus_2_plus=true;
		
		if(plus_2_minus) comp_yaw=deg2rad(360)+measured_yaw-init_yaw;

		else if(minus_2_plus) comp_yaw=measured_yaw-init_yaw-deg2rad(360);
		else comp_yaw = measured_yaw-init_yaw;
	
		if(rad2deg(comp_yaw)>=179.9) comp_yaw = comp_yaw - deg2rad(360);
		
		else if(rad2deg(comp_yaw)<=-179.9) comp_yaw = deg2rad(360)+comp_yaw ;
		else comp_yaw = comp_yaw;
		
		plus_2_minus=0;
		minus_2_plus=0;
		return comp_yaw;
}

void pose_esti::reconfigureCB(ardrone_pose_estimation::onlinegainConfig &config, uint32_t level)
{
    //cout<<"reconfigure"<<endl;
	pos_gain[P_GAIN]=config.Position_P;
	pos_gain[I_GAIN]=config.Position_I;
	pos_gain[D_GAIN]=config.Position_D;

	vel_gain[P_GAIN]=config.Velocity_P;
	vel_gain[I_GAIN]=config.Velocity_I;
	vel_gain[D_GAIN]=config.Velocity_D;
}

void pose_esti::publish_pose(double x,double y,double z, double roll,double pitch,double yaw)
{
		pose.header.stamp = ros::Time::now();
		pose.x=x;
		pose.y=y;
		pose.z=z;
		pose.roll=deg2rad(roll);
		pose.pitch=deg2rad(pitch);
		pose.yaw=yaw;
		pose_pub.publish(pose);		
}

void pose_esti::publish_new_pose(double x,double y,double z, double roll,double pitch,double yaw)
{
		new_pose.header.stamp = ros::Time::now();
		new_pose.x=x;
		new_pose.y=y;
		new_pose.z=z;
		new_pose.roll=deg2rad(roll);
		new_pose.pitch=deg2rad(pitch);
		new_pose.yaw=deg2rad(yaw);
		new_pose_pub.publish(new_pose);		
}

void pose_esti::publish_kf_transform()
{

	tf::Matrix3x3 wRb(1,0,0,
						0,-1,0,
						0,0,-1);

	//
	tf::Quaternion btq;
	tf::Matrix3x3 mtx = wRb.transpose();
	mtx.getRotation(btq);

	tf::Transform world_to_base_kf;
	world_to_base_kf.setIdentity();
	tf::Quaternion temp;
	//
	tf::Quaternion q(0,0,0,0);
	tf::quaternionMsgToTF(ptam_pose.pose.pose.orientation, temp);

	tfScalar ptam_yaw, ptam_pitch, ptam_roll;
	tf::Matrix3x3(temp).getRPY(ptam_roll, ptam_pitch, ptam_yaw);
	float pitch = deg2rad(navdata.rotX);
	float roll = deg2rad(navdata.rotY);
	q.setEulerZYX(ptam_yaw,pitch,roll);
	
	tf_sub_.lookupTransform("/world","/ptam_fixed",ros::Time(0),worldToPTAMFixedTF);
	world_to_base_kf.setRotation(q);

	tf::Vector3 origin;

	origin.setValue (esti(0),esti(1),navdata.altd);

	world_to_base_kf.setOrigin (origin);
	tf::StampedTransform world_to_base_kf_tf(world_to_base_kf, navdata.header.stamp, world_frame, base_kf_frame);
	tf_broadcaster.sendTransform (world_to_base_kf_tf);
}




