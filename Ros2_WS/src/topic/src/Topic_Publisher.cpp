#include "rclcpp/rclcpp.hpp"
#include "my_interface/msg/example.hpp"

class TopicPublisher : public rclcpp::Node
{
public:
    TopicPublisher(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        command_publisher_ = this->create_publisher<my_interface::msg::Example>("command", 10);
        timer_ =this->create_wall_timer(std::chrono::milliseconds(500),std::bind(&TopicPublisher::timer_callback,this));
    }

private:
    void timer_callback()
    {
        my_interface::msg::Example message;
        message.data1=1;
        message.data2=2;
        RCLCPP_INFO(this->get_logger(),"Publish: %d,%ld",message.data1,message.data2);
        command_publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<my_interface::msg::Example>::SharedPtr command_publisher_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TopicPublisher>("topic_publisher_01");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
