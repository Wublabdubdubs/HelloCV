#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/QuadWord.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2_ros/transform_listener.h"
#include "tf2_ros/buffer.h"
#include "tf2/utils.h"
#include "chrono"

using namespace std::chrono_literals;

class TFlistener : public rclcpp::Node
{
private:   
    std::shared_ptr<tf2_ros::TransformListener> listener_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::shared_ptr<tf2_ros::Buffer> buffer_;

public:
    TFlistener(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        buffer_= std::make_shared<tf2_ros::Buffer>(this->get_clock());
        this->listener_ =std::make_shared<tf2_ros::TransformListener>(*buffer_,this);
        timer_=this->create_wall_timer(1500ms,std::bind(&TFlistener::getTransform,this));
    }
    void getTransform()
    {
        try
        {   
            const auto transform=buffer_->lookupTransform("base_link","target_point",this->get_clock()->now(),1s);
            auto translation=transform.transform.translation;
            auto rotation=transform.transform.rotation;
            double Y,P,R;
            tf2::getEulerYPR(rotation,Y,P,R);
            RCLCPP_INFO(this->get_logger(),"trs: %f,%f,%f",translation.x,translation.y,translation.z);
            RCLCPP_INFO(this->get_logger(),"ypr: %f,%f,%f",Y,P,R);

        }
        catch(const std::exception &e)
        {
            RCLCPP_WARN(get_logger(),"%s",e.what());
        }
    }

};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TFlistener>("L1");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}