#include "./BSP/WWDG/wwdg.h"
#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"



int main(void)
{
    HAL_Init();                              /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);      /* 设置时钟, 72Mhz */
    delay_init(72);                          /* 延时初始化 */
    usart_init(115200);                            /* 波特率初始化 */


    if (__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST)!=RESET)
    {
        printf("0000\n");                     /*窗口看门狗复位*/
        __HAL_RCC_CLEAR_RESET_FLAGS();
    }
    
    else
    {
        printf("waibu000\n")  ;                        /*窗口看门狗复位*/
    }
    delay_ms(500);
    printf("weigo\n");
    wwdg_init(0x7f,0x5f,WWDG_PRESCALER_8);
    
    while(1)
    { 
        delay_ms(57);
       // HAL_WWDG_Refresh(&g_wwdg_handle);
    }
}

