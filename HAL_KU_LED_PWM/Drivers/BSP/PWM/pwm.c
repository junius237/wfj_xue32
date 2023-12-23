#include "./BSP/PWM/pwm.h"
#include "./BSP/LED/led.h"

TIM_HandleTypeDef g_timx_pwm_chy_handle;
/*通用定时器PWM输出初始化函数*/
void pwm_timx_chy_init(uint16_t psc, uint16_t arr)
{
    TIM_OC_InitTypeDef timx_oc_pwm_chy;

    g_timx_pwm_chy_handle.Instance = TIM3;                       /*定时器3*/
    g_timx_pwm_chy_handle.Init.Prescaler = psc;                  /*定时器分频*/
    g_timx_pwm_chy_handle.Init.Period = arr;                     /*自动重装载值*/
    g_timx_pwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP; /*向上计数模式*/

    HAL_TIM_PWM_Init(&g_timx_pwm_chy_handle); /*初始化PWM*/

    timx_oc_pwm_chy.OCMode = TIM_OCMODE_PWM1;                                           /*PWM模式1*/
    timx_oc_pwm_chy.Pulse = arr / 2;                                                    /*设置比较值，确定占空比*/
    timx_oc_pwm_chy.OCPolarity = TIM_OCPOLARITY_LOW;                                    /*输出极性为低*/
    HAL_TIM_PWM_ConfigChannel(&g_timx_pwm_chy_handle, &timx_oc_pwm_chy, TIM_CHANNEL_2); /*配置PWM通道*/

    HAL_TIM_PWM_Start(&g_timx_pwm_chy_handle, TIM_CHANNEL_2); /*开启对应PWM通道*/
}

/*定时器输出PWM Msp初始化函数*/

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        GPIO_InitTypeDef gpio_init_truct;

        __HAL_RCC_GPIOB_CLK_ENABLE(); /*开启时钟*/
        __HAL_RCC_TIM3_CLK_ENABLE();  /*开启定时器3时钟*/

        gpio_init_truct.Pin = GPIO_PIN_5;
        gpio_init_truct.Mode = GPIO_MODE_AF_PP;       /*推挽式复用功能*/
        gpio_init_truct.Pull = GPIO_PULLUP;           /*上拉*/
        gpio_init_truct.Speed = GPIO_SPEED_FREQ_HIGH; /*高速*/

        HAL_GPIO_Init(GPIOB, &gpio_init_truct);

        __HAL_RCC_AFIO_CLK_ENABLE();     /*重映射时钟*/
        __HAL_AFIO_REMAP_TIM3_PARTIAL(); /*重映射TIM3*/
    }
}
