import launch
import launch_ros

def generate_luanch_description():

    start_static_tf_broadcast =  launch_ros.actions.node(
        package="demo_tf",
        excutable="static_tf_broadcaster",
        output="log"
    )

    start_dynamic_tf_broadcast = launch_ros.actions.node(
        package="demo_tf",
        excutable="dynamic_tf_broadcaster",
        output="log"
    )



    return launch.LaunchDescription([
        start_dynamic_tf_broadcast,
        start_static_tf_broadcast
    ])