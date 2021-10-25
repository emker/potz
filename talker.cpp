#include "ros/ros.h"
#include "std_msgs/String.h"
#include "laba2/Message1.h"
#include <sstream>
#include <random>

int main(int argc, char **argv)
{
  std::random_device rnd;
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<laba2::Message1>("chatter", 1000);
  ros::Rate loop_rate(1);
  int count = 0;
  while (ros::ok())
  {
    laba2::Message1 msg;
    std::stringstream ss;
    int x,y;
    x=rnd();
    y=rnd();
    ss << x <<","<<y;
    msg.coord = ss.str();
    ROS_INFO("%s", msg.coord.c_str());
    chatter_pub.publish(msg);
    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}
