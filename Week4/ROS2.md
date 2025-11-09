我们不得不承认，本周ROS2和gazebo的学习称得上是时间紧，任务重，故难以复现前三周笔记一样“就算我那天失忆了，回来看一遍也能会”的详细程度

与此同时，[鱼香ROS](https://fishros.com/d2lros2/#/)实在是给出了相当详尽的学习资料，可以成为任何一个人系统学习ROS2的最优解

因此本篇笔记将不得不舍弃一些为了连续性作出没用的基础示例，着重记录在实践过程中遇到的疑惑/重点，以及笔者在看过鱼神的教程后不懂的地方，难免导致行文跳跃（说好听点叫“模块化”），请谅解



~~有一说一鱼香的视频比教程强好多~~

## ROS2的功能
ROS2的目标就一个：完成一个复杂的机器人系统内多个功能模块的通信，可以认为是在软件层面对机器人的连接与组装

一个机器人的各个功能模块被抽象为节点，节点之间通过话题/服务/动作/参数四种方式进行通信，而节点内部的处理和节点与实际功能模块之间的交互不受ros管辖

用户（我们）可以通过调用它提供的rclcpp/rclpy在自己的c++/python程序中实现这一功能

## 如何编写并运行一个ROS节点
ROS2使用colcon作为编译的工具

问题来了，编译，我们不是有Cmake了吗？colcon是干啥的？

是的，Cmake可以处理对于复杂大量源代码的编译工作，但Cmake编译出的可执行文件之间亦存在的复杂的先后依赖关系，而Colcon是这一关系的管理者

Calcon通过调用Cmake等编译系统，实现对一个庞大的ros程序进行编译



那么，开始吧

首先我们需要选择一个文件夹作为工作空间，初学阶段（按教程里的方法），我们对每一个小项目都单独创建`ProjectName/ProjectName_ws`（以下将这一目录简称为`WS`），之后在创建`WS/src`目录，在`WS/src`下，使用`ros2 pkg create`命令可以创建一个包，同时，我们可以给出参数来限制包的类型和依赖，具体而言：

> 千万注意：这里包的名字必须完全小写，可以有下划线，但必须小写，因为这里包的名字会在后续生成中作为cmake中的project名，而cmake要求project名完全小写，否则无法运行
>

以`ros2 pkg create service --build-type ament_cmake --dependencies rclcpp`为例

我们创建了一个名叫`service`的包，它的编译类型是`ament_cmake`，并为它安装`rclcpp`依赖

之后，我们会看到目录变成这样

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1761983153770-be65b3c0-22e1-477d-92c8-e812dbcea59b.png)

之后就可以在src目录下写自己的代码了

```cpp
//本代码可以说是整个ROS中的“Hello World”
//功能仅仅是创建一个什么功能都没有的节点，但还是有一些地方需要注释

#include "rclcpp/rclcpp.hpp"

class ServiceServer : public rclcpp::Node //声明新建的类，继承自rclcpp提供的Node类
{
public:
    ServiceServer(std::string name) : Node(name)//构造函数，唯一的参数是名字
    {
        RCLCPP_INFO(this->get_logger(), "服务节点%s节点已经启动.", name.c_str());
        //RCLCPP_INFO用于打印某一信息
        //this->get_logger()是Node类提供的函数，这里用于将信息与节点联系在一起
        //（可用于对多条INFO的筛选，目前直接背过然后忽略也行）
        //后两个参数参考scanf的参数，标准输出中%s转义的是char数组类的字符串(c风格)
        //string类型提供了函数c_str()用于将string转化为c风格字符串
    }

private:
};

int main(int argc, char **argv)
//主函数传参，argc代表参数数量，argv为字符串数组，依次代表每一条参数
//参数是执行时ros2传入的，我们不用管
{
    rclcpp::init(argc, argv);
    //ros2按照它的参数初始化
    auto node = std::make_shared<ServiceServer>("Server1");
    //auto 可以根据等号后面的式子类型推断变量类型
    //make_shared 用于生成特定类型的共享指针，后面括号里传入的是名字
    //什么是共享指针？总之就是一种高级的指针，会在所指内容被析构后自动释放内存
    rclcpp::spin(node);
    rclcpp::shutdown();
    //这两句的作用就是等待Crtl+C结束程序背过直接写就行
    return 0;
}

```

之后要编辑CMakeList.txt，在末尾添加

```cmake
add_executable(ServiceServer src/Service_Server.cpp)
ament_target_dependencies(ServiceServer rclcpp)
#这句话需要前面find_package(ament_cmake)
#意思意思是将原来的两句话（target_link***&target_include***）合并

install(
  TARGETS ServiceServer
  DESTINATION lib/${PROJECT_NAME}
)

```

`add_executable`用于生成可执行文件，这个在Cmake笔记里写过了

`install`是一个比较重要的命令用于安装可执行文件，这里我们只提供三个基本参数：

`TARGETS`用于指定`install`对象的类型是可执行文件，`ServiceServer`是可执行文件的名字

`DESTINATION`是目标目录\

为什么要install一遍？因为可执行文件默认生成在build目录下，想用ros2运行需要放在install的lib目录下，所以需要拷贝一遍

这样`WS/src`内就配置好了让我们`cd ..`退回到`WS`目录

在这个目录下执行`colcon build --packages-select service`

这就是使用`colcon`来进行编译了，`--packages-select`表示只编译一个功能包，也就是后面跟的`service`（这也就暗示了一个src下是可以有多个包的）

> 这里是报错的高频区域，如果遇到报错/警告一定要认真查看，许多问题会迎刃而解
>

它会首先在`src`目录下寻找`package.xml`文件，依据它进行构建，期间会用到`CMakeList.txt`，并且会报告在那一步遇到问题，如果一切都没有问题，就会告知你构建完成，并生成`build`,`install`,`log`三个文件夹

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1761987227481-d9bf41f1-0189-4792-994f-a79340fbb248.png)

接下来就可以`ros2 run service ServiceServer`了？

还不行，ros2并不知道你说的service在哪里，所以还要`source install/setup.bash`将这个功能包添加到环境变量中

一切顺利的话，你的命令行是这样的：

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1761987361400-25dd64eb-f7e3-4f49-8169-1676132a44b0.png)

## 通讯方式：
### 接口
无论是服务还是话题，传输一条信息都不能直接使用c++原生的数据类型，而是需要特定的接口，不太明白？让我们看看接口文件的内容长什么样子

用于话题的.msg：（注意文件名首字母大写）

```cmake
int32 data1
int64 data2
```

用于服务的.srv:

```cmake
int32 a
int32 b
---
int32 sum
```

其实就是一种自定义的结构体，不是吗？

定义出来怎么使用？

通过colcon编译为.hpp文件，之后就可以用了

具体而言，想编译接口需要依赖rosidl_default_generater，在ros2 pkg create时把参数传进去就行了

进一步，修改Cmake

```cmake

find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Example.msg"
)

```

此外还有在.xml文件之中添加一句：

```xml

<buildtool_depend>ament_cmake</buildtool_depend>

<depend>rosidl_default_generators</depend>

<member_of_group>rosidl_interface_packages</member_of_group> //Here!

<test_depend>ament_lint_auto</test_depend>
<test_depend>ament_lint_common</test_depend>
```

之后只需要把你需要的端口文件写进对应文件夹就行，应当如下：

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762079448847-46dff25d-1682-48a4-a048-d7529df0ba15.png)

使用colcon进行编译，之后就可以在install下找到生成的hpp文件了

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762079688558-8924bde9-4d12-40f9-91c2-d8dba99eb4e4.png)

### 话题
首先，DDS下，只有本质上只有话题一种通信方式：话题

服务由话题实现，参数由服务实现，动作由服务和话题实现

所以话题的基础性和重要性不言而喻，因此话题部分会更详细



类似于我们上网的帖子，一侧发送信息，一侧接受信息，适用于单向的大规模的信息传输

发送信息的叫`publisher`，接受的叫`subscriber`

在教程示例中发布一条信息看起来十分麻烦，但是实际上~~也确实不简单~~列出步骤还是比较清晰的：

让我们先不提定时功能

那么我们的目标就是创建一个TopicPublisher类，用于发送消息

ROS为发布信息提供了一个类：rclcpp::Publisher<T>，这个类中定义了publish(T message)函数

作用就是把message发布到对应topic上，上面的T是一种特殊的数据类型，也就是上文的接口

```cpp
//定义：
    rclcpp::Publisher<my_interface::msg::Example>::SharedPtr publisher_;
    //注意：这里创建的是对应类型的共享指针
    //这里用的数据类型是第一节自定义的接口
//使用：
    //首先定义要传输的message
    my_interface::msg::Example message;
    //初始化消息
    message.data1=1;
    message.data2=2;
    //这里我们还需要初始化一下publisher_
    publisher_ = this->create_publisher<my_interface::msg::Example>("command", 10);
    //create_publisher是ROS提供的用于构建的函数
    //尖括号中写的是接口类型
    //后面的括号中，第一个参数是话题名称，第二个参数是消息队列的最大长度
    publisher_->publish(message);
```

这样来看是不是还挺简单的？

然后加入定时：

ROS2为我们提供了定时的功能：rclcpp::TimerBase类型

它的定义和上文一样，要写成共享指针`rclcpp::TimerBase::SharedPtr timer_;`

但初始化值得谈一谈：

ROS2同样是给出了返回值为SharedPtr的构造方法需要两个参数：

第一个是设定的时间，需要用C++给出的chrono下的时间构造方法

第二个是到时间调用的函数，需要使用C++的统一函数类型，需要使用std::bind构造

把两个合在一起：

```cpp
#include "rclcpp/rclcpp.hpp"
#include "my_interface/msg/example.hpp"
//
class TopicPublisher : public rclcpp::Node
{
public:
    TopicPublisher(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        command_publisher_ = this->create_publisher<my_interface::msg::Example>("command", 10);
        timer_ =this->create_wall_timer(
            std::chrono::milliseconds(500),
            std::bind(&TopicPublisher::timer_callback,this)
            );
        //这里详细说一下bind的来历和用法
        /*
        C++11之后，有三种不同的函数：
            正常定义的函数
            类的成员函数
            匿名的lamda表达式函数
        为了将三者统一起来，变为可供人随意使用的函数模板，std::bind应运而生，语法如下：
            bind(
                原函数,
                std::placehoder::_1,std::placehoder::_2,...,std::placehoder::_n
            )
        正如其名，placehoder就是占位符，按照原函数的参数需要写就行，有几个写几个，因为这里callback不需要，所以不写
        但这和上面还是有点区别的，为什么?
        因为一般的函数会在这里自动转化为指针，但成员函数不行，所以需要显式地写出&
        同时，成员函数的运行不能脱离实体，所以这里还要补充一个this指针，表明使用自身作为这个实体
        */
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
```

完全搞懂了publisher怎么写，subscriber就好写了

```cpp
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
```

唯一可能影响理解的地方就是`create_subscription`中的bind，这里它是每次接收到一条消息就执行一次，其他的就显然了

### 服务
类似于我们上网用的服务器，一侧发送请求，一侧返回结果，适用于双向通信（本质由话题实现）

先说服务端，如果你已经理解了上面的所有内容，服务端的实现是十分易懂的：

```cpp
#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

class ServiceServer : public rclcpp::Node
{
public:
    ServiceServer(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        add_ints_server_=this->create_service<example_interfaces::srv::AddTwoInts>("add_two_ints_srv",std::bind(&ServiceServer::handle_add_two_ints,this,std::placeholders::_1,std::placeholders::_2));
    }
private:
    rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr add_ints_server_;
    void handle_add_two_ints(const std::shared_ptr<example_interfaces::srv::AddTwoInts::Request> request,std::shared_ptr<example_interfaces::srv::AddTwoInts::Response> response)
    {
        RCLCPP_INFO(this->get_logger(),"get: %ld + %d\n",request->a,request->b);
        response->sum=request->a+request->b;
    }
};
```

这里注意一下这个handle_add_two_ints函数的传参，第一个是接口Request类型，即分割线以上的部分，第二个参数对应分割线以下的部分，这里因为是SharedPtr，实际上，这个指针和客户端的一个指针是共享的，因此修改这个值就可以让客户端接收到

下面来看看Client的代码

```xml
#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"
class ServiceClient : public rclcpp::Node
{
public:
    ServiceClient(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        //到此为止一切正常
        Client_=this->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints_srv");
        //传参为频道，但是没有回调函数
    }
    //因为请求明显是一个主动的过程
    //发送请求代码如下
    void send_request(int a,int b)
    {
        RCLCPP_INFO(this->get_logger(),"clac %d + %d ",a,b);
        //下面循环是在等待服务端开机，下一行括号里的参数是等待的时间
        while(!Client_->wait_for_service(std::chrono::seconds(1)))
        {
            if(!rclcpp::ok())//这里的ok()若为零，则是收到了关闭命令
            {
                RCLCPP_ERROR(this->get_logger(),"异常");
                return ;
            }
            RCLCPP_INFO(this->get_logger(),"等待...");
        }

        auto request=std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
        request->a=a;
        request->b=b;
        //声明并初始化请求
        Client_->async_send_request(request,std::bind(&ServiceClient::result_callback_,this,std::placeholders::_1));
        //发送请求，第一个参数是请求内容，第二个是收到结果时的回调函数
    }
private:
    rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr Client_;
    //收到结果时，结果的类型时一个future类共享指针
    //future指针的特点是：不需要当前在定义时立马有意义，在必须使用的时候使用->get()，才会使得程序阻塞，等待get的结果（SharedPtr类型）
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
    int a,b;
    std::cin>>a>>b;
    node->send_request(a,b);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
```

### 参数
正如其名，指的是控制节点的变量，支持实时修改（本质由服务实现，不断抓取参数列表）

参数服务因为其客户端就是coder，所以它不需要写Client，而是使用极其简单的指令进行修改

话不多说，上代码：（以调节输出日志的等级为例）

```cpp
#include<chrono>
#include "rclcpp/rclcpp.hpp"

class ParameterBasic : public rclcpp::Node
{
public:
    ParameterBasic(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        this->declare_parameter("rcl_log_level",0);
        //declare_parameter 用于定义和初始化参数
        //第一个是参数名字，第二个是初始值
        this->get_parameter("rcl_log_level",log_level);
        //get_parameter 用于将参数的值赋给变量
        //第一个是参数名字，第二个是用来接受参数的变量
        this->get_logger().set_level((rclcpp::Logger::Level)log_level);
        //这句话是设置日志等级
        using namespace std::literals::chrono_literals;
        timer_=this->create_wall_timer(500ms,std::bind(&ParameterBasic::timer_callback,this));
        //定时接收
    }
private:
    int log_level;
    rclcpp::TimerBase::SharedPtr timer_;
    void timer_callback()
    {
        this->get_parameter("rcl_log_level",log_level);
        this->get_logger().set_level((rclcpp::Logger::Level)log_level);
        //每次将rcl_log_level的值赋值给log_level，然后修改
        std::cout<<"======================================================"<<std::endl;
        //只会输出>=level的日志,每一条的对应值在下
        RCLCPP_DEBUG(this->get_logger(), "我是DEBUG级别的日志，我被打印出来了!");	//10
        RCLCPP_INFO(this->get_logger(), "我是INFO级别的日志，我被打印出来了!");	//20
        RCLCPP_WARN(this->get_logger(), "我是WARN级别的日志，我被打印出来了!");	//30
        RCLCPP_ERROR(this->get_logger(), "我是ERROR级别的日志，我被打印出来了!");	//40
        RCLCPP_FATAL(this->get_logger(), "我是FATAL级别的日志，我被打印出来了!");	//50
    }
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ParameterBasic>("Para1");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

```



## QoS 
ROS2 采用 DDS<font style="color:rgb(25, 27, 31);">（Data Distribution Service 数据分发服务）来进行通信，它支持不同的QoS</font>（Quality of Service）不同的QoS对应不同的信息传输质量（可以理解为不同的行为准则）

具体而言，一个QoS有五个参数，分别代表传输信息时不同方面的要求

以<font style="color:black;">Reliability为例，它可以设置成 Best effort和Reliable两个值</font>

+ <font style="color:black;">Best effort: 尽力传送数据，但是网络不稳定可能会丢弃一些数据。</font>
+ <font style="color:black;">Reliable: 确保数据被传送到，若没收到就重新传，可能会重传多次，导致数据延时严重。</font>

再例如<font style="color:black;">Lifespan，可以将</font><font style="color:rgb(1, 1, 1);">Duration设置为需要的时间</font>

<font style="color:rgb(1, 1, 1);">Duration: 设置数据从发布到被接收的最大间隔时间。超过该时间将被认为是过时的数据，直接丢弃了。（这对于传感器数据来说很重要，因为过时的传感器数据毫无用处。）</font>

<font style="color:rgb(1, 1, 1);">对于其他参数，参考：</font>[<font style="color:#01B2BC;">Here</font>](https://mp.weixin.qq.com/s/J63fO4c_QIseLGQd5W2fAw)

另外，我们也可以通过指令`ros2 topic info /话题名 --verbose`来查看话题的QoS

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762334092113-94e87f93-78cf-4051-a1f2-243a91fcd6da.png)

## Launch工具
每一次我们想测试一下我们程序的结果，都需要给每一个节点单开一个终端，仅仅是测试诸如两个节点的通讯这样基础的工作我们就已经能感到明显的不便，很明显想要从事大规模的开发不解决这个问题是寸步难行的

ROS2自然会意识到这个问题，为我们提供了Launch工具

Luanch工具就是用来运行多个ROS2中的可执行文件用的，它有三种写法，出于功能性考虑，我们选择python写法

先贴代码，下面解释：

```python
# luanch文件一定以.luanch.py为结尾

import launch
import launch_ros
#要调用这两个库

#执行launch就是调用generate_launch_description函数，因此定义的这个函数名不能改变
#它应当返回luanch类型的量
#创建这个量需要使用launch.LaunchDescription函数并传入一个列表，其中的元素代表要执行的文件
#这些元素使用launch_ros.actions.Node构造，形式如下：
def generate_launch_description():
    start_ServiceSever = launch_ros.actions.Node(
        package='service',				#包名
        executable='ServiceServer',		#可执行文件名
        output = 'screen',				#输出方式(在screen/log/both之中选)
    )

    start_ServiceClient = launch_ros.actions.Node(
        package='service',
        executable='ServiceClient',
        output = 'screen',
    )

    return launch.LaunchDescription([
        start_ServiceSever,
        start_ServiceClient
    ])
```



更多Launch的用法在gazebo章节用到的地方继续拓展

## TF树
TF 就是 Transform 变换，即坐标的变化，包括平移和旋转

依据一些基础的代数几何知识，这个过程可以使用矩阵表示，又因为，我们常常会已知几对物体的两两相对位置，而需要推算其他的成对相对位置，树上矩阵前缀积可以快速解决查询，总之，在ROS中，我们使用树形结构解决相对坐标问题

实现上，ROS2称机器人的一个部分为一个link，并为它们建立一个/tf话题，通过不断在话题中发送TF信息，来实现对坐标相对关系的表示

TF分为静态TF和动态TF，下面我们分别写代码来实现一下

发布静态TF：

```cpp
//静态

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include "tf2_ros/static_transform_broadcaster.h"//注意这里在动态发布时要删去static

class StaticTFBroadCaster : public rclcpp::Node
{
private: 
    std::shared_ptr<tf2_ros::StaticTransformBroadcaster> broadcaster_;//声明一个静态发布器
public:
    StaticTFBroadCaster(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        
        //初始化发送器
        broadcaster_=std::make_shared<tf2_ros::StaticTransformBroadcaster>(this);
        
        //因为时静态的，所以只需要发送一遍
        this->publish_tf();
    }

    void publish_tf()
    {
        //声明一条TF消息
        geometry_msgs::msg::TransformStamped tranform;
        
        //初始化
        tranform.header.stamp=this->get_clock()->now(); //时间戳，因为我们可能需要按照是时间决定是否留存此消息，所以需要将时间给出
        tranform.header.frame_id="map";					//相对原点的名字
        tranform.child_frame_id="target_point";			//相对终点的名字
        //这里表示此条TF关系是以map为参考target_point的位置和方向
        //相对平移
        tranform.transform.translation.x=5.0;
        tranform.transform.translation.y=3.0;
        tranform.transform.translation.z=0.0;
        
        //相对旋转
        tf2::Quaternion q;
        q.setRPY(0.0,0.0,60*M_PI/180.0);
        tranform.transform.rotation=tf2::toMsg(q);
        //这里解释一下，空间中相对旋转有一种符合直觉的表示方法——RPY/欧拉角
        //	翻滚角（Roll）：绕前后轴的旋转。
        //	俯仰角（Pitch）：绕左右轴的旋转。
        //	偏航角（Yaw）：绕垂直轴的旋转。
        //但事实上，这样表示会存在歧义，因此ROS在传输时使用四元数表示角度
        //而我们的直觉和观察都会得到RPY数据，因此需要用ROS2提供的函数来进行转换
        this->broadcaster_->sendTransform(tranform);
    }
};
```

发布动态TF（和静态的大同小异）

```cpp

using namespace std::chrono_literals;
//调用此命名空间，允许我们使用100ms/1s/1h来表示时间
class DynamicTFBroadCaster : public rclcpp::Node
{
private: 
    std::shared_ptr<tf2_ros::TransformBroadcaster> broadcaster_;
    rclcpp::TimerBase::SharedPtr Timer_; //添加计时器
public:
    DynamicTFBroadCaster(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        broadcaster_=std::make_shared<tf2_ros::TransformBroadcaster>(this);
        // this->publish_tf();
        //改为定时发布
        Timer_=this->create_wall_timer(100ms,std::bind(&DynamicTFBroadCaster::publish_tf,this));
    }

    void publish_tf() {......} //发布的写法和上面一样
};
```

有人发布就需要有人接收

接收数据的过程分为两步：

首先，我们将当前所有TF数据放进一个缓冲区buffer

然后，在这个树状的关系中查找出指定的两个TF的相对位置



```cpp


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
    //声明缓冲区
    std::shared_ptr<tf2_ros::Buffer> buffer_;

public:
    TFlistener(std::string name) : Node(name)
    {
        RCLCPP_INFO(this->get_logger(), "%s节点已经启动.", name.c_str());
        //初始化缓冲区
        buffer_= std::make_shared<tf2_ros::Buffer>(this->get_clock());
        //将监听数据放入缓冲区
        this->listener_ =std::make_shared<tf2_ros::TransformListener>(*buffer_,this);
        //定时打印数据
        timer_=this->create_wall_timer(1500ms,std::bind(&TFlistener::getTransform,this));
    }
    void getTransform()
    {
        //若buffer中找不到信息，会出现错误，这里捕捉错误并输出防止程序崩溃
        try
        {   
            const auto transform=buffer_->lookupTransform("base_link","target_point",this->get_clock()->now(),1s);
            //lookupTransform 用于查看指定两点的相对坐标，注意先传目标，再传参考系
            //最后的时间参数表示等待时间，即若经过1s未收到消息，返回错误
            auto translation=transform.transform.translation;
            auto rotation=transform.transform.rotation;
            //我们得到的依然是四元数表示下的角度，使用getEulerYPR(rotation,Y,P,R)会将其转为YPR并将转换结果依次传入变量
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
```

## Rviz工具
Rviz用于将数据进行三维可视化呈现

使用鱼香ROS一键安装ROS的时候已经为我们装好了Rviz，

使用指令`Rviz2`就可以运行

Rviz和rqt一样，是需要插件才工作的，我们可以在界面左侧看到插件和配置列表，左下角add就可以选择我们需要的插件了，以TF为例：

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762668136639-285a4c9f-4b56-4562-9281-773e5e0394b0.png)

下面是插件面板

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762668200192-abae34f2-3842-46b8-bb3f-53f5aa8e3899.png)

Fixed Frame 就是坐标的参考，设在哪个哪个就在图的中间

下面TF的分选项，基本看名字都能看出作用：

show names 显示名字

show axes 显示坐标系（就是下图的红绿蓝柱子）

show arrows 显示箭头

marker scale （上面几项）大小

等等等等，可以通过尝试看出功能，不再一一赘述

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762668153703-673e921c-5d1e-4d33-852e-2952c603740c.png)

