#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"
class ServiceClient : public rclcpp::Node
{
public:
    ServiceClient(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        Client_=this->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints_srv");
    }
    void send_request(int a,int b)
    {
        RCLCPP_INFO(this->get_logger(),"clac %d + %d ",a,b);
        while(!Client_->wait_for_service(std::chrono::seconds(1)))
        {
            if(!rclcpp::ok())
            {
                RCLCPP_ERROR(this->get_logger(),"异常");
                return ;
            }
            RCLCPP_INFO(this->get_logger(),"等待...");
        }

        auto request=std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
        request->a=a;
        request->b=b;
        Client_->async_send_request(request,std::bind(&ServiceClient::result_callback_,this,std::placeholders::_1));
    }
private:
    rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr Client_;
    void result_callback_(rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedFuture result_future)
    {
        auto response=result_future.get();
        RCLCPP_INFO(this->get_logger(),"结果%ld",response->sum);
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ServiceClient>("Client1");
    int a=1,b=2;
    node->send_request(a,b);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
//ros2 pkg create example_parameters_rclcpp --build-type ament_cmake --dependencies rclcpp 