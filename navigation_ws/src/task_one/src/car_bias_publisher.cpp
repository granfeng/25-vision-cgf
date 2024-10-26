#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/static_transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2/LinearMath/Quaternion.h>

class CarBiasPublisher : public rclcpp::Node
{
public:
  CarBiasPublisher() : Node("car_bias_publisher")
  {
    tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
    tf_broadcaster_ = std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);

    timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&CarBiasPublisher::timer_callback, this));
  }

private:
  void timer_callback()
  {
    geometry_msgs::msg::TransformStamped transform_stamped;

    try
    {
      transform_stamped = tf_buffer_->lookupTransform("map_bias", "base_link", rclcpp::Time(0));
    }
    catch (tf2::TransformException &ex)
    {
      RCLCPP_WARN(this->get_logger(), "%s", ex.what());
      return;
    }

    geometry_msgs::msg::TransformStamped car_bias_transform;
    car_bias_transform.header.stamp = this->get_clock()->now();
    car_bias_transform.header.frame_id = "map_bias";
    car_bias_transform.child_frame_id = "car_bias";

    car_bias_transform.transform.translation.x = -transform_stamped.transform.translation.x;
    car_bias_transform.transform.translation.y = -transform_stamped.transform.translation.y;
    car_bias_transform.transform.translation.z = -transform_stamped.transform.translation.z;

    car_bias_transform.transform.rotation = transform_stamped.transform.rotation;

    tf_broadcaster_->sendTransform(car_bias_transform);
  }

  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
  std::shared_ptr<tf2_ros::StaticTransformBroadcaster> tf_broadcaster_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CarBiasPublisher>());
  rclcpp::shutdown();
  return 0;
}