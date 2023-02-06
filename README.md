# EECS

 ![image-20220807195012677](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220807195012677.png)

- 学而时习之，大道至简。
- 知识点之间的关联；学习中正向关联建立网络，考虑共性和承接；在问题修复的时候反向连接提供思路；



## 1.怎么认为项目有深度

- 你在这个项目中遇到了什么难点？——方案评估下的常规做法实施的时候碰到了什么阻碍？
  - 如单线UART通信的风险评估；

- 你是如何解决这个难点的？——先找到常规做法施行时阻碍的根本原因，针对这个问题的应对策略是什么？分析不出来的时候，分析一下同类产品的方案，用他们的方案先试试。
- 在这个项目中你的收获是什么？——找问题过程中对底层的深度剖析，新的组件的使用方法，下次评估方案的时候要考虑到这次实施问题所碰到的问题点，提前避开；更进一步想一下会不会又其他的类似问题在这个项目中被掩盖了，可能存在问题？
- 解决这个难点用了哪个组件或者方法？——新组件是如何解决旧评估方案中遇到的问题？原理是什么？新方案的SOP整理。



## 2.怎么学习一个概念？

- 它出现是为了解决什么问题？——项目总结的时候第四点问的就是这个问题
- 它的官方定义是什么？——多读几遍，甚至抄一遍，不要放过细节
- 想一下是否曾经用过类似的东西，能否进行对比学习？这样会提高学习效率
- 应用它，把它的SOP梳理出来 ——谁调用了它，它又调用了谁？使用的时候需要注意点什么？
- 应用完之后能不能更加具体形象地描述这个概念？——实操之后需要理解它的底层逻辑，一上来就做这个会让归纳内化流于形式



## 3.如何快速上手一门新的编程语言

- 程序的机构化设计规则是怎么样的
- 变量如何定义
- 程序线程规则是怎么样子的
- 函数的写法，函数的调用方法



## 4.如何悟道？

- 首先要记住它，也就是需要背诵，能够脱口而出至少能体现专业度，所以纸笔依旧是一个强大的工具，即使面对计算机科学，不动笔墨不读书；
- 然后要迭代，应用前后对一个概念的理解深度是不一样的，实践后需要反刍才能大道至简，才能简化第一步的记忆内容；——学而时习之，才能温故而知新，要多复习！
- 接着需要从更高的层次上总结，寻找同类概念，进行对比，提升认知水平；
- 。。。待续





## 5.如何刷数据结构

- 背诵数据结构的构造结构体，记不住就反复记，死记，没有元结构体，没有数据结构；——数据结构基石
- 刷简单的算法题，不要浪费太多时间，不懂就看答案——这个原则很重要，一题刷一年没有意义，再而衰，三而竭；
- 记住卡壳的地方，理解它——简化后面的记忆
- 默写——这个操作做完之后，记住这个解题流程，并且就把这道算法题作为基础样本，后面去解决其他问题；
- 重刷——巩固基础数据结构样本



## 6.如何学习嵌入式通信外设

- 首先记住有什么管脚，几根管脚，每根管脚的作用；
- 然后根据通信时序图来反推第一步的结论；
- 用电平模拟的方法，不要调库函数，把通信过程描下来；
- 了解一下大家在这个通信外设上踩过什么坑，这个外设有什么升级版本？——QSPI之类的



## 7.控制变量法

### 7.1 控制变量法的时域问题

example:在室外对绘本台灯进行传输距离测试，首先进行正式服版本的测试，发现传输距离超过60米，符合要求；然后进行驱动修改，升级为功率下调版本，发现此时的通讯距离只有45米。此时能马上得出功率下调之后，传输距离变短吗？

- 从理论上来说是没有问题，事实也很有可能是如此，但是，仍旧不够有严谨。理由如下：

  虽然是同一台机器，但是所处的测试时序是不一样的，不能排除在第二台机器进行测试的时候存在无线电波的非肉眼可见的干扰存在，为了排除这种干扰，得出公正客观的测试结果，应该把上述的实验重复至少一遍才能得出结论。

- 控制变量法的实验结果应该要复现验证来加以确认。

### 7.2 控制哪些变量？

如果无法直接定位问题点，则怀疑全链路。

分组件怀疑所有流程节点。怀疑电子(信号完整性、电平稳定性、电平正确性)、怀疑软件(编程流程正确性、单步Reaction正确性)。

- 硬件

  - 供电是否正常
  - 波形发生器触发的模块输出是否正确

- 软件

  - 编程流程是否有步骤缺失

    - example：外部中断异常的时候分步怀疑编程过程的正确性

      - Step1:初始化用来产生中断的 GPIO；  
      - Step2:初始化 EXTI；  
      - Step3:配置 NVIC；  
      - Step3:编写中断服务函数。

      通过**日志输出**或者、**波形**、或者**点灯**现象分布排查上述所有编程过程是否产生正确的Reaction.

### 7.3 变量取极限检验控制效果

example:验证pathA是影响A路天线功率的参数，把值设置为0和设置为100，分别测试；这个过程重复测试几次(避免时域问题干扰测试结果)；如果pathA为0的时候没有A路天线没有发射功率，而pathA为100时发射功率超标，信号性能极好；则可以证明调节pathA可以控制天线发射功率。

要不要多机子测试，属于空间问题，一般来说如果量产的机子，一致性会比工程样机好一些，出现异常的概率比较小，时域的控制变量的合格数据一般来说可以铺开到不同的机子上。部分工厂除外，小心驶得万年船，还是要严谨一点免得背锅。



## 8、如何做笔记

不要着急`Ctrl+C、Ctrl+V`，在22年9月的嵌入式备考发现，如果是自己整理过的文档，复习的效率极高，如果是copy来的文案，即使有印象，复习起来的效率并不高，甚至是有点重新学的感觉。本质原因是对这个内容的熟悉程度不够，如果天天复习，那即使是别人的笔记，也可以在备考的时候上手很快，有点类似高考。

Lesson Learn：

- 好的文档先记录位置，不要直接cp，cp完就不会再去看了，容易鸵鸟式地认为这个东西就是自己的，其实不是；
- 费曼学习法，学习完了用自己的话给别人讲述，通俗的讲述，如果不能让一个高中生听懂你讲的是什么，那就是自己没理解透；计算机科学源自生活，在生活中有模型，模型挖掘，建立理科的感性认知；
  - example：内存的缓存机制和我们把常用网页的地址放在标签栏的做法是一样的。
- **实际操作。** 实践是检验真理的唯一标准。
- 完成费曼学习法和实际操作后再去进行知识点的总结，这样的东西才算是内化了。
- 如果不能达到以上的步骤，就要把copy来的知识点，频繁地进行复习，programed it into your mind，烧录进你的大脑。
