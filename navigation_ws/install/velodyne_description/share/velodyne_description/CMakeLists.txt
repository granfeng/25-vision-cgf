cmake_minimum_required(VERSION 3.5)
project(velodyne_description)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(urdf REQUIRED)
find_package(xacro REQUIRED)

# 安装 URDF 文件
install(DIRECTORY urdf/
  DESTINATION share/${PROJECT_NAME}/urdf)

# 安装 launch 文件
install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch)

# 安装资源文件
install(DIRECTORY meshes/
  DESTINATION share/${PROJECT_NAME}/meshes)

# 安装 package.xml 文件
install(FILES package.xml
  DESTINATION share/${PROJECT_NAME})

# 安装 CMakeLists.txt 文件
install(FILES CMakeLists.txt
  DESTINATION share/${PROJECT_NAME})

# 安装资源索引
install(CODE "execute_process(COMMAND ${ament_cmake_python_executable} -m ament_index_python.packages --install-dir ${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/packages --package-name ${PROJECT_NAME})")

# 安装依赖项
ament_export_dependencies(rclcpp sensor_msgs tf2_ros)

ament_package()