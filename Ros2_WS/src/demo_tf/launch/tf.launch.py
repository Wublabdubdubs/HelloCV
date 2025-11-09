import launch
import launch_ros

def generate_launch_description():

    start_static_tf_broadcast =  launch_ros.actions.Node(
        package="demo_tf",
        executable="static_tf_broadcaster",
        output="log"
    )

    start_dynamic_tf_broadcast = launch_ros.actions.Node(
        package="demo_tf",
        executable="dynamic_tf_broadcaster",
        output="log"
    )



    return launch.LaunchDescription([
        start_dynamic_tf_broadcast,
        start_static_tf_broadcast
    ])