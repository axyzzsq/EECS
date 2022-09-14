# STM32 EXTI10-15,EXTI5-9共用一个中断向量处理方法

## 1、PR寄存器方法

```C
void EXTI9_5_IRQHandler(void)
{
  u32 Temp = 0x00; 
  PEout(2) = ~PEout(2);
  Temp = EXTI->PR;       //取读是那个引脚的中断
  switch(Temp)
  {
      case Exti_From_Pin8:
          EXTI_ClearITPendingBit(EXTI_Line8);
          break;
          
      case Exti_From_Pin9:
          if(Sys_Status < MAX_STATUS)
          {
              Sys_Status ++;
          }
          EXTI_ClearITPendingBit(EXTI_Line9);
          break;
          
      default:
          break;
  }
}

```

中断处理程序说明，由于外部中断EXTI5——EXTI9公用了一个中断（EXTI10——EXTI15类似）所以要区分不同的中断源需要进行相应的判断。如上EXTI9_5_IRQHandler中，通过取读EXTI->PR寄存器来判断中断的来源。

## 2、EXTI_GetITStatus方法

 ![img](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/202209141051086.png)

利用  ITStatus EXTI_GetITStatus(uint32_t EXTI_Line)  函数判断具体的中断源 再进一步处理。