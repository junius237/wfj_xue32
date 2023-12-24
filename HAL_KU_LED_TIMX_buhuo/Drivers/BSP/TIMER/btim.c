#include "./BSP/TIMER/btim.h"
#include "./BSP/LED/led.h"

TIM_HandleTypeDef g_timx_handle;

/*定时器中断初始化函数*/
void gtim_timx_int_init(uint16_t psc, uint16_t arr)
{
    g_timx_handle.Instance = TIM5;
    g_timx_handle.Init.Prescaler = psc;
    g_timx_handle.Init.Period = arr;
    g_timx_handle.Init.CounterMode = TIM_COUNTERMODE_UP;

    HAL_TIM_IC_Init(&g_timx_handle);
}

/*定时器基础msp初始化*/
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5)
    {
        GPIO_InitTypeDef g_init_struct;
        __HAL_RCC_GPIOA_CLK_ENABLE(); /*使能GPIO*/
        __HAL_RCC_TIM5_CLK_ENABLE();  /*使能通道5*/

        g_init_struct.Pin = GPIO_PIN_0;
        g_init_struct.Pull = GPIO_PULLDOWN;   /*设置下拉*/
        g_init_struct.Mode = GPIO_MODE_AF_PP; /*复用推挽输出*/
        HAL_GPIO_Init(GPIOA, &g_init_struct);

        HAL_NVIC_SetPriority(TIM5_IRQn, 1, 3); /*设置抢占优先级*/
        HAL_NVIC_EnableIRQ(TIM5_IRQn);         /*使能TIM5中断*/
    }
}
/*定时器中断服务函数*/
void TIM5_IRQHandler()
{
    HAL_TIM_IRQHandler(&g_timx_handle); /*定时器公共处理函数*/
}

/*通道TIM5输入捕获 初始化函数*/
void gtim_timx_cap_chy_init()
{
    TIM_IC_InitTypeDef timx_ic_cap_chy = {0};
    timx_ic_cap_chy.ICFilter = 0;                                              /*滤波*/
    timx_ic_cap_chy.ICPolarity = TIM_ICPOLARITY_RISING;                        /*捕获触发方式*/
    timx_ic_cap_chy.ICPrescaler = TIM_ICPSC_DIV1;                              /*捕获分频*/
    timx_ic_cap_chy.ICSelection = TIM_ICSELECTION_DIRECTTI;                    /*捕获选择，映射关系*/
    HAL_TIM_IC_ConfigChannel(&g_timx_handle, &timx_ic_cap_chy, TIM_CHANNEL_1); /**/

    __HAL_TIM_ENABLE_IT(&g_timx_handle, TIM_IT_UPDATE); /*使能更新中断*/
    HAL_TIM_IC_Start(&g_timx_handle, TIM_CHANNEL_1);    /*开始捕获*/
}

uint8_t g_timxchy_cap_sta = 0; /*输入捕获状态*/
uint16_t g_timxchy_cap_val = 0; /*输入捕获值*/

/*捕获回调函和更新中断函数数*/
void HAL_TIM_IC_CaptuerCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5)
    {
        if ((g_timxchy_cap_sta & 0x80) == 0) /*没有捕获成功*/
        {
            if (g_timxchy_cap_val & 0x40) /*捕捉一个下降沿*/
            {
                g_timxchy_cap_sta |= 0x80;                                                     /*成功捕捉一次高电平脉宽*/
                g_timxchy_cap_val = HAL_TIM_ReadCapturedValue(&g_timx_handle, TIM_CHANNEL_1);   /*获取当前捕获值*/
                TIM_RESET_CAPTUREPOLARITY(&g_timx_handle, TIM_CHANNEL_1);                      /*清除原来设置*/
                TIM_SET_CAPTUREPOLARITY(&g_timx_handle, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING); /*配置TIM5上升捕获*/
            }
            else
            {
                g_timxchy_cap_sta = 0; /*清除数据*/
                g_timxchy_cap_val = 0;
                __HAL_TIM_DISABLE(&g_timx_handle);                                              /*关闭定时器TIM5*/
                __HAL_TIM_SET_COUNTER(&g_timx_handle, 0);                                       /*定时器5计数器清除*/
                TIM_RESET_CAPTUREPOLARITY(&g_timx_handle, TIM_CHANNEL_1);                       /*清除原来设置*/
                TIM_SET_CAPTUREPOLARITY(&g_timx_handle, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING); /*配置TIM5下降沿捕获*/
                __HAL_TIM_ENABLE(&g_timx_handle);
            }
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if (htim->Instance == TIM5)
    {
        if ((g_timxchy_cap_sta & 0X80) == 0) /*没有捕获成功*/
        {
            if (g_timxchy_cap_sta & 0X40) /*捕获到高电平*/
            {
                if ((g_timxchy_cap_sta & 0X30) == 0X3F) /*高电平太长*/
                {
                    TIM_RESET_CAPTUREPOLARITY(&g_timx_handle, TIM_CHANNEL_1);                      /*清除原来设置*/
                    TIM_SET_CAPTUREPOLARITY(&g_timx_handle, TIM_CHANNEL_1, TIM_ICPOLARITY_RISING); /*配置TIM5上升捕获*/
                    g_timxchy_cap_sta |= 0X80;                                                     /*成功捕获一次*/
                    g_timxchy_cap_val = 0XFFFF;
                }
                else /*累计溢出次数*/
                {

                    g_timxchy_cap_sta++;
                }
            }
        }
    }
}
