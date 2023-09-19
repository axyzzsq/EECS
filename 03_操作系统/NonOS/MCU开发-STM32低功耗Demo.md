# STM32低功耗

## 一、休眠

要在 STM32 微控制器上配置低功耗睡眠模式，您可以使用电源管理模块和 HAL 库提供的低功耗睡眠模式 API。

低功耗睡眠模式背后的原理是通过禁用不必要的外设，降低核心电压和频率，使MCU进入低功耗状态，进入低功耗睡眠模式。这有助于节省电力并延长电池供电设备的电池寿命。

以下是如何使用 STM32 HAL 库配置低功耗睡眠模式的示例：

```c
#include "stm32f4xx_hal.h"

/* Configure the MCU to enter low-power sleep mode */
void enter_sleep_mode(void)
{
  /* Disable unnecessary peripherals */
  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_All);
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_All);
  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_All);

  /* Reduce the core voltage and frequency to save power */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_HSI);
  __HAL_RCC_HSI_CONFIG(RCC_HSI_OFF);

  /* Enter low-power sleep mode */
  HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

/* Example usage in the main loop */
int main(void)
{
    while (1) {
        /* Perform some tasks */

        /* Enter low-power sleep mode to save power */
        enter_sleep_mode();
    }
}

```

在此示例中，`enter_sleep_mode`调用该函数将 MCU 置于低功耗睡眠模式。首先，禁用不必要的外设以节省电力。然后通过配置电源管理模块降低核心电压和频率。最后，MCU 使用 HAL 库的`HAL_PWR_EnterSLEEPMode`函数进入低功耗睡眠模式。

请注意，如何配置低功耗睡眠模式的具体细节可能因所使用的特定 STM32 微控制器以及 HAL 库版本而异。有关如何在特定应用中配置低功耗睡眠模式的更多信息，请参阅设备数据表和 HAL 库文档。

`__HAL_RCC_PWR_CLK_ENABLE`是STM32 HAL库提供的宏，用于为单片机的电源管理模块启用时钟。该宏在头文件中定义`stm32f4xx_hal_rcc.h`，通常用于低功耗模式或其他电源管理功能。

电源管理模块负责控制微控制器的电源和电压调节，以及管理低功耗模式和睡眠状态。启用电源管理时钟是使用 HAL 库提供的任何电源管理功能所必需的。

该`__HAL_RCC_PWR_CLK_ENABLE`宏用于启用电源管理模块的时钟。它不接受任何参数，也不返回任何值。该宏通常在低功耗模式函数的开头调用，如我之前提供的示例代码所示。

## 二、唤醒

要从 STM32 微控制器的低功耗睡眠模式唤醒，通常需要配置中断或事件来触发唤醒过程。以下是如何使用 STM32 HAL 库配置外部中断以从低功耗睡眠模式唤醒的示例：

```c
#include "stm32f4xx_hal.h"

/* Configure the external interrupt */
void configure_interrupt(void)
{
  /* Enable the clock for GPIOA */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* Configure PA0 as an input */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Enable the EXTI interrupt */
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/* Example usage in the main loop */
int main(void)
{
    /* Configure the external interrupt */
    configure_interrupt();

    while (1) {
        /* Perform some tasks */

        /* Enter low-power sleep mode to save power */
        HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
    }
}

/* External interrupt handler */
void EXTI0_IRQHandler(void)
{
  /* Clear the interrupt flag */
  HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);

  /* Wake up from low-power sleep mode */
  HAL_PWR_DisableSleepOnExit();
}

```

在此示例中，`configure_interrupt`调用该函数以在 GPIO 引脚 PA0 上配置外部中断。中断设置为在上升沿触发。中断`EXTI0_IRQn`使能，中断优先级设置为最高级别（0）。

在主循环中，MCU 使用该函数进入低功耗睡眠模式`HAL_PWR_EnterSLEEPMode`。当外部中断发生时，`EXTI0_IRQHandler`调用该函数。该函数清除中断标志并使用该函数将 MCU 从低功耗睡眠模式唤醒`HAL_PWR_DisableSleepOnExit`。

该`HAL_PWR_DisableSleepOnExit`函数用于确保 MCU 从低功耗睡眠模式唤醒并恢复正常运行。如果不调用此函数，即使在中断发生后，MCU 仍可能继续处于低功耗睡眠模式。

请注意，如何配置外部中断和唤醒事件的具体细节可能因所使用的特定 STM32 微控制器以及 HAL 库版本而异。有关如何在特定应用程序中配置低功耗睡眠模式和唤醒事件的更多信息，请参阅设备数据表和 HAL 库文档。