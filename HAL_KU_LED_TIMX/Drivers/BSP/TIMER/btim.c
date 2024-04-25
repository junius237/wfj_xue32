#include "./BSP/TIMER/btim.h"
#include "./BSP/LED/led.h"


TIM_HandleTypeDef g_timx_handle;



/*定时器中断初始化函数*/
void btim_TIMx_int_init(uint16_t plc, uint16_t psc)
{
    g_timx_handle.Instance = TIM6;
    g_timx_handle.Init.Prescaler = plc;
    g_timx_handle.Init.Period = psc;
    HAL_TIM_Base_Init(&g_timx_handle);
    HAL_TIM_Base_Start_IT(&g_timx_handle);
}
/*定时器基础msp初始化*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{

    if (htim->Instance == TIM6)
    {
        __HAL_RCC_TIM6_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM6_IRQn, 1, 2);
        HAL_NVIC_EnableIRQ(TIM6_IRQn);
    }
}

/*定时器中断服务函数*/
void TIM6_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_handle);
}

/*定时器溢出回调函数*/

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)

{
    if (htim->Instance == TIM6)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    }
}
