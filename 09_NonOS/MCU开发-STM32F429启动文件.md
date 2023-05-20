# STM32F429 Startup

启动文件的作用

- 1-初始化堆栈指针SP
- 2-初始化PC指针，指向复位程序
- 3-初始化中断向量表
- 4-配置系统时钟
- 5-调用C库函数_main，最终进入C的世界

## 程序

```assembly
;******************** (C) COPYRIGHT 2015 STMicroelectronics ********************
;* File Name          : startup_stm32f429_439xx.s
;* Author             : MCD Application Team
;* @version           : V1.5.0
;* @date              : 06-March-2015
;* Description        : STM32F429xx/439xx devices vector table for MDK-ARM toolchain. 
;*                      This module performs:
;*                      - Set the initial SP
;*                      - Set the initial PC == Reset_Handler
;*                      - Set the vector table entries with the exceptions ISR address
;*                      - Configure the system clock and the external SRAM/SDRAM mounted  
;*                        on STM324x9I-EVAL boards to be used as data memory  
;*                        (optional, to be enabled by user)
;*                      - Branches to __main in the C library (which eventually
;*                        calls main()).
;*                      After Reset the CortexM4 processor is in Thread mode,
;*                      priority is Privileged, and the Stack is set to Main.
;* <<< Use Configuration Wizard in Context Menu >>>   
;*******************************************************************************
; 
; Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
; You may not use this file except in compliance with the License.
; You may obtain a copy of the License at:
; 
;        http://www.st.com/software_license_agreement_liberty_v2
; 
; Unless required by applicable law or agreed to in writing, software 
; distributed under the License is distributed on an "AS IS" BASIS, 
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.
; 
;*******************************************************************************

; Amount of memory (in bytes) allocated for Stack
; Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp               ; Top of Stack
                DCD     Reset_Handler              ; Reset Handler
                DCD     NMI_Handler                ; NMI Handler
                DCD     HardFault_Handler          ; Hard Fault Handler
                DCD     MemManage_Handler          ; MPU Fault Handler
                DCD     BusFault_Handler           ; Bus Fault Handler
                DCD     UsageFault_Handler         ; Usage Fault Handler
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     0                          ; Reserved
                DCD     SVC_Handler                ; SVCall Handler
                DCD     DebugMon_Handler           ; Debug Monitor Handler
                DCD     0                          ; Reserved
                DCD     PendSV_Handler             ; PendSV Handler
                DCD     SysTick_Handler            ; SysTick Handler

                ; External Interrupts
                DCD     WWDG_IRQHandler                   ; Window WatchDog                                        
                DCD     PVD_IRQHandler                    ; PVD through EXTI Line detection                        
                DCD     TAMP_STAMP_IRQHandler             ; Tamper and TimeStamps through the EXTI line            
                DCD     RTC_WKUP_IRQHandler               ; RTC Wakeup through the EXTI line                       
                DCD     FLASH_IRQHandler                  ; FLASH                                           
                DCD     RCC_IRQHandler                    ; RCC                                             
                DCD     EXTI0_IRQHandler                  ; EXTI Line0                                             
                DCD     EXTI1_IRQHandler                  ; EXTI Line1                                             
                DCD     EXTI2_IRQHandler                  ; EXTI Line2                                             
                DCD     EXTI3_IRQHandler                  ; EXTI Line3                                             
                DCD     EXTI4_IRQHandler                  ; EXTI Line4                                             
                DCD     DMA1_Stream0_IRQHandler           ; DMA1 Stream 0                                   
                DCD     DMA1_Stream1_IRQHandler           ; DMA1 Stream 1                                   
                DCD     DMA1_Stream2_IRQHandler           ; DMA1 Stream 2                                   
                DCD     DMA1_Stream3_IRQHandler           ; DMA1 Stream 3                                   
                DCD     DMA1_Stream4_IRQHandler           ; DMA1 Stream 4                                   
                DCD     DMA1_Stream5_IRQHandler           ; DMA1 Stream 5                                   
                DCD     DMA1_Stream6_IRQHandler           ; DMA1 Stream 6                                   
                DCD     ADC_IRQHandler                    ; ADC1, ADC2 and ADC3s                            
                DCD     CAN1_TX_IRQHandler                ; CAN1 TX                                                
                DCD     CAN1_RX0_IRQHandler               ; CAN1 RX0                                               
                DCD     CAN1_RX1_IRQHandler               ; CAN1 RX1                                               
                DCD     CAN1_SCE_IRQHandler               ; CAN1 SCE                                               
                DCD     EXTI9_5_IRQHandler                ; External Line[9:5]s                                    
                DCD     TIM1_BRK_TIM9_IRQHandler          ; TIM1 Break and TIM9                   
                DCD     TIM1_UP_TIM10_IRQHandler          ; TIM1 Update and TIM10                 
                DCD     TIM1_TRG_COM_TIM11_IRQHandler     ; TIM1 Trigger and Commutation and TIM11
                DCD     TIM1_CC_IRQHandler                ; TIM1 Capture Compare                                   
                DCD     TIM2_IRQHandler                   ; TIM2                                            
                DCD     TIM3_IRQHandler                   ; TIM3                                            
                DCD     TIM4_IRQHandler                   ; TIM4                                            
                DCD     I2C1_EV_IRQHandler                ; I2C1 Event                                             
                DCD     I2C1_ER_IRQHandler                ; I2C1 Error                                             
                DCD     I2C2_EV_IRQHandler                ; I2C2 Event                                             
                DCD     I2C2_ER_IRQHandler                ; I2C2 Error                                               
                DCD     SPI1_IRQHandler                   ; SPI1                                            
                DCD     SPI2_IRQHandler                   ; SPI2                                            
                DCD     USART1_IRQHandler                 ; USART1                                          
                DCD     USART2_IRQHandler                 ; USART2                                          
                DCD     USART3_IRQHandler                 ; USART3                                          
                DCD     EXTI15_10_IRQHandler              ; External Line[15:10]s                                  
                DCD     RTC_Alarm_IRQHandler              ; RTC Alarm (A and B) through EXTI Line                  
                DCD     OTG_FS_WKUP_IRQHandler            ; USB OTG FS Wakeup through EXTI line                        
                DCD     TIM8_BRK_TIM12_IRQHandler         ; TIM8 Break and TIM12                  
                DCD     TIM8_UP_TIM13_IRQHandler          ; TIM8 Update and TIM13                 
                DCD     TIM8_TRG_COM_TIM14_IRQHandler     ; TIM8 Trigger and Commutation and TIM14
                DCD     TIM8_CC_IRQHandler                ; TIM8 Capture Compare                                   
                DCD     DMA1_Stream7_IRQHandler           ; DMA1 Stream7                                           
                DCD     FMC_IRQHandler                    ; FMC                                             
                DCD     SDIO_IRQHandler                   ; SDIO                                            
                DCD     TIM5_IRQHandler                   ; TIM5                                            
                DCD     SPI3_IRQHandler                   ; SPI3                                            
                DCD     UART4_IRQHandler                  ; UART4                                           
                DCD     UART5_IRQHandler                  ; UART5                                           
                DCD     TIM6_DAC_IRQHandler               ; TIM6 and DAC1&2 underrun errors                   
                DCD     TIM7_IRQHandler                   ; TIM7                   
                DCD     DMA2_Stream0_IRQHandler           ; DMA2 Stream 0                                   
                DCD     DMA2_Stream1_IRQHandler           ; DMA2 Stream 1                                   
                DCD     DMA2_Stream2_IRQHandler           ; DMA2 Stream 2                                   
                DCD     DMA2_Stream3_IRQHandler           ; DMA2 Stream 3                                   
                DCD     DMA2_Stream4_IRQHandler           ; DMA2 Stream 4                                   
                DCD     ETH_IRQHandler                    ; Ethernet                                        
                DCD     ETH_WKUP_IRQHandler               ; Ethernet Wakeup through EXTI line                      
                DCD     CAN2_TX_IRQHandler                ; CAN2 TX                                                
                DCD     CAN2_RX0_IRQHandler               ; CAN2 RX0                                               
                DCD     CAN2_RX1_IRQHandler               ; CAN2 RX1                                               
                DCD     CAN2_SCE_IRQHandler               ; CAN2 SCE                                               
                DCD     OTG_FS_IRQHandler                 ; USB OTG FS                                      
                DCD     DMA2_Stream5_IRQHandler           ; DMA2 Stream 5                                   
                DCD     DMA2_Stream6_IRQHandler           ; DMA2 Stream 6                                   
                DCD     DMA2_Stream7_IRQHandler           ; DMA2 Stream 7                                   
                DCD     USART6_IRQHandler                 ; USART6                                           
                DCD     I2C3_EV_IRQHandler                ; I2C3 event                                             
                DCD     I2C3_ER_IRQHandler                ; I2C3 error                                             
                DCD     OTG_HS_EP1_OUT_IRQHandler         ; USB OTG HS End Point 1 Out                      
                DCD     OTG_HS_EP1_IN_IRQHandler          ; USB OTG HS End Point 1 In                       
                DCD     OTG_HS_WKUP_IRQHandler            ; USB OTG HS Wakeup through EXTI                         
                DCD     OTG_HS_IRQHandler                 ; USB OTG HS                                      
                DCD     DCMI_IRQHandler                   ; DCMI                                            
                DCD     CRYP_IRQHandler                   ; CRYP crypto                                     
                DCD     HASH_RNG_IRQHandler               ; Hash and Rng
                DCD     FPU_IRQHandler                    ; FPU
                DCD     UART7_IRQHandler                  ; UART7
                DCD     UART8_IRQHandler                  ; UART8
                DCD     SPI4_IRQHandler                   ; SPI4
                DCD     SPI5_IRQHandler                   ; SPI5
                DCD     SPI6_IRQHandler                   ; SPI6
                DCD     SAI1_IRQHandler                   ; SAI1
                DCD     LTDC_IRQHandler                   ; LTDC
                DCD     LTDC_ER_IRQHandler                ; LTDC error
                DCD     DMA2D_IRQHandler                  ; DMA2D
                                         
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
        IMPORT  SystemInit
        IMPORT  __main

                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler          [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler           [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler         [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler           [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler             [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler            [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  WWDG_IRQHandler                   [WEAK]                                        
                EXPORT  PVD_IRQHandler                    [WEAK]                      
                EXPORT  TAMP_STAMP_IRQHandler             [WEAK]         
                EXPORT  RTC_WKUP_IRQHandler               [WEAK]                     
                EXPORT  FLASH_IRQHandler                  [WEAK]                                         
                EXPORT  RCC_IRQHandler                    [WEAK]                                            
                EXPORT  EXTI0_IRQHandler                  [WEAK]                                            
                EXPORT  EXTI1_IRQHandler                  [WEAK]                                             
                EXPORT  EXTI2_IRQHandler                  [WEAK]                                            
                EXPORT  EXTI3_IRQHandler                  [WEAK]                                           
                EXPORT  EXTI4_IRQHandler                  [WEAK]                                            
                EXPORT  DMA1_Stream0_IRQHandler           [WEAK]                                
                EXPORT  DMA1_Stream1_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream2_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream3_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream4_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream5_IRQHandler           [WEAK]                                   
                EXPORT  DMA1_Stream6_IRQHandler           [WEAK]                                   
                EXPORT  ADC_IRQHandler                    [WEAK]                         
                EXPORT  CAN1_TX_IRQHandler                [WEAK]                                                
                EXPORT  CAN1_RX0_IRQHandler               [WEAK]                                               
                EXPORT  CAN1_RX1_IRQHandler               [WEAK]                                                
                EXPORT  CAN1_SCE_IRQHandler               [WEAK]                                                
                EXPORT  EXTI9_5_IRQHandler                [WEAK]                                    
                EXPORT  TIM1_BRK_TIM9_IRQHandler          [WEAK]                  
                EXPORT  TIM1_UP_TIM10_IRQHandler          [WEAK]                
                EXPORT  TIM1_TRG_COM_TIM11_IRQHandler     [WEAK] 
                EXPORT  TIM1_CC_IRQHandler                [WEAK]                                   
                EXPORT  TIM2_IRQHandler                   [WEAK]                                            
                EXPORT  TIM3_IRQHandler                   [WEAK]                                            
                EXPORT  TIM4_IRQHandler                   [WEAK]                                            
                EXPORT  I2C1_EV_IRQHandler                [WEAK]                                             
                EXPORT  I2C1_ER_IRQHandler                [WEAK]                                             
                EXPORT  I2C2_EV_IRQHandler                [WEAK]                                            
                EXPORT  I2C2_ER_IRQHandler                [WEAK]                                               
                EXPORT  SPI1_IRQHandler                   [WEAK]                                           
                EXPORT  SPI2_IRQHandler                   [WEAK]                                            
                EXPORT  USART1_IRQHandler                 [WEAK]                                          
                EXPORT  USART2_IRQHandler                 [WEAK]                                          
                EXPORT  USART3_IRQHandler                 [WEAK]                                         
                EXPORT  EXTI15_10_IRQHandler              [WEAK]                                  
                EXPORT  RTC_Alarm_IRQHandler              [WEAK]                  
                EXPORT  OTG_FS_WKUP_IRQHandler            [WEAK]                        
                EXPORT  TIM8_BRK_TIM12_IRQHandler         [WEAK]                 
                EXPORT  TIM8_UP_TIM13_IRQHandler          [WEAK]                 
                EXPORT  TIM8_TRG_COM_TIM14_IRQHandler     [WEAK] 
                EXPORT  TIM8_CC_IRQHandler                [WEAK]                                   
                EXPORT  DMA1_Stream7_IRQHandler           [WEAK]                                          
                EXPORT  FMC_IRQHandler                    [WEAK]                                             
                EXPORT  SDIO_IRQHandler                   [WEAK]                                             
                EXPORT  TIM5_IRQHandler                   [WEAK]                                             
                EXPORT  SPI3_IRQHandler                   [WEAK]                                             
                EXPORT  UART4_IRQHandler                  [WEAK]                                            
                EXPORT  UART5_IRQHandler                  [WEAK]                                            
                EXPORT  TIM6_DAC_IRQHandler               [WEAK]                   
                EXPORT  TIM7_IRQHandler                   [WEAK]                    
                EXPORT  DMA2_Stream0_IRQHandler           [WEAK]                                  
                EXPORT  DMA2_Stream1_IRQHandler           [WEAK]                                   
                EXPORT  DMA2_Stream2_IRQHandler           [WEAK]                                    
                EXPORT  DMA2_Stream3_IRQHandler           [WEAK]                                    
                EXPORT  DMA2_Stream4_IRQHandler           [WEAK]                                 
                EXPORT  ETH_IRQHandler                    [WEAK]                                         
                EXPORT  ETH_WKUP_IRQHandler               [WEAK]                     
                EXPORT  CAN2_TX_IRQHandler                [WEAK]                                               
                EXPORT  CAN2_RX0_IRQHandler               [WEAK]                                               
                EXPORT  CAN2_RX1_IRQHandler               [WEAK]                                               
                EXPORT  CAN2_SCE_IRQHandler               [WEAK]                                               
                EXPORT  OTG_FS_IRQHandler                 [WEAK]                                       
                EXPORT  DMA2_Stream5_IRQHandler           [WEAK]                                   
                EXPORT  DMA2_Stream6_IRQHandler           [WEAK]                                   
                EXPORT  DMA2_Stream7_IRQHandler           [WEAK]                                   
                EXPORT  USART6_IRQHandler                 [WEAK]                                           
                EXPORT  I2C3_EV_IRQHandler                [WEAK]                                              
                EXPORT  I2C3_ER_IRQHandler                [WEAK]                                              
                EXPORT  OTG_HS_EP1_OUT_IRQHandler         [WEAK]                      
                EXPORT  OTG_HS_EP1_IN_IRQHandler          [WEAK]                      
                EXPORT  OTG_HS_WKUP_IRQHandler            [WEAK]                        
                EXPORT  OTG_HS_IRQHandler                 [WEAK]                                      
                EXPORT  DCMI_IRQHandler                   [WEAK]                                             
                EXPORT  CRYP_IRQHandler                   [WEAK]                                     
                EXPORT  HASH_RNG_IRQHandler               [WEAK]
                EXPORT  FPU_IRQHandler                    [WEAK]
                EXPORT  UART7_IRQHandler                  [WEAK]
                EXPORT  UART8_IRQHandler                  [WEAK]
                EXPORT  SPI4_IRQHandler                   [WEAK]
                EXPORT  SPI5_IRQHandler                   [WEAK]
                EXPORT  SPI6_IRQHandler                   [WEAK]
                EXPORT  SAI1_IRQHandler                   [WEAK]
                EXPORT  LTDC_IRQHandler                   [WEAK]
                EXPORT  LTDC_ER_IRQHandler                [WEAK]
                EXPORT  DMA2D_IRQHandler                  [WEAK]

WWDG_IRQHandler                                                       
PVD_IRQHandler                                      
TAMP_STAMP_IRQHandler                  
RTC_WKUP_IRQHandler                                
FLASH_IRQHandler                                                       
RCC_IRQHandler                                                            
EXTI0_IRQHandler                                                          
EXTI1_IRQHandler                                                           
EXTI2_IRQHandler                                                          
EXTI3_IRQHandler                                                         
EXTI4_IRQHandler                                                          
DMA1_Stream0_IRQHandler                                       
DMA1_Stream1_IRQHandler                                          
DMA1_Stream2_IRQHandler                                          
DMA1_Stream3_IRQHandler                                          
DMA1_Stream4_IRQHandler                                          
DMA1_Stream5_IRQHandler                                          
DMA1_Stream6_IRQHandler                                          
ADC_IRQHandler                                         
CAN1_TX_IRQHandler                                                            
CAN1_RX0_IRQHandler                                                          
CAN1_RX1_IRQHandler                                                           
CAN1_SCE_IRQHandler                                                           
EXTI9_5_IRQHandler                                                
TIM1_BRK_TIM9_IRQHandler                        
TIM1_UP_TIM10_IRQHandler                      
TIM1_TRG_COM_TIM11_IRQHandler  
TIM1_CC_IRQHandler                                               
TIM2_IRQHandler                                                           
TIM3_IRQHandler                                                           
TIM4_IRQHandler                                                           
I2C1_EV_IRQHandler                                                         
I2C1_ER_IRQHandler                                                         
I2C2_EV_IRQHandler                                                        
I2C2_ER_IRQHandler                                                           
SPI1_IRQHandler                                                          
SPI2_IRQHandler                                                           
USART1_IRQHandler                                                       
USART2_IRQHandler                                                       
USART3_IRQHandler                                                      
EXTI15_10_IRQHandler                                            
RTC_Alarm_IRQHandler                            
OTG_FS_WKUP_IRQHandler                                
TIM8_BRK_TIM12_IRQHandler                      
TIM8_UP_TIM13_IRQHandler                       
TIM8_TRG_COM_TIM14_IRQHandler  
TIM8_CC_IRQHandler                                               
DMA1_Stream7_IRQHandler                                                 
FMC_IRQHandler                                                            
SDIO_IRQHandler                                                            
TIM5_IRQHandler                                                            
SPI3_IRQHandler                                                            
UART4_IRQHandler                                                          
UART5_IRQHandler                                                          
TIM6_DAC_IRQHandler                            
TIM7_IRQHandler                              
DMA2_Stream0_IRQHandler                                         
DMA2_Stream1_IRQHandler                                          
DMA2_Stream2_IRQHandler                                           
DMA2_Stream3_IRQHandler                                           
DMA2_Stream4_IRQHandler                                        
ETH_IRQHandler                                                         
ETH_WKUP_IRQHandler                                
CAN2_TX_IRQHandler                                                           
CAN2_RX0_IRQHandler                                                          
CAN2_RX1_IRQHandler                                                          
CAN2_SCE_IRQHandler                                                          
OTG_FS_IRQHandler                                                    
DMA2_Stream5_IRQHandler                                          
DMA2_Stream6_IRQHandler                                          
DMA2_Stream7_IRQHandler                                          
USART6_IRQHandler                                                        
I2C3_EV_IRQHandler                                                          
I2C3_ER_IRQHandler                                                          
OTG_HS_EP1_OUT_IRQHandler                           
OTG_HS_EP1_IN_IRQHandler                            
OTG_HS_WKUP_IRQHandler                                
OTG_HS_IRQHandler                                                   
DCMI_IRQHandler                                                            
CRYP_IRQHandler                                                    
HASH_RNG_IRQHandler
FPU_IRQHandler  
UART7_IRQHandler                  
UART8_IRQHandler                  
SPI4_IRQHandler                   
SPI5_IRQHandler                   
SPI6_IRQHandler                   
SAI1_IRQHandler                   
LTDC_IRQHandler                   
LTDC_ER_IRQHandler                 
DMA2D_IRQHandler                  
                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                 IF      :DEF:__MICROLIB
                
                 EXPORT  __initial_sp
                 EXPORT  __heap_base
                 EXPORT  __heap_limit
                
                 ELSE
                
                 IMPORT  __use_two_region_memory
                 EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

                 LDR     R0, =  Heap_Mem
                 LDR     R1, =(Stack_Mem + Stack_Size)
                 LDR     R2, = (Heap_Mem +  Heap_Size)
                 LDR     R3, = Stack_Mem
                 BX      LR

                 ALIGN

                 ENDIF

                 END

;************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE*****

```

## Stack初始化

```assembly
Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp
```

这段代码定义了一个名为StackMem的区域，其大小为`StackSize`，即`0x00000400`。这个区域是用来存储堆栈的，因此被命名为`STACK`。

`__initial_sp`是一个符号，表示堆栈指针的初始值。

这个符号在其他地方被定义，因此在这里只是被引用。这段代码的作用是为堆栈分配内存空间，并设置堆栈指针的初始值。

- `EQU `是汇编语言中的一个伪指令，用于定义符号常量。相当于C语言中的`#define`
- `AREA `是汇编语言中的一个伪指令，用于定义一个代码段的属性。
- 两个 AREA 指令，分别定义了` STACK `和 `HEAP` 两个代码段。这些指令指定了代码段的名称、属性和对齐方式。在这个代码块中，`STACK`和` HEAP` 两个代码段都是 `NOINIT`、`READWRITE `和` ALIGN=3 `的，这意味着它们是未初始化的、可读写的，并且按照 8 字节对齐。

## Heap初始化

```assembly
Heap_Size       EQU     0x00000200

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB
```

这段代码是 Cortex-M4 处理器的启动代码，用于设置初始堆栈和向量表。在这里，我们看到了一个名为`HEAP`的区域，其中定义了堆的大小和位置。在这个区域中，我们可以看到 `__heap_base `和 `__heap_limit`，它们分别表示堆的起始地址和结束地址。`Heap_Mem` 是一个空间占位符，用于指定堆的大小。

在这个区域中，可以使用 malloc() 和 free() 函数来动态地分配和释放内存。



## 复位片段

```assembly
; Reset handler
Reset_Handler    PROC
                 EXPORT  Reset_Handler             [WEAK]
        IMPORT  SystemInit
        IMPORT  __main

                 LDR     R0, =SystemInit
                 BLX     R0
                 LDR     R0, =__main
                 BX      R0
                 ENDP
```

- `PROC`和`ENDP`成对出现

- ```assembly
  LDR     R0, =SystemInit
                   BLX     R0
                   LDR     R0, =__main
                   BX      R0
  ```

  这几行代码的作用是在 Cortex-M 处理器的复位处理程序中调用 SystemInit 函数和 __main 函数。

  - `LDR R0, =SystemInit` 这行代码的作用是将 SystemInit 函数的地址加载到寄存器 R0 中。LDR 是汇编语言中的指令，用于将数据从内存中加载到寄存器中。= 表示将地址作为立即数加载到寄存器中。因此，这行代码的作用是将 SystemInit 函数的地址加载到寄存器 R0 中，以便后续调用该函数。

  - `BLX R0` BLX R0 是汇编语言中的指令，用于跳转到寄存器 R0 中存储的地址所指向的子程序，并将返回地址保存在链接寄存器中。在这段代码中，R0 中存储的是 SystemInit 函数的地址，因此 BLX R0 的作用是跳转到 SystemInit 函数，并将返回地址保存在链接寄存器中。这样，当 SystemInit 函数执行完毕后，程序会返回到 Reset_Handler 函数中继续执行后续代码。

  - ```assembly
    LDR R0, =__main
    BX R0
    ```

    这两行代码的作用是调用 `__main` 函数。`LDR R0, =__main` 的作用和第一行代码类似，将` __main` 函数的地址加载到寄存器 R0 中。BX R0 是汇编语言中的指令，用于跳转到寄存器 R0 中存储的地址所指向的子程序。因此，这两行代码的作用是跳转到` __main` 函数，并继续执行后续代码。

    总之，这几行代码的作用是在 Cortex-M 处理器的复位处理程序中调用 `SystemInit `函数和` __main `函数，以确保处理器在启动时处于正确的状态，并准备好执行其他代码。



## 中断向量配置

```assembly
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler          [WEAK]
                B       .
                ENDP
```

- `HardFault_Handler`如果重新实现了，当中断向量触发的时候，就跳转到外部执行`HardFault_Handler`,否则就调用` B       .`执行死循环。
- 在中断编程中，如果中断函数的名称写错了，编译并不会保存，但是自己写错的函数并不会被执行，在此处陷入死循环。