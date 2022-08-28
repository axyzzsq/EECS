# 状态机的引入

## 第1章 单片机初试牛刀

### 1.1 一个LED灯闪烁的故事

​		用单片机实现1个LED闪烁功能，要求LED亮500ms, 灭300ms。这个问题对于学过单片机的人来说是一个入门级的东西，实现起来简单的不能再简单了，就像C语言入门时的“HelloWorld”。好让我们来实现以下这个功能。

```C
int main()
{
    while(1)
    {
         LedOn();
         DelayMs(500);
         LedOff();
         DelayMs(300);
    }

```

### 1.2 两个LED灯同时闪烁的故事

​		根据上一节内容我们让LED完成了闪烁功能，在这一节当中，我们来让两个LED完成同时闪烁的功能，这对于一些工程来讲，还是一样入门级的东西，好，让我们来一起实现它。

```C
int main()
{
    while(1)
    {
         Led1On();
         Led2On();
         DelayMs(500);
         Led1Off();
         Led2Off();
         DelayMs(300);
    }
}
```

### 1.3 两个LED灯不同时闪烁的故事

​		上面两个例子，看起来实际上是一个故事，因为他们只是在延时的前后加上了LED的开关动作。这一节我们来尝试一下，让两个LED不同时闪烁，比如，LED1开200ms，关500ms，

​		LED2开300ms，关400ms。乍得一听，不知如何下手。对，有的小伙伴可能想起来了，用定时器来实现啊，嗯，没错可以这样做。那好我们来试试吧。在看下面的代码之前建议看到的小伙伴，想一想如果让你来自己实现这一功能你会怎么实现。
```C
#define LED_ON (unsigned char)(0x01)
#define LED_OFF (unsigned char)(0x00)
#define LED1_ON_TIME_MS (unsigned short)(200)
#define LED1_OFF_TIME_MS (unsigned short)(500)
#define LED2_ON_TIME_MS (unsigned short)(300)
#define LED2_OFF_TIME_MS (unsigned short)(400)
int main()
{
	/*初始化定时器中断间隔为*/
	TimerMsInit(1);
	while(1);
}
 
/*定时器中断函数*/
void TimerMsInterrupt(void)
{
	if(Led1Flag == LED_ON)
	{
		if(Led1Count > LED1_ON_TIME_MS)
		{
			Led1Count = 0;
			Led1Off();
			Led1Flag = LED_OFF;
		}
	} 
	else
	{
		if(Led1Count > LED1_OFF_TIME_MS)
		{
			Led1Count = 0;
			Led1On();
			Led1Flag = LED_ON;
		}
	}
	
	if(Led2Flag == LED_ON)
	{
		if(Led2Count > LED2_ON_TIME_MS)
		{
			Led2Count = 0;
			Led2Off();
			Led2Flag = LED_OFF;
		}
	} 
	else
	{
		if(Led2Count > LED2_OFF_TIME_MS)
		{
			Led2Count = 0;
			Led2On();
			Led2Flag = LED_ON;
		}
	} 
	Led1Count++;
	Led2Count++;
}
```

​		好，到这里我们已经把两个LED不同时闪烁的代码实现了，所有小伙伴都是想这么实现吗？或者还是在看到这段段码的时候根本没有想到怎么实现？其实在中断函数中已经有了“状态机”的模型，只是不太容易看出来。很多写单片机程序的人不太知道状态机是干什么的，不要着急，我们在下一个章节会详细介绍。其实到这里还是没有引出我们的重点内容，好吧，只能给聪明的小伙伴留一个思考题了。如果思考题没有想出来可以带着疑问去看第2章内容。

### 1.4 思考题

​		单片机有串口1个，灯2个分别为LED1和LED2，按键2个分别为KEY1和KEY2。要求如下：

1.  在KEY1按下的一瞬间，串口发送”KEY1 DOWN”

2.  在KEY1 按下时，LED1亮起，KEY1按下的时间长短任意。如果KEY1一直处于按下状态那么LED1一直亮起。

3.  在KEY1抬起的一瞬间，串口发送”KEY1 UP”

4.  在KEY1抬起时，LED1熄灭

5.  在KEY2按下的一瞬间，串口发送”KEY2 DOWN”

6.  在KEY2 按下时，LED2亮起，KEY2按下的时间长短任意。如果KEY2一直处于按下状态那么LED2一直亮起。

7.  在KEY2抬起的一瞬间，串口发送”KEY2 UP”

8.  在KEY2抬起时，LED2熄灭

9.  两个按键按下抬起，互不影响，且能立即反映做出动作。(最重要的要求)

10.  为了简化问题，这里不考虑按键去抖。

​    

## 第2章 状态机的通俗解释

### 2.1状态机的概念

​		按照最标准的概念介绍状态机可能很多人会不理解，所以想来想去还是用最通俗的语言来解释这一概念，配合一个简单的例子来介绍。

​		其实，状态机就是把一件事情分为几个过程来实现，每个过程对应一个状态。比如一个人一天的生活吧，当然人每天的生活非常复杂，这里我只举几个比较常见的事情，以吃饭为主，这样对于像我这样爱吃的小伙伴可能更容易理解。好了，来大致介绍一下吧，比如生活包括做饭、吃饭、洗碗、睡觉、看电视、散步。其中这些都是生活中的某一状态那么我们画一个状态图来理解一下吧。

![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20200808113330211.png)

​		通过上图可以看到，椭圆中就代表一个状态，饿了、吃饱了等代表进入到下一个状态的条件，也就是说满足一个条件就可以进入到下一个状态。

### 2.2 状态机具体化

​		好了，有了上一节的描述，小伙伴们对状态机应该有了一个模糊的概念，那么下面我们来将上一章的思考题，具体的变为几个状态，根据实际情况来画一下状态图。

​		对于上一节的思考题，LED1，KEY1和LED2，KEY2可以看做相同的情况，那么我们就用其中一个来说明。

- [ ] ![image-20220828234030554](https://second-1304959529.cos.ap-nanjing.myqcloud.com/folder/202208290010787.png)


### 第3章 状态机在单片机上的应用

### 3.1 代码实现步骤

### 3.2 应用代码详解

## 第4章 简单的举例

### 4.1 实例概述

### 4.2 示例状态图

### 4.3 ADC部分实现

### 4.4 总结

