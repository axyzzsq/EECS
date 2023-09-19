# GSM无线通信

一个GSM应用场景举例：家庭烟雾检测火灾报警系统，作为通信模块的角色，当烟雾传感器触发主控向GSM模组下发信息发送的指令时，模组通过2G网络向用户手机发出火灾报警短信，相较于Wi-Fi通信，减少了对路由的依赖，独立性和稳定性更高。

本文通过通信模型，基本原理、应用概述和代码解析来阐述如何在嵌入式开发中使用GSM模组完成通信功能。

## 一、通信模型

![image-20220723143029244](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220723143029244.png)

GSM模组Sim800C具有双向短信通信的功能，既可以发送短信，也可以接收存储短信。GSM模组插上sim卡之后和基站在2.4G频段传输PDU串进行通信。

PDU串是模组和基站一种标准通信格式，应用开发者不需要过多关注，有兴趣可以查阅[pdu串短信格式说明](https://blog.csdn.net/qq_34719012/article/details/117081122)。知道GSM会把从主控接收的文本数据编码为PDU串数据再转发给基站、从基站接收的PDU串数据自动转换成文本数据等待主控读取即可。

主控通信通过串口发送**AT指令字符串数据**对模组进行初始化、短信发送等操作。

## 二、基础知识

### 1、AT指令

AT指令是一种交互规则指令(不同的厂商会定义不同的指令)，是指在以“AT”开头的指令，主控芯片通过串口发送的用来控制模组的通信业务的指令。

AT指令集可分为三个类型：

|   语法   |                     类型                      |                  说明                  |
| :------: | :-------------------------------------------: | :------------------------------------: |
| 执行命令 | 有参数: AT+< x > = <…> <br />无参数: AT+< x > |         用来设置AT命令中的属性         |
| 测试命令 |                 AT + < x >=？                 | 用来查询AT命令设置的合法参数数值有哪些 |
| 查询命令 |                  AT+< x >？                   |    用来查询当前AT指令的设置的属性值    |

大部分模组支持3GPP TS27.007 AT命令集，并在此基础上扩展出自定义指令集。以Ｃ开头的命令如AT+CFUN是是通用的命令，以N开头的命令是模组厂商自定义的命令，不同的模组厂商可能使用不同的命令实现相同的功能。

### 2、GSM通信网

GSM通信原理写了又删了，凑字数的嫌疑太过于明显，有兴趣自己去百度，CSDN上有很多，对模组应用开发来说没啥用。知道这玩意儿会通过2.4G网络和基站通信就行。

相关博文：

[GSM 百科解释](https://www.mscbsc.com/cidian/baike2ff)

[无线通信——GSM系统组成](https://blog.csdn.net/m0_52840978/article/details/123342783)

## 三、应用概述

从应用层开发的角度来看GSM模组的使用，主要关注主控芯片要完成一个通信功能(如发送短信)时候，要依次发送什么AT指令，不同的指令如何在参数中加入payload数据。

#### 1、串口测试

在模组焊接好天线之后，通过串口工具对模组进行AT指令测试，可以判定拿到的GSM模组是不是好的。

![image-20220723164543895](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220723164543895.png)



如图所示，用USB转串口的VCC、GND、TX、RX和Sim800C互联，在电脑用串口上位机发送命令就可以测试了，串口测试过程如下图所示：

![image-20220723164904578](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220723164904578.png)

对上述的测试过程进行解析：

```shell
AT+CMGF=1               	#主控发送给模组,命令模组进行初始化
OK                 			#模组回复给主控,表示主控发过来的上一条指令执行结束
AT+CSMP?           			#主控发送给模组,查询模组当前短消息文本模式参数
+CSMP:17,167,0,0			#模组回复给主控,回复支持文本格式,具体内容查看下文【GSM AT指令含义和设置参考博文】

OK							#模组回复给主控,表示上一条查询的响应结束
AT+CSCS?					#主控发送给模组,查询GSM模组当前的短信内容支持的编码格式
+CSCS:"IRA" 				#模组回复给主控,表示支持IRA编码(国际参考字符编码集(ITU-T T.50))

OK							#模组回复给主控,表示上一条查询命令的响应结束
AT+CMGS="18559316525"  		#主控发送给模组,表示即将给18559316525这个手机号码发送短信
> 							#模组回复给主控,此时用户可以输入内容
(> Hello XMUT,This is Fire Warning Message Sending Test!)  #在模组回复的'>'写入发送的内容，写完不能加回车
+CMGS:17					#模组回复给主控,表示传输方向是output

OK							#模组回复给主控,表示已经设置好了发送内容和发送目的地

1A                      	#主控发送给模组,短信发送指令,注意发送的是0X1A,此时要选中16进制发送,不用加回车
```

[GSM AT指令含义和设置参考博文](https://blog.csdn.net/JDDXM/article/details/84751467)

Note：

AT指令集的输入，串口上位机发送给下位的时候，需要加入回车换行；

对”>”返回进行短信内容输入的时候，将要发送的文本内容则不能加回车，否则就会造成模组发送Error。

依次执行上述内容之后，就可以把短信发送出去了，效果如下图：

![image-20220723172702038](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/image-20220723172702038.png)



#### 2、程控通信

在上一小节的串口测试中，已经实现了GSM模组的短信发送，只需要将串口线改接到主控芯片上，集成串口发送指令的函数，就可以实现外部触发程控短信发送。

主函数：

main函数先初始化管脚，在while(1)中触发短信发送

```C
int main(void)
 {
	delay_init();
	BEEP_Init();
	uart_init(9600); //TX-pin9,RX-pin10
    Adc_Init();
  	TIM2_Int_Init(999,7199); //10Khz锟侥硷拷锟斤拷频锟绞ｏ拷锟斤拷锟斤拷锟斤拷5000为500ms  
	
	Lcd_GPIO_init();  //Init lcd pin
	Lcd_Init();		  //init lcd
	Lcd_Puts(0,0,(unsigned char *)init_lcd1);	//display
	Lcd_Puts(0,1,(unsigned char *)init_lcd2);	//display
	//Send_message(content_Smoke,phone_number); // send context by sim800c
	while(1)
	{
		//ADC get smoke value
		if(T_100ms >= 10)
		{
			T_100ms = 0;	
			ALValue = (u16)(Get_Adc(9));
			printf("\r\n ALValue: %d\n",ALValue);
			// LCD display
			Display_1602_somoke(ALValue);
		}
		
		if(((u16)ALValue)>WARNING)
		{
			alarm_time += 1;
			GPIO_SetBits(GPIOB,GPIO_Pin_5);
			delay_ms(300);
			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
			//Lcd_Puts(0,1,(unsigned char *)init_lcd2);	//display
			if(alarm_time == 15)
			{
				Send_message(content_Smoke,phone_number); // 发送短信
			}
			if(alarm_time > 1000)
			{
				alarm_time = 0;
			}
		}
	}
}
```

printf重定向,发送到和GSM模组相连的串口上，

发送短信集成代码：

```C
void Send_message(unsigned char *content,unsigned char *number)
{
	delay_ms(500);
	printf("AT+CMGF=1\r\n");  //
	delay_ms(500);
	printf("AT+CSMP=17,167,2,25\r\n");//设置短信文本模式参数
	delay_ms(500);
	printf("AT+CSCS=\"GSM\"\r\n");     //配置英文格式字节集
	delay_ms(500);
	printf("AT+CMGS=\"");
	printf("%s",number);
	printf("\"\r\n");	//接收短信的电话号码
	delay_ms(500);
	printf("%s",content); // 发送短信内容！
	delay_ms(500);
	GSM_SendData(0x1A);
	delay_ms(500);
	GSM_SendData(0x1A);
	delay_ms(500);
	GSM_SendData(0x1A);
//	printf("\r\n");	
	delay_ms(500);
}
```

程控的效果和串口测试效果一致。



