#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <gazebo_msgs/msg/model_states.hpp>
#include <cmath>

// 定义 CarController 类，继承自 rclcpp::Node
class CarController : public rclcpp::Node
{
public:
  // 构造函数，初始化节点名称和目标位置
  CarController() : Node("car_controller"), target_x_(3.0), speed_(0.5)
  {
    // 声明速度参数，默认值为 0.5
    this->declare_parameter("speed", 0.5);
    speed_ = this->get_parameter("speed").as_double();

    // 注册参数回调函数，当参数发生变化时自动调用
    param_callback_handle_ = this->add_on_set_parameters_callback(
      std::bind(&CarController::param_callback, this, std::placeholders::_1));

    // 创建发布者，用于发布控制命令
    cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);

    // 创建订阅者，订阅里程计消息
    odom_sub_ = this->create_subscription<nav_msgs::msg::Odometry>(
      "/odom", 10, std::bind(&CarController::odom_callback, this, std::placeholders::_1));

    // 创建订阅者，订阅模型状态消息
    model_states_sub_ = this->create_subscription<gazebo_msgs::msg::ModelStates>(
      "/gazebo/model_states", 10, std::bind(&CarController::model_states_callback, this, std::placeholders::_1));
  }

private:
  // 里程计回调函数，更新当前位置
  void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
  {
    current_x_ = msg->pose.pose.position.x;
    current_y_ = msg->pose.pose.position.y;
  }

  // 模型状态回调函数，更新球体位置
  void model_states_callback(const gazebo_msgs::msg::ModelStates::SharedPtr msg)
  {
    // 遍历模型名称，找到球体的位置
    for (size_t i = 0; i < msg->name.size(); ++i)
    {
      if (msg->name[i] == "my_sphere")
      {
        sphere_x_ = msg->pose[i].position.x;
        sphere_y_ = msg->pose[i].position.y;
        break;
      }
    }

    // 创建控制命令消息
    geometry_msgs::msg::Twist cmd_vel;

    // 如果当前位置与目标位置的差距大于 0.1，则设置线速度
    if (std::abs(current_x_ - target_x_) > 0.1)
    {
      cmd_vel.linear.x = speed_;
    }
    else
    {
      cmd_vel.linear.x = 0.0;
    }

    // 如果球体与车体的距离小于 1m，则将目标位置设置为原点
    if (std::sqrt(std::pow(current_x_ - sphere_x_, 2) + std::pow(current_y_ - sphere_y_, 2)) < 1.0)
    {
      target_x_ = 0.0;
    }

    // 发布控制命令
    cmd_vel_pub_->publish(cmd_vel);
  }

  // 参数回调函数，当参数发生变化时自动调用
  rcl_interfaces::msg::SetParametersResult param_callback(const std::vector<rclcpp::Parameter> &parameters)
  {
    auto result = rcl_interfaces::msg::SetParametersResult();
    result.successful = true;

    // 遍历参数列表，更新速度参数
    for (const auto &param : parameters)
    {
      if (param.get_name() == "speed")
      {
        speed_ = param.as_double();
        RCLCPP_INFO(this->get_logger(), "Speed changed to: %f", speed_);
      }
    }

    return result;
  }

  // 发布者，用于发布控制命令
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;

  // 订阅者，用于订阅里程计消息
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;

  // 订阅者，用于订阅模型状态消息
  rclcpp::Subscription<gazebo_msgs::msg::ModelStates>::SharedPtr model_states_sub_;

  // 参数回调句柄
  rclcpp::node_interfaces::OnSetParametersCallbackHandle::SharedPtr param_callback_handle_;

  // 当前位置
  double current_x_ = 0.0;
  double current_y_ = 0.0;

  // 球体位置
  double sphere_x_ = 0.0;
  double sphere_y_ = 0.0;

  // 目标位置
  double target_x_;

  // 速度参数
  double speed_;
};

// 主函数
int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<CarController>());
  rclcpp::shutdown();
  return 0;
}