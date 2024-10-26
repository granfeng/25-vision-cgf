import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments=['0', '0', '0', '0', '0', '0', 'map', 'odom'],
            output='screen'
        ),
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            arguments=['2', '3', '0', '1.57079632679', '0', '0', 'map', 'map_bias'],
            output='screen'
        ),
        Node(
            package='rviz2',
            executable='rviz2',
            arguments=['-d', os.path.join(get_package_share_directory('task_one'), 'config', 'tf_demo.rviz')],
            output='screen'
        )
    ])