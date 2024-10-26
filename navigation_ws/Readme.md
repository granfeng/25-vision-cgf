# ***任务汇报***
## ***BT 一、gazebo和rviz***
### ***1. 知识+实现逻辑梳理***
1. rviz2和gazebo区别
    +  rviz2是已有数据的情况下，把数据显示出来而以，而gazebo仿真软件是通过模拟真实环境产生数据
    +  Gazebo是独立的软件，gazebo_ros_pkgs将Gazebo和ROS2连接起来。（插件）
2. URDF统一机器人描述格式，URDF使用XML格式描述机器人文件。（建模）
    + xacro语法，让机器人模型文件具有可编程能力.(优化)
3. 好用的插件
    + ros snippets
    + xml tools
4. 实现逻辑
    + 建模（车模、传感器模型）
    + launch文件
    + 联动rviz、gazebo
    + 更新cmakelists、xml文件
    + 编译运行

### ***2. 实现遇到的困难or提升空间***
1. catkin包移植到ros2上，比较复杂（次级困难，理解文本工作量大）
2. py语言基础不好，launch读起来很费劲
3. 找到的教程大部分是py的，很少ament_cmake，资料零散
4. ***joint_state_publisher突然显示找不到***(重大困难，直接阻塞)
5. 可以用solidworks建一个更好看的小车模型
6. 了解小车要多少个雷达可以达到最好的探测效果



## ***BT 二、ros分布式通信***
### ***1. 知识+实现逻辑梳理***
#### 1. 
   + 

### ***2. 实现遇到的困难***


## ***BT 三、pcl点云库***
### ***1. 知识+实现逻辑梳理***
#### 1. 
   + 

### ***2. 实现遇到的困难***



## ***BT 四、tf坐标系转换***
### ***1. 知识+实现逻辑梳理***
#### 1. 
   + 

### ***2. 实现遇到的困难***


## ***ZT 一、SLAM***
### ***1. 知识+实现逻辑梳理***
#### 1. 
   + 

### ***2. 实现遇到的困难***


## ***ZT 二、规划***
### ***1. 知识+实现逻辑梳理***
#### 1. 
   + 

### ***2. 实现遇到的困难***




## ***Reference***
### 1.学习资料
 + ros2入门资料
   + https://fishros.com/d2lros2/#/
   + https://www.bilibili.com/video/BV1gr4y1Q7j5?spm_id_from=333.788.videopod.episodes&vd_source=8e6c4903bf3dd43b18ac0991f35e7ddc&p=95
   + https://blog.csdn.net/weixin_45897172/article/details/136342381?spm=1001.2014.3001.5502
   + https://blog.csdn.net/weixin_45897172/article/details/137723655?spm=1001.2014.3001.5502
 + 建模部分和xacro学习
   + https://blog.csdn.net/weixin_45897172/article/details/138439323?spm=1001.2014.3001.5502
 + 