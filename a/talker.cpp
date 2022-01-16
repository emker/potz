#include <ros/ros.h>
#include <turtlesim/Pose.h>
#include <turtlesim/Spawn.h>
#include <turtlesim/TeleportAbsolute.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <geometry_msgs/Twist.h>
using namespace std;
  
ros::Time inittime;
std::vector<float> timevector={};
std::vector<float> xvector={};
std::vector<float> yvector={};
std::vector<float> thetavector={};
ros::ServiceClient gotopoint;
int pn=0;
bool ok=true;
ros::Publisher turtle_vel;


void poseCallback(const turtlesim::PoseConstPtr& msg){
 if (!timevector.empty()&ok) {
  float xx,yy,tt,time,theta;
  tt=timevector.at(pn);
  time=(ros::Time::now()-inittime).toSec();
  while ((time>tt)&(pn<timevector.size())) {
  pn=pn+1;
  if (pn<timevector.size())
     tt=timevector.at(pn);
  }
  if (pn>=timevector.size()) {
     ok=false;
  }
  if (ok) {
  xx=xvector.at(pn);
  yy=yvector.at(pn);
  theta=thetavector.at(pn);
  ros::service::waitForService("/turtle1/teleport_absolute");

  turtlesim::TeleportAbsolute srv;
  srv.request.x=xx;
  srv.request.y=yy;
  srv.request.theta=theta;
  gotopoint.call(srv);

  }
  }
}
int main(int argc, char** argv){
  ros::init(argc, argv, "talker");
 ros::NodeHandle node;

   ros::Duration(1.0).sleep();  
  string line;
  ifstream myfile ("/tmp/route.txt");
  if (myfile.is_open()) {
   while (getline(myfile,line)) {
      std::stringstream ssline(line);
      float tt,xx,yy,theta;
      ssline >> tt >> xx >> yy >> theta;
      timevector.push_back(tt);
      xvector.push_back(xx);
      yvector.push_back(yy);
      thetavector.push_back(theta);
     }
   }
   myfile.close();  
      turtle_vel =
    node.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 10);
  ros::service::waitForService("/turtle1/teleport_absolute");
 gotopoint =
    node.serviceClient<turtlesim::TeleportAbsolute>("/turtle1/teleport_absolute");
  ros::Subscriber sub = node.subscribe("/turtle1/pose", 10, &poseCallback);
  inittime=ros::Time::now();
  ros::spin();
  return 0;
};
