`sudo apt install gazebo`安装gazebo

仿真，需要机器人和环境，下面我们对它们分别创建

## URDF表示机器人
想要在gazebo环境中对机器人进行仿真，我们必须先把机器人的样子描述出来，现在最通用的办法是使用urdf文件，它采用xml格式，让我们先举个例子（文件需要使用.uxdf为后缀）

### 编写URDF文件
```xml
<?xml version="1.0"?>
<!--必须先声明版本号-->
<robot name="first_robot"> <!--机器人标签（必须有名字） -->
  <!--一个部件被称作一个link-->
  <!--Body-->
  <link name="base_link">
    <!--visual标签用于描述其视觉效果-->
    <visual>
      <!--origin表示相对原点的位置-->
      <origin xyz="0.0 0.0 0.0" rpy="0.0 0.0 0.0"/>
      <!--geometry表示几何形状：box长方体，cylinder圆柱，sphere球-->
      <geometry>
        <cylinder radius="0.10" length="0.12"/>
      </geometry>
      <!--材质-->
      <material name="white">
        <color rgba="1.0 1.0 1.0 0.5"/>
      </material>
    </visual>
  </link>
  <!--语法上与上面相似>
  <!--IMU-->
  <link name="IMU_link">
    <visual>
      <origin xyz="0.0 0.0 0.0" rpy="0.0 0.0 0.0"/>
      <geometry>
        <box size="0.02 0.02 0.02"/>
      </geometry>
      <material name="black">
        //最后一个参数是透明度
        <color rgba="0.0 0.0 0.0 0.5"/>
      </material>
    </visual>
  </link>
  <!--两个link之间使用joint链接-->
  <!--Joint-->
  <!--type表示链接类型，fixed表示固定不动-->
  <joint name="IMU_joint" type="fixed">
    <parent link="base_link"/>
    <child link="IMU_link"/>
    <!--位置偏差-->
    <origin xyz="0.0 0.0 0.03" rpy="0.0 0.0 0.0"/>
  </joint>
</robot>
```

让我们在rviz中看看这个简单的机器人：

添加RobotModel插件，使用文件显示，选择我们刚写好的文件

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762671261080-162fbe00-f45c-4393-a646-80a74b977cc5.png)

What's wrong?

Rviz检测不出机器人组件的位置关系，所以显示不出来

我们不是写joint了吗？

事实上，rviz并不能识别joint标签给出的关系，怎么办？

ROS2为我们提供了`robot_state_publisher`和`joint_state_publisher`两个组件，它们可以解析

urdf文件并发布话题来给出机器人的信息，但是robot_state_publisher需要我们将整个urdf文件的内容作为参数传入，怎么办？

我们需要为此写一个launch文件：

```python
import launch
import launch_ros
from ament_index_python.packages import get_package_share_directory
import os
def generate_launch_description():

    #找到文件路径并使用os.path.join拼接，作为默认路径
    urdf_package_path=get_package_share_directory('robot_description')
    default_urdf_path=os.path.join(urdf_package_path,'urdf','first_robot.urdf')
    #从输入中获取参数（没有就用默认值）
    action_declare_arg_mode_path=launch.actions.DeclareLaunchArgument(
        name='model',default_value=str(default_urdf_path),description='Path of the modle file'
    )

    #通过文件路径获取内容
    #这里Command是CMD命令的意思，使用 cat 命令即可将文件内容显示出来
    substitutions_command_result=launch.substitutions.Command([
        'cat ',
        launch.substitutions.LaunchConfiguration('model')
    ])
    #上文substitutions_command_result只是一个命令
    #launch_ros.descriptions.ParameterValue可以执行这个命令并获得它的结果
    #value_type=str即以string形式输出
    robot_description_value=launch_ros.descriptions.ParameterValue(substitutions_command_result,value_type=str)
    #启动publisher节点
    action_robot_state_publisher=launch_ros.actions.Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description':robot_description_value}]
        #相当于在命令后加上 -p '...'=...
    )
    #joint节点的启动就相对简单
    action_joint_state_publisher=launch_ros.actions.Node(
        package='joint_state_publisher',
        executable='joint_state_publisher'
    )
    #启动Rviz(除了常用的命令行形式,ROS中也有Rviz的功能包，可以通过启动功能节点的方式启动)
    action_rviz_node =launch_ros.actions.Node(
        package='rviz2',
        executable='rviz2'
    )
    return launch.LaunchDescription([
        action_declare_arg_mode_path,
        action_robot_state_publisher,
        action_joint_state_publisher,
        action_rviz_node
    ])

```

成功（下图多装了一个IMU）

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762672124480-d475595d-78ef-4f01-9718-b120447b8b35.png)

### 使用XACRO拓展URDF
一个机器人可能有多个相同的组件（两个轮子，两只手等情况）

如果只是不断重复代码未免有点笨拙了，xacro允许我们像定义函数一样写一段xacro格式标签，来实现代码的复用，下面用轮子举个例子：

```xml
<?xml version="1.0"?>
<!--这里相当于声明形式函数，因此不需要写名字-->
<!--xmlns:xacro="https:/www.ros.org/wiki/xacro"表明下面的代码符合xacro标准-->
<robot xmlns:xacro="https:/www.ros.org/wiki/xacro">  
  <!--给出函数名和参数列表-->
  <xacro:macro name="wheel_xacro" params="wheel_name xyz">
    <!--${}表示引用替换为变量值-->
    <link name="${wheel_name}_link">
      <visual>
        <origin xyz="0.0 0.0 0.0" rpy="1.57079 0.0 0.0"/>
        <geometry>
          <cylinder radius="0.032" length="0.04"/>
        </geometry>
        <material name="yellow">
          <color rgba="1.0 1.0 0.0 0.8"/>
        </material>
      </visual>
      <xacro:cylinder_inertia m="0.05" r="0.032" h="0.04"/>
    </link>

    <joint name="${wheel_name}_joint" type="continuous">
      <parent link="base_link"/>
      <child link="${wheel_name}_link"/>
      <origin xyz="${xyz}" rpy="0.0 0.0 0.0"/>
      <axis xyz="0 1 0"/>
    </joint>
  </xacro:macro>
</robot>

<!------------------------------使用------------------------------>
<xacro:wheel_xacro wheel_name="..." xyz="...">

```

每个组件都如此编写，之后再将它们组合：

```xml
<?xml version="1.0"?>
<robot xmlns:xacro="https://www.ros.org/wiki/xacro" name="fish_robot">
<!--这里include的功能就是引入文件，需要传入路径，使用find即可查找（注意是小括号）-->
<xacro:include filename="$(find robot_description)/urdf/fishbot/base.urdf.xacro"/>

<xacro:include filename="$(find robot_description)/urdf/fishbot/sensor/imu.urdf.xacro"/>
<xacro:include filename="$(find robot_description)/urdf/fishbot/sensor/camera.urdf.xacro"/>
<xacro:include filename="$(find robot_description)/urdf/fishbot/sensor/laser.urdf.xacro"/>

<xacro:include filename="$(find robot_description)/urdf/fishbot/actuator/wheel.urdf.xacro"/>
<xacro:include filename="$(find robot_description)/urdf/fishbot/actuator/caster.urdf.xacro"/>

<xacro:base_xacro length="0.12" radius="0.10"/>
<xacro:imu_xacro xyz="0.0 0.0 0.0"/>
<xacro:camera_xacro xyz="0.10 0.0 0.075"/>
<xacro:laser_xacro xyz="0.00 0.0 0.10"/>

<xacro:wheel_xacro wheel_name="left_wheel" xyz="0.0 0.10 -0.06"/>
<xacro:wheel_xacro wheel_name="right_wheel" xyz="0.0 -0.10 -0.06"/>
<xacro:caster_xacro caster_name="front_wheel" xyz="0.08 0.0 -0.076"/>
<xacro:caster_xacro caster_name="back_wheel" xyz="-0.08 0.0 -0.076"/>

</robot>
```



相对而言结构就清晰多了

但是rviz是无法识别xacro格式的，我们使用xacro命令可以将其转化为urdf格式并输出

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762673048882-8f3052a8-bb3d-4f52-976d-871b6ebf1468.png)

于是，我们只需要更换launch文件中的路径和命令就可以了

```python
def generate_launch_description():

    urdf_package_path=get_package_share_directory('robot_description')
    default_urdf_path=os.path.join(urdf_package_path,'urdf','first_robot.xacro') #changed
    action_declare_arg_mode_path=launch.actions.DeclareLaunchArgument(
        name='model',default_value=str(default_urdf_path),description='Path of the modle file'
    )

    #通过文件路径获取内容
    substitutions_command_result=launch.substitutions.Command([
        'xacro ',#changed
        launch.substitutions.LaunchConfiguration('model')
    ])
    ......#后面的内容无需更改
```

运行`ros2 launch ...`的效果：

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762673509889-446c4423-4275-45cf-85d6-a147407eb27c.png)

在此之后，此时我们的机器人还之后visual标签，只是一个能看见的幽灵，还需要为其添加实体属性，好在目前我们只需要实体和视觉保持一致，所以只需要添加<collision>标签，里面的内容复制visual里的就行，不再展示

（完整代码在 [github仓库](https://github.com/Wublabdubdubs/HelloCV) Ros_WS/robot_description/fishbot 目录下）



## world表示世界
有了机器人，我们还需要机器人所处的仿真环境，这个就简单多了

首先命令行直接输入`gazebo`运行gazebo，最上面可以拖入简单的正方体，球，圆柱

我们还可以点左上角`edit > building edit`来自己创建房间



![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762674747974-a827655a-04d3-44ff-bd28-f481d28c2912.png)

之后左上角 保存 > 另存为... 即可保存，但为了模拟更真实的世界，这些肯定是不够的，幸好gazebo为我们提供了一些模型：

首先创建gazebo的默认模型目录：

`mkdir -p ~/.gazebo & cd ~/.gazebo`

之后将模型下载下来：

`git clone https://gitee.com/ohhuo/gazebo_models.git ~/.gazebo/models`

之后记得把`.git`删掉，防止被误识别为模型

## 将机器人放进环境
依然是使用launch文件

```python
import launch
i
mport launch.launch_description_sources
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
    #以上内容和之前一样，启动robot_state_publisher
    #但是这里我们不再需要joint节点，因为gazebo起到joint的作用，将link之间的TF关系发布

    #利用gazebo自带的launch文件启动gazebo，并传入世界参数
    action_launch_gazebo=launch.actions.IncludeLaunchDescription(
        launch.launch_description_sources.PythonLaunchDescriptionSource(
            [get_package_share_directory('gazebo_ros'),'/launch','/gazebo.launch.py']
        ),
        launch_arguments=[("world",default_gazebo_world_path),('verbose','true')]
    )
    #使用gazebo提供的节点，依据/robot_description这个topic在gazebo中创建机器人
    action_spawn_entity =launch_ros.actions.Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=['-topic','/robot_description','-entity','fishbot']
    )
    return launch.LaunchDescription([
        action_declare_arg_xacro_path,
        action_robot_state_publisher,
        action_launch_gazebo,
        action_spawn_entity
    ])

```

于是我们得到了机器人在仿真环境中的样子：

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762685482731-145dc637-5085-450a-aede-8f1d354455d7.png)

注意，一开始机器人是纯白的，这是因为我们使用的是urdf文件，而gazebo会采用sdf格式，在转化过程中颜色信息丢失了，图中雷达部分是黑色的这是因为在雷达部分添加了：

```xml
<!--gazebo标签就是给gazebo看的标签，可以在转化后传递消息，此例中为调整部件为黑色-->
<gazebo reference="laser_cylinder_link">
  <material>Gazebo/Black</material>
</gazebo>
<gazebo reference="laser_link">
  <material>Gazebo/Black</material>
</gazebo>
```

## 控制小车移动
在移动前，我们还需要给小车添加一些属性，比如摩擦系数，刚性，这里以与移动相关的轮子为例：

```xml
<!--caster作为万向轮，起支撑作用，我们把它的摩擦力设为零即可-->
        <gazebo reference="${caster_name}_link">
            <mu1 value="0.0"/>  <!--切向摩擦-->
            <mu2 value="0.0"/>  <!--法向摩擦-->
            <kp value="1000000000.0"/>
            <kd value="1.0"/>
        </gazebo>
<!--wheel是移动轮。，需要有一定的摩擦系数-->
        <gazebo reference="${wheel_name}_link">
            <mu1 value="20.0"/>
            <mu2 value="20.0"/>
            <kp value="1000000000.0"/>
            <kd value="1.0"/>
        </gazebo>
```

之后就要想办法让小车移动了

移动这样复杂的操作需要用到插件（plugin）

这里所谓的插件事实上是一个动态链接库，它会解析插件标签内的内容，并运行来达成我们需要的效果

例（来自鱼香ros仓库）：

```xml
<?xml version="1.0"?>
<robot xmlns:xacro="http://www.ros.org/wiki/xacro">
    <xacro:macro name="gazebo_control_plugin">
        <gazebo>
            <!-- 两轮差速插件，下面的.so文件就是所谓的插件 -->
            <!-- 正如前面所说，这里的程序会解析下面的标签，所以每个标签的含义都是确定的 -->
            <plugin name='diff_drive' filename='libgazebo_ros_diff_drive.so'>
                <ros>
                    <!-- 使用默认命名空间，重定向不变 -->
                    <namespace>/</namespace>
                    <remapping>cmd_vel:=cmd_vel</remapping>
                    <remapping>odom:=odom</remapping>
                </ros>
                <update_rate>30</update_rate>
                <!-- wheels -->
                <left_joint>left_wheel_joint</left_joint>
                <right_joint>right_wheel_joint</right_joint>
                <!-- kinematics -->
                <wheel_separation>0.2</wheel_separation>
                <wheel_diameter>0.064</wheel_diameter>
                <!-- limits -->
                <max_wheel_torque>20</max_wheel_torque>
                <max_wheel_acceleration>1.0</max_wheel_acceleration>
                <!-- output -->
                <publish_odom>true</publish_odom>
                <publish_odom_tf>true</publish_odom_tf>
                <publish_wheel_tf>true</publish_wheel_tf>

                <odometry_frame>odom</odometry_frame>
                <robot_base_frame>base_footprint</robot_base_frame>
            </plugin>
        </gazebo>
   </xacro:macro>
</robot>
```

之后再启动gazebo，就可以使用`ros2 run teleop_twist_keyboard teleop_twist_keyboard `开始键盘操控了，`awdx键`控制速度，`uoijklm,.键`控制方向

原理方面，可以通过反复`ros2 topic/node info`查得：

当我们使用命令行发送命令，`/teleop_twist_keyboard`节点会通过话题`/cmd_vel`发送信息

而`/diff_drive`会订阅`/cmd_vel`接收信息，同时再通过`/odom`和`/tf`话题改变小车的相关信息

## 为小车添加传感器
传感器同样需要使用插件进行实现，无论是雷达，imu,还是相机，都有ROS2提供的现成插件，我们再根据参考文档调整参数即可，同样使用鱼香ROS的代码为例：

```xml
<?xml version="1.0"?>
<robot xmlns:xacro="http://www.ros.org/wiki/xacro">
    <xacro:macro name="gazebo_sensor_plugin">
        <gazebo reference="laser_link">
            <sensor name="laserscan" type="ray">
                <plugin name="laserscan" filename="libgazebo_ros_ray_sensor.so">
                    <ros>
                        <namespace>/</namespace>
                        <remapping>~/out:=scan</remapping>
                    </ros>
                    <output_type>sensor_msgs/LaserScan</output_type>
                    <frame_name>laser_link</frame_name>
                </plugin>
                <always_on>true</always_on>
                <visualize>true</visualize>
                <update_rate>5</update_rate>
                <pose>0 0 0 0 0 0</pose>
								<!-- 激光传感器配置 -->
                <ray>
                    <!-- 设置扫描范围 -->
                    <scan>
                        <horizontal>
                            <samples>360</samples>
                            <resolution>1.000000</resolution>
                            <min_angle>0.000000</min_angle>
                            <max_angle>6.280000</max_angle>
                        </horizontal>
                    </scan>
                    <!-- 设置扫描距离 -->
                    <range>
                        <min>0.120000</min>
                        <max>8.0</max>
                        <resolution>0.015000</resolution>
                    </range>
                    <!-- 设置噪声 -->
                    <noise>
                        <type>gaussian</type>
                        <mean>0.0</mean>
                        <stddev>0.01</stddev>
                    </noise>
                </ray>
            </sensor>
        </gazebo>

        

<gazebo reference="imu_link">
    <sensor name="imu_sensor" type="imu">
        <plugin name="imu_plugin" filename="libgazebo_ros_imu_sensor.so">
            <ros>
                <namespace>/</namespace>
                <remapping>~/out:=imu</remapping>
            </ros>
            <initial_orientation_as_reference>false</initial_orientation_as_reference>
        </plugin>
        <update_rate>100</update_rate>
        <always_on>true</always_on>
        <!-- 六轴噪声设置 -->
        <imu>
            <angular_velocity>
                <x>
                    <noise type="gaussian">
                        <mean>0.0</mean>
                        <stddev>2e-4</stddev>
                        <bias_mean>0.0000075</bias_mean>
                        <bias_stddev>0.0000008</bias_stddev>
                    </noise>
                </x>
                <y>
                    <noise type="gaussian">
                        <mean>0.0</mean>
                        <stddev>2e-4</stddev>
                        <bias_mean>0.0000075</bias_mean>
                        <bias_stddev>0.0000008</bias_stddev>
                    </noise>
                </y>
                <z>
                    <noise type="gaussian">
                        <mean>0.0</mean>
                        <stddev>2e-4</stddev>
                        <bias_mean>0.0000075</bias_mean>
                        <bias_stddev>0.0000008</bias_stddev>
                    </noise>
                </z>
            </angular_velocity>
            <linear_acceleration>
                <x>
                    <noise type="gaussian">
                        <mean>0.0</mean>
                        <stddev>1.7e-2</stddev>
                        <bias_mean>0.1</bias_mean>
                        <bias_stddev>0.001</bias_stddev>
                    </noise>
                </x>
                <y>
                    <noise type="gaussian">
                        <mean>0.0</mean>
                        <stddev>1.7e-2</stddev>
                        <bias_mean>0.1</bias_mean>
                        <bias_stddev>0.001</bias_stddev>
                    </noise>
                </y>
                <z>
                    <noise type="gaussian">
                        <mean>0.0</mean>
                        <stddev>1.7e-2</stddev>
                        <bias_mean>0.1</bias_mean>
                        <bias_stddev>0.001</bias_stddev>
                    </noise>
                </z>
            </linear_acceleration>
        </imu>
    </sensor>
</gazebo>

    </xacro:macro>


<gazebo reference="camera_link">
    <sensor type="depth" name="camera_sensor">
        <plugin name="depth_camera" filename="libgazebo_ros_camera.so">
            <frame_name>camera_optical_link</frame_name>
        </plugin>
        <always_on>true</always_on>
        <update_rate>10</update_rate>
        <camera name="camera">
            <horizontal_fov>1.5009831567</horizontal_fov>
            <image>
                <width>800</width>
                <height>600</height>
                <format>R8G8B8</format>
            </image>
            <distortion>
                <k1>0.0</k1>
                <k2>0.0</k2>
                <k3>0.0</k3>
                <p1>0.0</p1>
                <p2>0.0</p2>
                <center>0.5 0.5</center>
            </distortion>
        </camera>
    </sensor>
</gazebo>


</robot>
<!-- 在主程序中：-->
  <xacro:gazebo_sensor_plugin/>
```

这些插件会将结果通过话题发送出来，我们可以使用Rviz订阅相关话题来查看运行结果：

点击左下角add，选择By topic，就可以使用Rviz可视化结果了

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762687674432-1ae96d16-df5a-4ca7-bf99-ea65d6c4ec4c.png)

比如使用深度相机获得的点云：

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762687853675-859e83f3-b191-4236-98f0-656dfac0957c.png)

和仿真中的实际情况符合地很好：

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1762687978767-cb1efa33-a28b-400e-9271-aae2a69d4040.png)

