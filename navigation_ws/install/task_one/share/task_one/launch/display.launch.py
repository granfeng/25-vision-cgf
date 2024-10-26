from ament_index_python.packages import get_package_share_path

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.conditions import IfCondition, UnlessCondition
from launch.substitutions import Command, LaunchConfiguration

from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue

def generate_launch_description():
    # 获取 'task_one' 包的共享路径
    task_one_path = get_package_share_path('task_one')
    
    # 设置默认的 URDF 文件路径
    default_model_path = task_one_path / 'urdf/xiaoche.urdf'

    # 声明一个启动参数 'gui'，默认值为 'false'，可选值为 'true' 或 'false'
    gui_arg = DeclareLaunchArgument(name='gui', default_value='false', choices=['true', 'false'],
                                    description='Flag to enable joint_state_publisher_gui')
    
    # 声明一个启动参数 'model'，默认值为默认的 URDF 文件路径
    model_arg = DeclareLaunchArgument(name='model', default_value=str(default_model_path),
                                      description='Absolute path to robot urdf file')

    # 使用 xacro 命令解析 URDF 文件，并将其作为参数传递给 'robot_description'
    robot_description = ParameterValue(Command(['xacro ', LaunchConfiguration('model')]),
                                       value_type=str)

    # 启动 'robot_state_publisher' 节点，并将 'robot_description' 作为参数传递
    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_description}]
    )

    # 根据 'gui' 参数的值，决定启动 'joint_state_publisher' 还是 'joint_state_publisher_gui'
    joint_state_publisher_node = Node(
        package='joint_state_publisher',
        executable='joint_state_publisher',
        condition=UnlessCondition(LaunchConfiguration('gui'))  # 如果 'gui' 为 'false'，则启动此节点
    )

    joint_state_publisher_gui_node = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        condition=IfCondition(LaunchConfiguration('gui'))  # 如果 'gui' 为 'true'，则启动此节点
    )

    # 启动 'rviz2' 节点，并设置输出为屏幕
    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
    )

    # 返回 LaunchDescription 对象，包含所有定义的启动参数和节点
    return LaunchDescription([
        gui_arg,
        model_arg,
        # rviz_arg,  # 如果你有自定义的 RViz 配置文件，请取消注释
        joint_state_publisher_node,
        joint_state_publisher_gui_node,
        robot_state_publisher_node,
        rviz_node
    ])