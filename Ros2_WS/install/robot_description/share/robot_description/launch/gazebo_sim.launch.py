import launch
import launch.launch_description_sources
import launch_ros
from ament_index_python.packages import get_package_share_directory
import os
def generate_launch_description():
    
    urdf_package_path=get_package_share_directory('robot_description')
    default_urdf_path=os.path.join(urdf_package_path,'urdf','fishbot','fishbot.urdf.xacro')
    default_gazebo_world_path=os.path.join(urdf_package_path,'world','custom_room.world')
    
    action_declare_arg_xacro_path=launch.actions.DeclareLaunchArgument(
        name='model',default_value=str(default_urdf_path),description='Path of the modle file'
    )

    #通过文件路径获取内容
    substitutions_command_result=launch.substitutions.Command([
        'xacro ',
        launch.substitutions.LaunchConfiguration('model')
    ])
    robot_description_value=launch_ros.descriptions.ParameterValue(substitutions_command_result,value_type=str)
    action_robot_state_publisher=launch_ros.actions.Node(
        package='robot_state_publisher',
        #---------------------------
        # executable='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description':robot_description_value}]
    )

    action_launch_gazebo=launch.actions.IncludeLaunchDescription(
        launch.launch_description_sources.PythonLaunchDescriptionSource(
            [get_package_share_directory('gazebo_ros'),'/launch','/gazebo.launch.py']
        ),
        launch_arguments=[("world",default_gazebo_world_path),('verbose','true')]
    )
    action_spawn_entity =launch_ros.actions.Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=['-topic','/robot_description','-entity','fishbot']
    )
    # action_joint_state_publisher=launch_ros.actions.Node(
    #     package='joint_state_publisher',
    #     executable='joint_state_publisher'
    # )
    return launch.LaunchDescription([
        action_declare_arg_xacro_path,
        action_robot_state_publisher,
        action_launch_gazebo,
        action_spawn_entity
    ])