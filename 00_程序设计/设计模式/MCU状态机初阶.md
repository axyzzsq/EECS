# 初阶状态机

## 第1章 单片机初试牛刀

### 1.1 一个LED灯闪烁的故事

用单片机实现1个LED闪烁功能，要求LED亮500ms, 灭300ms。这个问题对于学过单片机的人来说是一个入门级的东西，实现起来简单的不能再简单了，就像C语言入门时的“HelloWorld”。好让我们来实现以下这个功能。

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
}

```

### 1.2 两个LED灯同时闪烁的故事

根据上一节内容我们让LED完成了闪烁功能，在这一节当中，我们来让两个LED完成同时闪烁的功能，这对于一些工程来讲，还是一样入门级的东西，好，让我们来一起实现它。

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

上面两个例子，看起来实际上是一个故事，因为他们只是在延时的前后加上了LED的开关动作。这一节我们来尝试一下，让两个LED不同时闪烁，比如，LED1开200ms，关500ms，LED2开300ms，关400ms。乍得一听，不知如何下手。对，有的小伙伴可能想起来了，用定时器来实现啊，嗯，没错可以这样做。那好我们来试试吧。在看下面的代码之前建议看到的小伙伴，想一想如果让你来自己实现这一功能你会怎么实现。

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

根据上图我们可以看出，一共将整个过程分为了四个状态，分别为检测按键是否按下、LED打开、检测按键是否抬起、串口输出LED关闭。而触发进入到下一状态的条件包括按键按下、处理完成、按键抬起，其中处理完成实际上就是无条件执行的，也就是说，不管怎样，LED打开后就是要进入到等待按键抬起状态。

状态图有什么要求呢？总结以下几点：

- 1 每个状态中不可以有延时。
- 2 必须能够从上一个状态进入到下一个状态，也就是不能永远停留在一个状态。

好了，这一章就说这么多吧，下一章，我们将用代码来实现上面的例子。

## 第3章 状态机在单片机上的应用

### 3.1 代码实现步骤

根据上一章，我们可以看到状态机的状态图，而这一章我们将状态图进一步转化为实际的代码示例。转化的过程实际上只需要定义一个变量来指示整个状态就可以了。另一方面，从状态图中可以看到，如果把整个状态当成一个模块，那么这么模块应该在while(1)中无限循环下去。好了，来看具体的程序代码吧。

### 3.2 应用代码详解

```C
#define KEY_STAT_CHECK_DOWN (unsigned char)(0x00) /*检测按键是否按下状态*/
#define KEY_STAT_CHECK_UP (unsigned char)(0x01) /*检测按键是否抬起状态*/
#define KEY_STAT_DOWN_HANDLE (unsigned char)(0x02) /*按键按下处理*/
#define KEY_STAT_UP_HANDLE (unsigned char)(0x03) /*按键抬起处理*/
unsigned char Key1Stat; /*定义 KEY1 模块的状态变量用于记录状态*/
unsigned char key2stat; /*定义 KEY2 模块的状态变量用于记录状态*/
void Key1Moudle(unsigned char *pstat); /*KEY1 模块处理函数*/
void Key2Moudle(unsigned char *pstat); /*KEY2 模块处理函数*/
void Key1StatCheckDown(unsigned char *pstat); /*KEY1 的按键按下检测函数*/
void Key1StatCheckUp(unsigned char *pstat); /*KEY1 的按键抬起检测函数*/
void Key1StatDownHandle(unsigned char *pstat); /*KEY1 的按键按下处理函数*/
void Key1StatUpHandle(unsigned char *pstat); /*KEY1 的按键抬起处理函数*/
void Key2StatCheckDown(unsigned char *pstat); /*KEY2 的按键按下检测函数*/
void Key2StatCheckUp(unsigned char *pstat); /*KEY2 的按键抬起检测函数*/
void Key2StatDownHandle(unsigned char *pstat); /*KEY2 的按键按下处理函数*/
void Key2StatUpHandle(unsigned char *pstat); /*KEY2 的按键抬起处理函数*/
int main()
{
	while(1)
	{
		/*按键 1 模块处理*/
		Key1Moudle(&Key1Stat);
		/*按键 2 模块处理*/
		Key2Moudle(&Key1Stat);
	}
} 
 
void Key1Moudle(unsigned char *pstat)
{
	/*根据当前状态判断执行哪一个状态处理函数*/
	switch(*stat)
	{
        case KEY_STAT_CHECK_DOWN : 
            Key1StatCheckDown(pstat); 
            break;
        case KEY_STAT_DOWN_HANDLE :
            Key1StatDownHandle(pstat);
            break;
        case KEY_STAT_CHECK_UP :
            Key1StatCheckUp(pstat);
            break;
        case KEY_STAT_UP_HANDLE :
            Key1StatUpHandle(pstat);
            break;
		/*当没有此状态,默认设置为检测按键是否按下状态*/
        default :
            *pstat = KEY_STAT_CHECK_DOWN;
            break;
	} 
	return;
}
 
void Key2Moudle(unsigned char *pstat)
{
	/*根据当前状态判断执行哪一个状态处理函数*/
	switch(*stat)
	{
        case KEY_STAT_CHECK_DOWN :
            Key2StatCheckDown(pstat);
            break;
        case KEY_STAT_DOWN_HANDLE :
            Key2StatDownHandle(pstat);
            break;
        case KEY_STAT_CHECK_UP :
            Key2StatCheckUp(pstat);
            break;
        case KEY_STAT_UP_HANDLE :
            Key2StatUpHandle(pstat);
            break;
		/*当没有此状态,默认设置为检测按键是否按下状态*/
        default :
            *pstat = KEY_STAT_CHECK_DOWN;
            break;
		}
	return;
}
	
void Key1StatCheckDown(unsigned char *pstat)
{
	/*判断按键是否按下*/
	if(ReadKey1() == 0x00)
	{ 	/*将状态设置为 按键按下处理状态*/
		*pstat = KEY_STAT_DOWN_HANDLE;
	} 
	return;
} 
 
void Key1StatDownHandle(unsigned char *pstat)
{
	/*KEY1 按下状态处理*/
	UartSend("KEY1 DOWN");
	Led1On();
	/*处理完成后,设置到 检测按键是否抬起状态*/
	*pstat = KEY_STAT_CHECK_UP;
	return;
} 
 
void Key1StatCheckUp(unsigned char *pstat)
{
	/*判断按键是否抬起*/
	if(ReadKey1() != 0x00)
	{ 	/*将状态设置为 按键抬起处理状态*/
		*pstat = KEY_STAT_UP_HANDLE;
	} 
	return;
}
 
void Key1StatUpHandle(unsigned char *pstat)
{
	/*KEY1 按下状态处理*/
	UartSend("KEY1 UP");
	Led1Off();
	/*处理完成后,设置到 检测按键是否按下状态*/
	*pstat = KEY_STAT_CHECK_DOWN;
	return;
} 
 
void Key2StatCheckDown(unsigned char *pstat)
{
	/*判断按键是否按下*/
	if(ReadKey2() == 0x00)
	{ 	/*将状态设置为 按键按下处理状态*/
		*pstat = KEY_STAT_DOWN_HANDLE;
	} 
	return;
} 
 
void Key2StatDownHandle(unsigned char *pstat)
{
	/*KEY2 按下状态处理*/
	UartSend("KEY2 DOWN");
	Led2On();
	/*处理完成后,设置到 检测按键是否抬起状态*/
	*pstat = KEY_STAT_CHECK_UP;
	return;
} 
 
void Key2StatCheckUp(unsigned char *pstat)
{
	/*判断按键是否抬起*/
	if(ReadKey2() != 0x00)
	{ 	/*将状态设置为 按键抬起处理状态*/
		*pstat = KEY_STAT_UP_HANDLE;
	}
	return;
} 
 
void Key2StatUpHandle(unsigned char *pstat)
{
	/*KEY2 按下状态处理*/
	UartSend("KEY2 UP");
	Led2Off();
	/*处理完成后,设置到 检测按键是否抬起状态*/
	*pstat = KEY_STAT_CHECK_DOWN;
	return;
}
```



## 第4章 简单的举例

### 4.1 实例概述

这一章我们来一个实战的练习，以片外ADC为例子。那么现在来介绍一下片外ADC的采集过程。通常片外单芯片ADC采集。大致过程为：空闲状态->设置ADC开始采集->等待ADC采集完成->读取ADC采集数据，这里需要说明，片外这些ADC不会采集的特别快，采集一次需要的时间远比片上ADC的时间要长,如果单片机单纯的等待ADC采集完成，那么单片机的效率会变极低。所以采用状态机的方式，将ADC作为1个模块来完成。为了体现状态机

方式的编程技巧，在第3章讲述的例子上，加上ADC功能，加上ADC后系统按键反映必须还是非常灵敏，不能出现迟钝等现象，这就要求，ADC模块中不能添加延时。

下面描述一下ADC需要实现功能。ADC需要1s采集一次，采集后的数值保存到变量中。当然，请各位一定要明白，通篇文章中的程序代码都是功能示意性代码，并非完整代码。

### 4.2 示例状态图

![img](https://img-blog.csdnimg.cn/20200808114042683.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2h1b2hvbmdwZW5n,size_16,color_FFFFFF,t_70)

### 4.3 ADC部分实现

```C
#define ADC_ACQUISITION_PERIOD_MS (unsigned short)(1000)/*ADC 采集周期*/
#define ADC_STAT_CHECK_TIME (unsigned char)(0x00) /*检查是否到达 ADC 采集时间*/
#define ADC_STAT_START (unsigned char)(0x01) /*启动 ADC 采集*/
#define ADC_STAT_CHECK_COMPLETE (unsigned char)(0x02) /*检查 ADC 是否采集完成*/
#define ADC_STAT_CHECK_READ_DATA (unsigned char)(0x03) /*读取 ADC 采集的数据*/
 
typedef struct
{
	unsigned short CountMs; /*ADC 采集周期计时*/
	unsigned short AdcData; /*ADC 采集的数据*/
	unsigned char stat; /*ADC 模块状态变量*/
}AdcStructTypeDef;
 
AdcStrcutTypeDef AdcStruct;
 
/*将第 3 章的 main 函数增加 1 条 ADC 模块*/
int main()
{
	while(1)
	{
		/*按键 1 模块处理*/
		Key1Moudle(&Key1Stat);
		/*按键 2 模块处理*/
		Key2Moudle(&Key1Stat);
		/*ADC 模块采集*/
		AdcModule(&AdcStruct);
	}
}
 
 /*定时器 1ms 中断函数*/
void TimerInterruptMs(void)
{
	/*在定时器中， 改变 ADC 的计数变量*/
	pAdcStruct->CountMs++;
}
 
void AdcModule(AdcStrcutTypeDef *pAdcStruct)
{
	/*根据当前状态判断执行哪一个状态处理函数*/
	switch(pAdcStrcut->stat)
	{
        case ADC_STAT_CHECK_TIME :
            AdcStatCheckTime(pAdcStruct);
            break;
        case ADC_STAT_START :
            AdcStatStart(pAdcStruct);
            break;
        case ADC_STAT_CHECK_COMPLETE :
            AdcStatCheckComplete(pAdcStruct);
            break;
        case ADC_STAT_CHECK_READ_DATA :
            AdcStatCheckReadData(pAdcStruct);
            break;
		/*默认情况下设置状态为 ADC 检查是否到采集时间*/
        default :
            pAdcStruct->stat = ADC_STAT_CHECK_TIME;
            break;
	}
} 
 
void AdcStatCheckTime(AdcStrcutTypeDef *pAdcStruct)
{
	if(pAdcStruct->CountMs < ADC_ACQUISITION_PERIOD_MS)
	{ 
		/*采集周期没有到达直接返回*/
		return;
	}
	
	/*清零计时器*/
	pAdcStruct->CountMs = 0;
	/*设置状态为 启动 ADC 采集*/
	pAdcStruct->stat = ADC_STAT_START;
	return;
}
 
void AdcStatStart(AdcStrcutTypeDef *pAdcStruct)
{
	/*设置 ADC 开始转换*/
	AdcStartCmd();
	/*设置状态为 检查 ADC 是否采集完成*/
	pAdcStruct->stat = ADC_STAT_CHECK_COMPLETE;
	return;
}
 
void AdcStatCheckComplete(AdcStrcutTypeDef *pAdcStruct)
{ 
	/*读取 ADC 状态,判断是否采集完成*/
	if(AdcReadCompleteStat() == 0x00)
	{
	/*没有采集完成直接返回*/
	return;
	} 
	/*设置状态为 检查 ADC 是否采集完成*/
	pAdcStruct->stat = ADC_STAT_CHECK_READ_DATA;
}
 
void AdcStatCheckReadData(AdcStrcutTypeDef *pAdcStruct)
{
	/*读取 ADC 采集的数据到变量*/
	pAdcStruct->AdcData = AdcReadData();
	/*设置状态为 检查是否到达 ADC 采集时间*/
	pAdcStruct->stat = ADC_STAT_CHECK_TIME;
}

```



### 4.4 总结

通过第3节的例子可以发现，当使用状态机编程时，如果在已有的模块上增加一个模块是非常容易的，而且对其他模块并没有干扰。

在资源有限的单片机中，没有操作系统，没有多任务，如果使用传统的方法，很难实现复杂的功能。而使用状态机编程，不像操作系统那样需要大量的单片机RAM，FLASH等资源，可以将复杂的问题，分解为多个状态，从而实现简单的编程，充分利用单片机的资源利用率，也使单片机做出复杂的项目，提供了强大的技术方法。
