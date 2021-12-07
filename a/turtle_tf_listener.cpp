#include <ros/ros.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/Pose.h>
#include <random>
  float xx0,yy0;
    ros::Publisher turtle_vel;
void poseCallback(const turtlesim::PoseConstPtr& msg){
   float vx=0;
   float vz=1.0*atan2(yy0-(msg->y),xx0-(msg->x))-msg->theta;
         geometry_msgs::Twist vel_msg;
   if ((vz*vz)<0.0001)  vx=0.5*sqrt((yy0-(msg->y))*(yy0-(msg->y))+(xx0-(msg->x))*(xx0-(msg->x)));   
      vel_msg.angular.z=vz;
      vel_msg.linear.x=vx;
      turtle_vel.publish(vel_msg);     
}
int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_listener");
  std::random_device seed_gen;
  std::default_random_engine engine(seed_gen());
  std::uniform_real_distribution<> rnd(1,9);
  ros::NodeHandle node;
  ros::Subscriber sub;
  int mode=0;
  xx0=rnd(engine);
  yy0=rnd(engine);
  
  ros::service::waitForService("spawn");
  ros::ServiceClient add_turtle =
    node.serviceClient<turtlesim::Spawn>("spawn");
  turtlesim::Spawn srv;
  srv.request.x=xx0;
  srv.request.y=yy0;
  srv.request.name="turtle2";
  add_turtle.call(srv);
   turtle_vel =
    node.advertise<geometry_msgs::Twist>("turtle2/cmd_vel", 10);
  tf::TransformListener listener;
  ros::Rate rate(10.0);
  while (node.ok()){
    if (mode==0) {
    tf::StampedTransform transform;
    try{
      listener.lookupTransform("/turtle2", "/turtle1",
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
    vel_msg.linear.x = 0.6* sqrt(pow(transform.getOrigin().x(), 2) +
                                  pow(transform.getOrigin().y(), 2));
    if (vel_msg.linear.x<0.5) {
       mode=1;
       sub= node.subscribe("/turtle2/pose", 30, &poseCallback);  
    }
    turtle_vel.publish(vel_msg);
    rate.sleep();
    } else {
     ros::spin(); 
    }
    rate.sleep();
  }
  return 0;
};
