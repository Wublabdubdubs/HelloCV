参考资料：

[Linux下CMake简明教程_linux cmake-CSDN博客](https://blog.csdn.net/whahu1989/article/details/82078563)

## 什么是CMake
一种跨平台的编译工具，可以管理大量复杂程序之间的编译关系（先后，次数，时间）

具体而言，有一个叫做MakeFile的文件，可以指导程序的编译，而CMake可以用来生成这个文件

## 安装
Linux环境下安装CMake极其简单：

`sudo apt install cmake`

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760792114135-93eafcdd-93bc-442c-85d9-88a6ab8f2a52.png)

## 使用
首先进行一个最简单的试用

创建文件A.cpp随便写点啥（比如hello world)

在同目录下创建 CMakeLists.txt，内容如下

```plain
cmake_minimum_required(VERSION 3.22)
project(demo)
add_executable(main A.cpp)
```

第一句表明了CMake最低版本限制（不可省略，否则报错）

第二句表示项目名称

第三句表示项目内容，即：最终要生成文件"main"，用到的是 A.cpp

之后终端cd到文件所在目录运行cmake`cmake .`之后可以看到生成的cmake相关的一系列文件包括我们需要的makefile，之后只需要在命令行输入`make`即可依据makefile生成我们需要的文件

最后文件夹大概长这样：（其中两个以B命名的文件是我之后写的）

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760838223146-56381c22-383d-445c-8971-a60e79b4c2c9.png)

如果有多个文件想一起编译怎么办呢？把它们都写入第三行括号里就行：

```plain
cmake_minimum_required(VERSION 3.22)

project(demo)

add_executable(main A.cpp B.cpp)
```

那如果文件变得更多，我们不可能把它们一个一个写进来吧？

`aux_source_directory`可以把一个文件夹下所有源文件存入一个变量，之后只要传入这个文件，就可以了

但我们不是一直都会想要编译一个文件夹内的所有文件的`set(变量名 源文件列表)`是更常规的做法

```plain
cmake_minimum_required(VERSION 3.22)

project(demo)

aux_source_directory(. SRC_LIST) # "."表示当前目录
set(SRC_LIST A.cpp B.cpp C.cpp) #在本例子中，这句话和上面的一句一样，只写其中一句即可
#如果你同时写了5，6行句子也没有问题，LRC_LIST作为变量，自然是可以被修改的

add_executable(main ${SRC_LIST})
```

在实际工作中，我们一般不会把所有文件放到同一层目录下

![这才是更常见的情况](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760840993716-c6e028f5-9d23-49f8-bdaf-f3558bab1efd.png)

```plain
cmake_minimum_required(VERSION 3.22)

project(demo)

aux_source_directory(./Part_B LIST_B)
aux_source_directory(./Part_C LIST_C)

add_executable(main A.cpp ${LIST_B} ${LIST_C})
```

然后，再测试一下：

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760841287928-ae3c0066-bbd5-424d-a08a-e62d8e093f2a.png)

欸？

是的，当不在一个目录下时，我们的A.cpp找不到头文件了，这很好解决，修改一下：

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760841467390-d46dbf9c-b1af-4d61-810d-ca3459d4a88e.png)

但是，有点丑？

我们其实也可以通过修改CMakeList.txt来解决

我们可以添加：`include_directories(./Part_B ./Part_C)`

它的功能就是字面意思，添加执行include命令时寻找的目录，多个路径空格隔开即可



以上就是Cmake最基础的操作啦！

## 正规一点
让我们看看我们的文件夹

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760842663383-b5886268-fe83-48dd-9776-1d0569a1561a.png)

简直是个灾难

事实上，在一般的项目管理中文件结构长这样

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760843193212-db42e2f7-d217-43ae-8bde-449a57c9f727.png)

include用来存放库，src用来存放源代码

按照我们之前的理解CMAakeLists.txt长这样：

```plain
cmake_minimum_required(VERSION 2.8)

project(demo)

include_directories (./include)

aux_source_directory(./src SRC_LIST)

add_executable(main ${SRC_LIST})
```

但是大批的Cmake配置文件还是会出现在根目录，问题还没解决

我们的build文件夹就是为了这个，我们cd到build里，`cmake ..`

但我们的产出文件还在和配置文件混在一起，因此需要加一句，让CMakeLists.txt变成这样:

```plain
cmake_minimum_required(VERSION 2.8)

project(demo)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories (./include)

aux_source_directory(./src SRC_LIST)

add_executable(main ${SRC_LIST})
```

set我们前面说过，给变量赋值，其中

`EXECUTABLE_OUTPUT_PATH`是自带变量，表示结果的位置，

`PROJECT_SOURCE_DIR`也是自带变量，表示项目根目录

于是，大功告成，赏心悦目

## 库相关
什么是库？

库是一些程序运行时可以调用的已有代码，有我们熟悉的iostream/cstdio等等，也可以是自己安装的第三方库(opencv)或自己写的库，分为静态和动态

静态库是在程序编译后直接连在可执行文件里的，动态库是随着调用再被编译执行的

因此它们也就有了各自的优劣：

静态库性能更好而占用空间较大且不易更新，动态库则相反

如果需要创建自己的库，可以使用`add_library()`

而我们目前主要用第三方库（opencv）为例

因为cmake官方已经为我们定义好了opencv相关的一些基础变量，我们可以直接使用

```plain
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(main ${OpenCV_LIBS})
```

这三句代码即可解决opencv库的调用，

`find_package`可以自动找到第三方库的位置，并将相关的信息存进变量，为下文作准备

REQUIRED的意思是没有这个库程序无法运行，如果没找到cmake会停止并报错

`include_directories`将opencv库路径添加进库文件搜寻目录

`target_link_libraries` 将opencv库链接进程序

