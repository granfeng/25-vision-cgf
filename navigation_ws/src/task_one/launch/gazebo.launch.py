from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_path

def generate_launch_description():
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([str(get_package_share_path('gazebo_ros')), '/launch/gazebo.launch.py']),
    )

    spawn_entity = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=['-entity', 'bot_xiaoche', '-file', str(get_package_share_path('task_one') / 'urdf/xiaoche.urdf')],
        output='screen'
    )

    return LaunchDescription([
        gazebo,
        spawn_entity,
    ])