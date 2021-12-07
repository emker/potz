#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/Pose.h>
int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_listener");
  ros::NodeHandle node;
  ros::Publisher turtle_vel;
  int mode=0;
   ros::Duration(1.0).sleep();
   turtle_vel =
    node.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 10);
  tf::TransformListener listener;
  ros::Rate rate(10.0);
  while (node.ok()){
    tf::StampedTransform transform;
    try{
      listener.lookupTransform("/turtle1", "/turtle2",
                               ros::Time(0), transform);
    }
    catch (tf::TransformException &ex) {
      ROS_ERROR("%s",ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }
    geometry_msgs::Twist vel_msg;
    vel_msg.angular.z = 4.0 * atan2(transform.getOrigin().y(),
                                    transform.getOrigin().x());
    vel_msg.linear.x = 0.5 * sqrt(pow(transform.getOrigin().x(), 2) +
                                  pow(transform.getOrigin().y(), 2));
    if ((mode==0)&&(vel_msg.linear.x<0.5)) {
       int i;
       for (i=0;i<10;i++) 
           rate.sleep();
     mode=1;      
     }
    if (mode==1) 
       turtle_vel.publish(vel_msg); 
    rate.sleep();
  }
  return 0;
};
