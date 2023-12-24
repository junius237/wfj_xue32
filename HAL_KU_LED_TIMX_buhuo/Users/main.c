#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/TIMER/btim.h"
#include "./BSP/LED/led.h"

extern uint8_t  g_timxchy_cap_sta; /*输入捕获状态*/
extern uint16_t  g_timxchy_cap_val; /*输入捕获值*/

int main(void)
{

    uint32_t temp = 0;
    uint8_t t = 0;

    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    usart_init(115200);
    led_init();
    gtim_timx_int_init(0XFFFF, 72 - 1);

    while (1)
    {

        if (g_timxchy_cap_sta & 0X80) /*成功捕获一次*/
        {
            temp = g_timxchy_cap_sta & 0X3F;
            temp *= 65536;                 /*溢出时间总和*/
            temp += g_timxchy_cap_val;     /*高电平时间*/
            printf("HIGH:%d us\r\n" ,temp); /*打印时间*/
            g_timxchy_cap_sta = 0;         /*下一次捕获*/
        }

        t++;

            if (t > 20)
        {
            t = 0;
            LED0_TOGGLE(); /*灯亮程序正常运行*/
        }
        delay_ms(10);
    }
}
