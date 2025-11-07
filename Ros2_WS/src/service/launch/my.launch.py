import launch
import launch_ros

def generate_launch_description():
    start_ServiceSever = launch_ros.actions.Node(
        package='service',
        executable='ServiceServer',
        output = 'sreen',
    )
    
    start_ServiceClient = launch_ros.actions.Node(
        package='service',
        executable='ServiceClient',
        output = 'sreen',
    )

    return launch.LaunchDescription([
        start_ServiceSever,
        start_ServiceClient
    ])