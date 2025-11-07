#include "my_interface/msg/example.hpp"
#include "rclcpp/rclcpp.hpp"
class TopicSubscriber : public rclcpp::Node
{
public:
    TopicSubscriber(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        command_subscribe_=this->create_subscription<my_interface::msg::Example>("command",10,std::bind(&TopicSubscriber::command_callback,this,std::placeholders::_1));
    }
private:
    rclcpp::Subscription<my_interface::msg::Example>::SharedPtr command_subscribe_;
    void command_callback(const my_interface::msg::Example::SharedPtr msg)
    {
        RCLCPP_INFO(this->get_logger(),"%d %d",msg->data1,msg->data2);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TopicSubscriber>("Subscriber1");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}