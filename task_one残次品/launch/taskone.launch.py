from ament_index_python.packages import get_package_share_path

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition, UnlessCondition
from launch.substitutions import Command, LaunchConfiguration

from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue

def generate_launch_description():
    # 获取task_one包的共享路径
    task_one_path = get_package_share_path('task_one')
    # 设置默认的URDF文件路径
    default_model_path = task_one_path / 'urdf/xiaoche.urdf'
    # 设置默认的RViz配置文件路径
    default_rviz_config_path = task_one_path / 'rviz/xiaoche.rviz'

    # 定义一个启动参数gui，用于控制是否启动joint_state_publisher_gui
    gui_arg = DeclareLaunchArgument(name='gui', default_value='false', choices=['true', 'false'],
                                    description='Flag to enable joint_state_publisher_gui')
    # 定义一个启动参数model，用于指定URDF文件的路径
    model_arg = DeclareLaunchArgument(name='model', default_value=str(default_model_path),
                                      description='Absolute path to robot urdf file')
    # 定义一个启动参数rvizconfig，用于指定RViz配置文件的路径
    rviz_arg = DeclareLaunchArgument(name='rvizconfig', default_value=str(default_rviz_config_path),
                                     description='Absolute path to rviz config file')

    # 使用xacro命令解析URDF文件，并将其作为参数传递给robot_state_publisher
    robot_description = ParameterValue(Command(['xacro ', LaunchConfiguration('model')]),
                                       value_type=str)

    # 启动robot_state_publisher节点，发布机器人的状态
    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_description}]
    )

    # 根据gui参数，决定启动joint_state_publisher还是joint_state_publisher_gui
    joint_state_publisher_node = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
        condition=UnlessCondition(LaunchConfiguration('gui'))  # 如果gui为false，启动此节点
    )

    joint_state_publisher_gui_node = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        condition=IfCondition(LaunchConfiguration('gui'))  # 如果gui为true，启动此节点
    )

    # 启动rviz2节点，用于可视化机器人模型
    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', LaunchConfiguration('rvizconfig')],  # 使用指定的RViz配置文件
    )

    # 返回LaunchDescription对象，包含所有定义的节点和参数
    return LaunchDescription([
        gui_arg,
        model_arg,
        rviz_arg,
        joint_state_publisher_node,
        joint_state_publisher_gui_node,
        robot_state_publisher_node,
        rviz_node
    ])