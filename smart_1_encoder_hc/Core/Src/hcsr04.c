#include "hcsr04.h"


double hc_sr04_get_distance(void)

{
    int count;
    double Distance;
    /* 定时器13，通道1，模式随便，主要用于计数，没有使用边沿捕获 */
    HC_Start(); // 开启超声波模块

    HAL_TIM_Base_Start(&htim13); // 开启定时器

    // 对超声波输入端口操作
    while (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_9) == GPIO_PIN_RESET)
        ; // 等待输入电平拉高

    __HAL_TIM_SetCounter(&htim14, 0);

    // 对超声波输入端口操作
    while (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_9) == GPIO_PIN_SET)
        ; // 等待输入电平变低

    count = __HAL_TIM_GetCounter(&htim14);

    HAL_TIM_Base_Stop(&htim14);

    Distance = count * 340 / 2 * 0.000001 * 100;

    return Distance;
}

void HC_Start(void)
{

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET); // 拉高

    TIM13_Delay_us(15);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET); // 拉低
}
