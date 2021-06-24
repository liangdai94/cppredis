# cppredis
## 简介

用cpp从头写简单的redis，具体思路参考的是redis0.1版本。这个版本代码量较小，但也能学习到redis的精华，比较适合新手入门。

## 计划

目前只只是初步完成数据库功能，性能情况需要进一步测试。线上效果图：

![image-20210620215828562](https://img-blog.csdnimg.cn/2021062421293896.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2RhaV9fbGlhbmc=,size_16,color_FFFFFF,t_70#pic_center)

只能说是初步能用。具体使用方式：

```shell
make
##打开两个窗口
./server
./client -h 127.0.0.1 -p 9898 ##默认值，读取配置文件功能暂未实现，如需更改可以到源码里更改
```

需要注意的是，此版本的```Makefile```比较简单，修改头文件后make不能生效，需要make clean 再make，后期再完善吧。

对了，看redis0.1时，没有注释看的头疼，也记得加上注释吧，也不知道有没有人看一起交流的。

## 项目概要

### 设计思路

- 采用eventLoop+线程池结构（在我机器上是一个主线程+2个工作线程， 根据每个人的电脑配置不同 工作线程的数量也不同）
- epoll版本的IO复用
- 客户对象的产生与销毁只由主线程处理，尽可能避免竞争的出现
- 主线程维护一个线程安全的队列，主线程生产，工作线程消费
- 数据库目前采用```unordered_map<string, string>```，因此目前功能单一，只实现了set，get的功能，且暂不支持RDB持久化

### 经验总结

- 头文件依赖顺序很重要！！！
- 细节！细节！细节！

## 最后

跪求star！！！欢迎交流与指导！感谢！
