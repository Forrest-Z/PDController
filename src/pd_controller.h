#include <ros/ros.h>
#include <tf2_ros/buffer.h>
#include <nav_core/base_local_planner.h>
#include <costmap_2d/costmap_2d_ros.h>
#include "pid_implementation.h"
#include "smoother.h"
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/Float64.h>
#include <base_local_planner/trajectory_planner_ros.h>
#include <dynamic_reconfigure/server.h>

#include <pd_controller/PDControllerConfig.h>





namespace pd_controller{
    class PDController : public nav_core::BaseLocalPlanner{
        public:
        PDController();
        ~PDController();
        void initialize(std::string name, tf2_ros::Buffer* tf , costmap_2d::Costmap2DROS* costmap_ros);
        bool isGoalReached();
        bool setPlan(const std::vector<geometry_msgs::PoseStamped>& global_plan);
        bool computeVelocityCommands(geometry_msgs::Twist& cmd_vel);


        private:
        void reconfigureCB(pd_controller::PDControllerConfig &config , uint32_t level);
        double check_yaw(geometry_msgs::PoseStamped pose);
        double check_yaw_robot(geometry_msgs::PoseWithCovarianceStamped pose);
        double check_desirable_rotation(double desired_yaw ,double yaw);
        void send_command_vel(geometry_msgs::Twist& cmd_vel, double f_vel ,double rot_vel);
        void amclCallback(const geometry_msgs::PoseWithCovarianceStamped msg);

         tf2_ros::Buffer* tf_;
         ros::Time goal_reached_time;
         costmap_2d::Costmap2DROS *costmap_ros_;
         base_local_planner::TrajectoryPlannerROS collision_planner_;
         geometry_msgs::PoseStamped goal;
         geometry_msgs::PoseStamped robot_pose;
         _Smoother vs;
        dynamic_reconfigure::Server<pd_controller::PDControllerConfig> *dsrv_;

        ros::Publisher omega_pub;
        ros::Publisher error_pub;

         bool goal_reached = false;
         bool stopped= false;

         bool collision_flag;
         bool latch_distance;
         bool xy_latch_distance;
         double vel_forward;
         double vel_rot;
         bool rotate_to_goal;
         double linear_tolerance;
         double angular_tolerance;
         double k_p, k_i,k_d;

    };
};
