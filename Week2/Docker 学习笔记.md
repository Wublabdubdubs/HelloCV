本文主要参考[这篇文章](https://blog.csdn.net/leah126/article/details/131871717)

## Docker的简介
（参考B站某个视频）

想象这样一个场景：

你写完了一个有趣的C++程序，想分享给你的一个朋友，他很开心地接受了

然后，他的电脑没有运行c++所需要的环境，你带着他装了mingw

问题解决了？没有，他的电脑是Linux系统的，你的windows.h被ban了，于是你们浪费了半个下午，啥都没干

虽然上述例子有些不太合理，但它描述了一个常见的情形：你写的东西在别人的环境下往往难以正常运行

我们好像已经有一个解决方案了：虚拟机

但。。。为了一个小项目装个虚拟机，既麻烦又低效，于是，一个更好的解决方案诞生了：**Docker**

Docker 去除了虚拟机对于硬件的模拟，而仅仅是携带了程序运行所需要的环境依赖，仅需几秒即可快速部署

## 安装(Ubuntu 22.04环境)
打开官网，按照教程，首先下载 Docker 的 apt 仓库（注释由本人依次查询菜鸟教程）

```bash
# Add Docker's official GPG key:
sudo apt-get update #更新软件库
sudo apt-get install ca-certificates curl #安装ca-certificates包
sudo install -m 0755 -d /etc/apt/keyrings #按照755格式创建目录
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg -o /etc/apt/keyrings/docker.asc #下载文件到-o后的目录
sudo chmod a+r /etc/apt/keyrings/docker.asc #将文件权限设置为对所有用户可读

# Add the repository to Apt sources:
echo \
  "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "${UBUNTU_CODENAME:-$VERSION_CODENAME}") stable" | \
  sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get update
```

依据官网的指南，执行完这些，docker就在运行中了，可以通过`sudo systemctl status docker`验证这一点

然后让我们抓一个简单的容器看看`sudo docker run hello-world`

欸？？？

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760758370803-038af0e4-4f41-44fb-bfb1-a82ceb842918.png)

以下内容参考 [这篇文章](https://zhuanlan.zhihu.com/p/28662850275)

事实上，如果按照官网的链接流程，会连到默认的仓库，但是，电子长城是没有安检口的，所以，可能会连接超时(虽然事后发现上图实际原因是没有权限即没写sudo)

因此，我们有必要将其加速——使用国内的镜像下载源

换源有两种方式

1. 临时换源 （别急着跳过啊！)

刚刚下载完Docker的我们可以先临时换源来检测它是否正常运行，也能为接下来的成功奠定心理基础

格式为`sudo docker pull+镜像源地址+/+要拉取的镜像名`

具体来讲`sudo docker pull docker.m.daocloud.io/hello-world`

即用`docker.m.daocloud.io`这个源来获取`hello-world`这个容器

如果运行成功了，那证明Docker没有问题，就可以放心进行下面的操作

![成功的参考标准](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760772030086-f504c3b0-bcc6-4a28-b860-3e124936c5e0.png)

2. 永久换源

这个需要更改Docker的配置文件

首先创建其配置的目录`sudo mkdir -p /etc/docker`

然后更改配置文件的内容

```bash
sudo tee /etc/docker/daemon.json <<-'EOF'
{
    "registry-mirrors": [
    "https://docker.m.daocloud.io",
    "https://docker.imgdb.de",
    "https://docker-0.unsee.tech",
    "https://docker.hlmirror.com",
    "https://cjie.eu.org"
    ]
}
EOF

```

不要担心原来没有这个文件，它会帮你自动创建

之后需要

```plain
sudo systemctl daemon-reload 
sudo systemctl restart docker
```

以确保正常使用

3. 怎么还有一条

阿里云镜像比较麻烦，我单开一条

首先注册并登录阿里云账号 [阿里云官网](https://www.aliyun.com/)

然后找到<font style="color:rgb(24, 24, 24);">容器镜像服务 ACR</font>

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760775913106-318503fe-f68d-4a53-a08a-b48dc6b13dc5.png)

管理控制台->创建个人版实例->完善认证，创建完是下面这样

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760775989176-97f0f6c3-d8f6-49b7-a38f-fa007c520613.png)

然后点开左侧镜像工具->镜像加速器

你会看到你的加速器地址，将其复制添加到上述配置文件里就行了

4. （如果你还是用不了）随着我国政策不断完善，人民群众的安全感不断提高，越来越多被称为镜像源的危险被阻挡在高墙之外，鼓励我们秉持创新精神，发奋刻苦，不断寻找更适合自己的镜像下载新方式！！！

到此为止，大功告成，我们可以像正常人一样使用docker了！

## 使用
了解如何使用Docker就像了解如何使用虚拟机一样，我们需要掌握它提供的命令（遇到不能执行的可以在前面添加`sudo`试试

1. `docker version` 查看版本信息
2. `docker info` 查看docker的信息，比如容器和镜像的数量
3. `docker image` 查看 **本地** 镜像列表（`-a` 查看全部/`-q` 仅显示id）
4. `docker search 镜像名` **在线** 搜索镜像
5. `docker pull 镜像名` 下载镜像
6. `docker rmi 镜像名` 删除镜像

有了镜像，才可以创建容器，所以首先下载镜像(以Linux 的 centos 为例)

7. `docker run image` 新建容器并启动

参数

    1. `--name="名字"`指定容器名
    2. `-d`   后台运行
    3. `-it` 交互运行

例子：![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760789578457-b878e541-5e01-49db-8e93-142b95e50d5f.png)

下面的井号就表示进入了centos的bash，即容器中



8. `docker ps` 列出正在运行的容器
9. （容器内）

`exit` 退出容器并停止

`ctrl+P+Q` 退出容器但是不停止

10. `docker start 容器id` 启动

`docker restart 容器id` 重启

`docker stop 容器id` 停止

`docker kill 容器id` 强停止

（都挺符合直觉的，不再解释）

11. `docker top 容器id` 显示容器内的进程
12. `docker exec` 进入正在运行的容器（开启新终端）

`docker attach` 进入容器正在执行的终端

13. `docker cp 容器id:容器路径 主机路径` 拷贝文件到外面

## 原理
其实按照行文逻辑，我们应该提前许多写原理部分，但是实际情况是：上来就讲原理确实吃不消

而现在，我们对容器和镜像已经有了感觉上的认识，可以开始了解原理了。

首先，第一个概念：UnionFileSystem（下文非正式地简称UFS，其实正经缩写是UnionFS）

。。。等等，逛哪里来了，这和前文相关吗？

这就是它最大的特点：你感觉不到

什么是UFS，首先，它是一种FS（FileSystem）——文件系统

那么何为Union？它是将几个文件系统分层管理（比如下载和删除或访问）但是让它们看起来像是同层的文件系统

没听懂？没事，举个例子：

这个例子就是我们的第二个概念：镜像

我们观察一下镜像的下载过程

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760783180392-8adf5cca-4d65-411c-8831-94ee2a30e7fa.png)

图中红框子框起来的东西是几堆不同的文件系统，它们的关系就是分层的，即

第2个文件系统被建立在第1个文件系统的基础上，而第3个文件系统建立在第2个的基础上，他们是依次依赖的（下层文件被上层文件系统共享，而下层文件系统不需要访问上层文件系统），而不是平行存在的

好像还是不够具体？请看教程原例

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760783414467-a0190dcc-01cb-42e2-be02-7bdd7e8409c5.png)

并且，这些原本被分层的文件在使用的时候会向用户略去分层的细节，而令他们看起来更像是一个文件系统，这就是UFS的意思

镜像和容器都是UFS形式构成的，他们的关系是这样的：

镜像是几层只读的文件，我们无法对其进行修改，没法在上面写程序，自然也就无法使用

为了使用，Docker会为我们在它的最上面新建一层，这一层是可读且可写的，而连带这新的一层构成的整体，就是容器。

最上面新建的这一层叫容器层，容器层之下均称为镜像层，我们对容器进行的所有操作都是基于容器层的

## 数据卷 Docker Volume
紧接着上文，既然容器在启动时会新添一层，我们自然会有这样的担心：

我用着用着久而久之它不会越来越大吗？这不浪费吗？

上次建的层不会影响下次运行吗？

其实思考几秒就会得出这样的结论：容器层在使用完会被删除，事实上，Docker也是这么干的

然而，这会带来更大的问题：我数据呢？？？

既然我运行一个程序，它必然有输出啊，容器层没了我输出数据呢？

答案是：也没了。Docker会在关闭容器是“删库跑路”

那咋办？每次都手动把东西粘出来？

Docker为我们提供了办法：数据卷

即：我们将数据存在本地，容器要用就给它用，用完了再把输出吐出到本地，换句话说，就是把一个文件夹共享给容器

**具体而言，操作如下**

`docker run -it -v 主机目录:容器内目录 镜像名 /bin/bash`

效果如图：

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760785751842-5495ef11-3f27-4fb7-871d-de50c79aa65e.png)

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760785793237-34555cd8-8453-423a-bbf9-f349fbfad006.png)

## DockerFile
DockerFile是啥？用来生成镜像的脚本

类比为win环境下的C++

DockerFile 相当于源程序 A.cpp

DockerImages 相当于可执行的 A.exe

DockerContainers 相当于运行中的 A.exe 进程

它的配置很简单（如下图），仅需要配置至多十几个参数：

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760786431260-29328759-532d-4863-9773-760896936fe4.png)

写完上述配置文件（名字自拟），就可以进行镜像的创建了

`docker build -f dockerfile文件路径 -t 镜像名:[tag] .`

注意最后的空格和点，这表示的是当前文件夹的路径，不可省！

正常的话，结果是这样的：

![](https://cdn.nlark.com/yuque/0/2025/png/61501673/1760787805181-dff3f293-bf59-47a2-9ef8-fe8e42f64798.png)

关于网络/接口：

在装好docker的时候，你的电脑就会多一个名叫Docker0的网卡，它是未来所以容器公用的路由器

每运行一个新的容器，就会带来一对接口，他们彼此相连，又各自分别与容器和docker0相连，而docker0与物理网卡相连，作为容器们的路由器，此时，你的docker以及所有的容器都可以通过宿主机连接出去，但是宿主机没法访问docker，此时可以通过`<font style="color:rgba(0, 0, 0, 0.8);">docker run -d -p <宿主机端口>:<容器端口> <镜像名></font>`

<font style="color:rgba(0, 0, 0, 0.8);">在创建容器时，将容器端口映射到主机端口上，主机就可以通过这个端口访问容器了</font>



**特别鸣谢：YMH **提供更加有效的镜像源

