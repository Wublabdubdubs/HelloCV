#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2_ros/static_transform_broadcaster.h"

class StaticTFBroadCaster : public rclcpp::Node
{
private: 
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> broadcaster_;
public:
    StaticTFBroadCaster(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        broadcaster_=std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        this->publish_tf();
    }

    void publish_tf()
    {
        geometry_msgs::msg::TransformStamped tranform;
        tranform.header.stamp=this->get_clock()->now();
        tranform.header.frame_id="map";
        tranform.child_frame_id="target_point";
        tranform.transform.translation.x=5.0;
        tranform.transform.translation.y=3.0;
        tranform.transform.translation.z=0.0;
        tf2::Quaternion q;
        q.setRPY(0.0,0.0,60*M_PI/180.0);
        tranform.transform.rotation=tf2::toMsg(q);
        this->broadcaster_->sendTransform(tranform);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<StaticTFBroadCaster>("Caster");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}