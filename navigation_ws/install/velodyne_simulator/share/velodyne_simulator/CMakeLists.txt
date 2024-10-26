cmake_minimum_required(VERSION 3.5)
project(velodyne_simulator)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(gazebo_ros REQUIRED)
find_package(gazebo_dev REQUIRED)

install(FILES package.xml
  DESTINATION share/${PROJECT_NAME})

install(FILES CMakeLists.txt
  DESTINATION share/${PROJECT_NAME})

install(CODE "execute_process(COMMAND ${ament_cmake_python_executable} -m ament_index_python.packages --install-dir ${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/packages --package-name ${PROJECT_NAME})")

ament_export_dependencies(rclcpp sensor_msgs tf2_ros gazebo_ros gazebo_dev)

ament_package()