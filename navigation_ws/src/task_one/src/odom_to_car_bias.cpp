#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>

class OdomToCarBias : public rclcpp::Node
{
public:
  OdomToCarBias() : Node("odom_to_car_bias")
  {
    tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

    odom_sub_ = this->create_subscription<nav_msgs::msg::Odometry>(
      "/odom", 10, std::bind(&OdomToCarBias::odom_callback, this, std::placeholders::_1));
  }

private:
  void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
  {
    geometry_msgs::msg::TransformStamped transform_stamped;

    try
    {
      transform_stamped = tf_buffer_->lookupTransform("car_bias", msg->header.frame_id, msg->header.stamp);
    }
    catch (tf2::TransformException &ex)
    {
      RCLCPP_WARN(this->get_logger(), "%s", ex.what());
      return;
    }

    geometry_msgs::msg::PoseStamped odom_pose;
    odom_pose.header = msg->header;
    odom_pose.pose = msg->pose.pose;

    geometry_msgs::msg::PoseStamped car_bias_pose;
    tf2::doTransform(odom_pose, car_bias_pose, transform_stamped);

    RCLCPP_INFO(this->get_logger(), "Odom in car_bias frame: x=%f, y=%f, z=%f",
                car_bias_pose.pose.position.x, car_bias_pose.pose.position.y, car_bias_pose.pose.position.z);
  }

  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<OdomToCarBias>());
  rclcpp::shutdown();
  return 0;
}