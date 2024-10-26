cmake_minimum_required(VERSION 3.5)
project(task_three)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# 查找 ROS 2 依赖项
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(PCL 1.8 REQUIRED COMPONENTS common io filters)

# 添加可执行文件
add_executable(task_three_node src/task_three_node.cpp)

# 链接库
target_link_libraries(task_three_node
  ${PCL_LIBRARIES}
  rclcpp::rclcpp
  sensor_msgs::sensor_msgs
  pcl_conversions::pcl_conversions
)

# 安装可执行文件
install(TARGETS task_three_node
  DESTINATION lib/${PROJECT_NAME})

# 安装包
ament_package()