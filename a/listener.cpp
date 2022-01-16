#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/Pose.h>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
ofstream myfile;

float px=0;
float py=0;
float ptheta=0;
float epsilon=0.01;
ros::Time inittime;

void poseCallback(const turtlesim::PoseConstPtr& msg){

 if (abs(msg->x-px)+abs(msg->y-py)+abs(msg->theta-ptheta)>epsilon) {
     myfile << (ros::Time::now()-inittime) << " " << msg->x << " " << msg->y << " " << msg->theta << "\n";
     px=msg->x;
     py=msg->y;
     ptheta=msg->theta;
     }
  
}

int main(int argc, char **argv)
{
 ros::init(argc, argv, "listener");
 if (argc>1) {
 epsilon=atof(argv[1]);
 ROS_INFO("%f\n",epsilon);
 }
 ros::NodeHandle n;
 myfile.open("/tmp/route.txt",ios::out);
 ros::Subscriber sub = n.subscribe("/turtle1/pose", 10, &poseCallback);
 inittime=ros::Time::now();
 ros::spin();
 myfile.close();
 return 0;
}
