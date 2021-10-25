#include "ros/ros.h"
#include "std_msgs/String.h"
#include "laba2/Message1.h"
#include <vector>

void chatterCallback(const laba2::Message1::ConstPtr& msg)
{
  std::stringstream ss(msg->coord.c_str());
  std::string c1;

  int x=0;
  int y=0;
  int n=0;
  while (std::getline(ss,c1,',')) {
  if (n==0) x=std::stoi(c1);
  else if (n==1) y=std::stoi(c1);
  n++;
  }
  ROS_INFO("Coordinates: %d; %d", x,y );
}

int main(int argc, char **argv)
{

 ros::init(argc, argv, "listener");
 ros::NodeHandle n;
 ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);

  ros::spin();

  return 0;
}

