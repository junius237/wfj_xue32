#include "./BSP/EXTI/exti.h"
#include "./SYSTEM/delay/delay.h"

/*首先定义一个EXTI的函数*/
void exti_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOE_CLK_ENABLE();

    gpio_init_struct.Pin = GPIO_PIN_4;            /*使用引脚*/
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING; /*下降沿触发*/
    gpio_init_struct.Pull = GPIO_PULLUP;          /*设置上拉*/
    HAL_GPIO_Init(GPIOE, &gpio_init_struct);

    HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 2); /*设置优先级*/
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);         /*使能中断*/
}

/*设置中断服务函数*/

void EXTI4_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4); /*处理EXTI中断请求，也就是这里pin4里面的信息*/
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) /*检测到按钮摁下*/
{
    delay_ms(20); /*软件消抖*/

    if (GPIO_Pin == GPIO_PIN_4) /*检查是否摁下*/
    {
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == 0) /*读取该引脚的电平*/
        {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5); /*翻转该引脚的电平*/
        }
    }
}
