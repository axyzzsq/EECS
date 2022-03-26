# LR寄存器与BX LR的用途

从一个普通子函数返回与异常（中断）返回的汇编指令都是BX  LR，但是这两者的意义是一样的吗？

当单片机涉及函数跳转或者任务调度的时候，理解MCU如何处理堆栈与改变各个通用寄存器的状态尤为重要，下面就来比较一下在不同情况下LR寄存器的意义。

 ![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326194706.png)

上图是一个简单的例子，当仿真点击“下一步”的时候，MCU就会执行BX LR指令，并且从aa子函数返回到main函数中，可以看到在普通子函数里执行BX  LR的意义是“跳转到LR寄存器里的地址执行”。

![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326195148.png)

OS_TaskReturn是作为一个错误返回函数，并且赋值到堆栈中R14（LR）相应的位置。

![](https://pic-1304959529.cos.ap-guangzhou.myqcloud.com/DB/20220326195203.png)

任务调度函数会将PSP堆栈指针指向我们定义自定义的堆栈，然后最后一句汇编是BX LR，那么按照普通子函数执行BX LR的意义，这个时候MCU是会运行OS_TaskReturn这个函数吗？这样不就进入错误函数了吗？

其实不然，因为用M系列实现ucos的任务调度是在PendSV异常（中断）里面实现的，而在异常（中断）里的LR寄存器不再是普通函数里面“返回地址”的意义，而是一个定义为一个称作exc-return的值，当执行BX  LR时候，这个值决定了MCU退出异常（中断）之后的运行等级是在特权级还是用户级以及用的堆栈是MSP还是PSP，而MCU即将要执行的是从PSP堆栈中POP出来的p_task函数，而不是OS_TaskReturn函数。
