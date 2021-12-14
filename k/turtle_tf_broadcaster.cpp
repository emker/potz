#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <turtlesim/Pose.h>
#include <turtlesim/Spawn.h>
#include <random>
std::string turtle_name;
void poseCallback(const turtlesim::PoseConstPtr& msg){
  static tf::TransformBroadcaster br;
  tf::Transform transform;
  transform.setOrigin( tf::Vector3(msg->x, msg->y, 0.0) );
  tf::Quaternion q;
  q.setRPY(0, 0, msg->theta);
  transform.setRotation(q);
  br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", turtle_name));
}
int main(int argc, char** argv){
  ros::init(argc, argv, "my_tf_broadcaster");
  if (argc != 2){ROS_ERROR("need turtle name as argument"); return -1;};
  turtle_name = argv[1];
   ros::NodeHandle node;
   ros::Duration(4.0).sleep();  
  if (!strcmp(argv[1],"/turtle1")) {
    std::random_device seed_gen;
  std::default_random_engine engine(seed_gen());
  std::uniform_real_distribution<> rnd(1,9);
  float a0=(float)rnd(engine);
  ros::service::waitForService("spawn");
  ros::ServiceClient add_turtle =
    node.serviceClient<turtlesim::Spawn>("spawn");
  turtlesim::Spawn srv;
  srv.request.x=rnd(engine);
  srv.request.y=rnd(engine);

  srv.request.name="turtle1";
  add_turtle.call(srv);
  }  
  ros::Subscriber sub = node.subscribe(turtle_name+"/pose", 10, &poseCallback);
  ros::spin();
  return 0;
};
