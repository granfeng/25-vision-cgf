from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        # 启动 Gazebo 服务器
        Node(
            package='gazebo_ros',
            executable='gzserver',
            arguments=['-s', 'libgazebo_ros_init.so', '-s', 'libgazebo_ros_factory.so'],
            output='screen'
        ),

        # 启动 Gazebo 客户端
        Node(
            package='gazebo_ros',
            executable='gzclient',
            output='screen'
        ),

        # 加载机器人模型
        Node(
            package='gazebo_ros',
            executable='spawn_entity.py',
            arguments=[
                '-entity', 'bot_xiaoche',
                '-file', '/home/chen/Task/navigation_ws/install/task_one/share/task_one/urdf/xiaoche.urdf',
                '--ros-args'
            ],
            output='screen'
        )
    ])