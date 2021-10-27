#include "ros/ros.h"
#include "laba2/Message1.h"
#include <vector>

void chatterCallback(const laba2::Message1::ConstPtr& msg)
{
  std::stringstream ss(msg->coord.c_str());
  std::stringstream output;
  std::string koord;
  int x=0;
  int y=0;
  int z=0;
  int n=0;
  int i;
  while (std::getline(ss,koord,',')) {
   if (n==0) {
     x=std::stoi(koord);
    }
   else if (n==1) {
      y=std::stoi(koord);
   } else if (n==2) {
      z=std::stoi(koord);
   }
  n++;
  }
  output<<std::string(y+1,'\n')<<std::string(x,' ')<<"*\n";
  output<<std::string(x,' ')<<"(%d,%d)"<<std::string(23-y,'\n');
  ROS_INFO(output.str().c_str(), x,y);
}

int main(int argc, char **argv)
{

 ros::init(argc, argv, "listener");
 ros::NodeHandle n;
 ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);

 ros::spin();

  return 0;
}

