# MCU Key Detection

## 1、89C51 Platform

```C
#include <reg51.h>

// Define the lookup table to map the key number to the corresponding port and bit
// For example, the first entry (0x7F) corresponds to port P1.7
// and the second entry (0xEF) corresponds to port P1.6, etc.
const unsigned char keymap[] = { 0x7F, 0xEF, 0xDF, 0xBF };

// Function to detect key press by looking up the port and bit corresponding to the key number
unsigned char keypressed(void)
{
    unsigned char i, port_val;

    // Check each key in turn
    for (i = 0; i < 4; i++)
    {
        // Read the port value and mask off all but the bit corresponding to the current key
        port_val = P1 & keymap[i];

        // If the key is pressed (i.e. the corresponding bit is 0), return the key number
        if (port_val == 0)
        {
            return i;
        }
    }

    // No key was pressed, return a value indicating that
    return 0xFF;
}

void main()
{
    while(1)
    {
        // Call the keypressed function to detect any key presses
        unsigned char key = keypressed();

        // If a key was pressed, do something with it
        if (key != 0xFF)
        {
            // Do something with the key number (0 to 3)
            // For example, print it to an LCD or send it over UART
        }
    }
}

```

`P1`是一个8bit的数据，当P1 Port下有一个按键按下的时候，就会把`P1`的一个bit反转。





## 2、STM32 Platform

```C
#include "stm32f10x.h"

uint16_t keymap[] = {GPIO_Pin_0, GPIO_Pin_1, GPIO_Pin_2, GPIO_Pin_3}; // Lookup table for the keymap
GPIO_TypeDef* keyport = GPIOA; // GPIO port used for the keys
uint8_t numkeys = 4; // Number of keys to check

void key_init(void) 
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU; // Set as input with internal pull-up resistor
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(keyport, &GPIO_InitStruct);
}

int key_scan(void) 
{
    uint8_t i;
    uint16_t port_val;

    for (i = 0; i < numkeys; i++) 
    {
        port_val = keyport->IDR & keymap[i]; // Read the value of the port and mask off all bits except for the one corresponding to the current key
        
        if (port_val == 0) { // If the key is pressed, return the index of the key
            return i;
        }
    }
    
    return -1; // No key is pressed
}

int main(void) 
{
    int key_index;
    
    key_init();
    
    while (1) 
    {
        key_index = key_scan();
        
        if (key_index >= 0) 
        { 
            // A key is pressed
            // Do something with the key
        }
    }
}

```

此代码使用`key_init()`函数将四个键初始化为带有内部上拉电阻的输入。

`key_scan()`函数使用查找表 ( `keymap`) 依次检查每个键并在按下时返回键的索引。

如果没有按键被按下，函数返回`-1`。在`main()`函数中，代码会持续扫描按键并在按键被按下时执行某些操作。



在代码行中`port_val = keyport->IDR & keymap[i]`，我们使用按位与运算符 ( `&`) 将 GPIO 端口的输入值与位掩码组合在一起，位掩码中有一个位设置为 1，对应于当前正在检查的键。

`IDR`GPIO 端口的（输入数据寄存器）是`keyport`一个 16 位寄存器，用于保存输入引脚的当前逻辑状态。的每一位`IDR`对应于端口的特定输入引脚。通过与`IDR`位掩码`keymap[i]`进行与运算，我们得到一个结果，在与当前被检查的键的输入引脚对应的位置处有一个位设置为 1（如果键被按下）或 0（如果键未被按下） .

例如，假设我们正在检查`keymap`表中的第二个键（即`keymap[1] = GPIO_Pin_1`）。如果按下第二个键，对应的输入引脚`GPIO_Pin_1`将读为逻辑低电平 (0)，而其他输入引脚将读为逻辑高电平 (1)。如果我们`IDR`用 AND 寄存器`keymap[1]`，结果将在位置 1 处将一个位设置为 1，并将所有其他位设置为 0。

如果按下当前键，结果值`port_val`将等于位掩码`keymap[i]`，如果未按下则为 0。然后我们可以检查是否`port_val`等于 0 以确定键是否被按下。