#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/KEY/key.h"


int main(void)
{
    
    HAL_Init();                              /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);      /* 设置时钟, 72Mhz */
    delay_init(72);                          /* 延时初始化 */
    key_init();                              /* key初始化 */
    usart_init(115200);
    printf("666")
    while(1)
    {
        

       if ( g_usart1_rx_flag==1)
       {
        HAL_UART_Transmit(&g_uart1_handle,(uint8_t*)g_rx_buffer,1,100);
        while (__HAL_USART_GET_FLAG(&g_uart1_handle,USART_FLAG_TC)!=1)
        printf("\r\n");
        g_usart1_rx_flag=0;


       }
       else
       {
        delay_ms(10);
       }
       
        
    }
}

   


