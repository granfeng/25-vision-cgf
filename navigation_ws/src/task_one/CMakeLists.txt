cmake_minimum_required(VERSION 3.5)
project(task_one)

# 查找必要的包
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)

# 添加 C++ 可执行文件
add_executable(odom_to_car_bias src/odom_to_car_bias.cpp)
target_link_libraries(odom_to_car_bias
  ${rclcpp_LIBRARIES}
  ${tf2_ros_LIBRARIES}
  ${geometry_msgs_LIBRARIES}
  ${nav_msgs_LIBRARIES}
)

# 安装目标
install(TARGETS
  odom_to_car_bias
  DESTINATION lib/${PROJECT_NAME}
)

# 声明 Python 包
ament_python_install_package(${PROJECT_NAME})

# 声明依赖
ament_export_dependencies(rclcpp tf2_ros geometry_msgs nav_msgs)

ament_package()