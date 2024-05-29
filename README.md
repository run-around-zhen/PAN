# 实验配置
参考https://github.com/conweave-project/conweave-ns3 中Run NS-3 on Ubuntu 20.04进行配置即可，基本使用在该文档中均可见到。

# 实验图复现
实验数据从云盘下载https://cloud.189.cn/t/3A3muiJNzANn （访问码：6jo5）

将指定的数据复制到mix/output目录下，同时复制.history中的内容

运行analysis/plot_xxx.py画图文件即可

# 代码运行
在run.py中修改如下的地方，修改负载均衡算法、网络负载、网络拓扑，运行后即可生成数据文件
![cdc43310bfb9078440ec4cdcd660ec1](https://github.com/run-around-zhen/gemma/assets/55088145/da984f8b-1301-4c9c-a2a2-c416a1afcd25)
运行画图文件即可

**一些备注：**
以下文件均在src/point-to-point/model文件夹下

只添加了两个文件：
pfc-waring.h/cc:PAN的具体实现，其余的分散在conga.cc/letflow.cc上和一些更上层的代码

**运行Conga：**

在switch-node.cc代码中修改以下两行，一个执行不使用PAN，另外一个使用
![image](https://github.com/run-around-zhen/PAN/assets/55088145/b4462dde-932c-44a6-ab09-55d2bc262f1e)


**运行LetFlow:**

在switch-node.cc代码中修改以下两行，一个执行不使用PAN，另外一个使用
![image](https://github.com/run-around-zhen/PAN/assets/55088145/5686b5c4-5780-4e81-a500-4fc5e26e7c9e)

