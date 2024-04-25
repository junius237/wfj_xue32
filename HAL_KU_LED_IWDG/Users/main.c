#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/IWDG/wdg.h"

int main(void)
{

    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    usart_init(9600);
    printf("请及时喂狗，不然饿死啦！！！\r\n");
    iwdg_init(IWDG_PRESCALER_32, 1250); /*预分频32，重装再值1250溢出时间约为1s*/
    while (1)
    {

        delay_ms(1000);
        iwdg_feed();
        printf("已经喂狗\r\n");
    }
}
