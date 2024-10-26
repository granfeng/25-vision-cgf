#include <rclcpp/rclcpp.hpp>
#include <gazebo_msgs/srv/set_model_state.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <cmath>

class SphereController : public rclcpp::Node
{
public:
  SphereController() : Node("sphere_controller")
  {
    client_ = this->create_client<gazebo_msgs::srv::SetModelState>("/gazebo/set_model_state");                                     // 客户端
    timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&SphereController::timer_callback, this));          // 定时器
  }

private:
  void timer_callback()
  {
    auto request = std::make_shared<gazebo_msgs::srv::SetModelState::Request>();                                                   // 服务的请求对象
    request->model_state.model_name = "my_sphere";
    request->model_state.pose.position.x = 3.0 * std::cos(angle_);
    request->model_state.pose.position.y = 3.0 * std::sin(angle_);                                                                 // 计算球体在圆周上的位置
    request->model_state.pose.position.z = 0.5;                                                                                    // 球体半径
    request->model_state.twist.linear.x = 0.0;
    request->model_state.twist.linear.y = 0.0;
    request->model_state.twist.linear.z = 0.0;
    request->model_state.twist.angular.x = 0.0;
    request->model_state.twist.angular.y = 0.0;
    request->model_state.twist.angular.z = 0.0;

    auto result = client_->async_send_request(request);
    angle_ += 0.01;                                                                                                               // 调整角度
  }

  rclcpp::Client<gazebo_msgs::srv::SetModelState>::SharedPtr client_;
  rclcpp::TimerBase::SharedPtr timer_;
  double angle_ = 0.0;
};

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<SphereController>());
  rclcpp::shutdown();
  return 0;
}