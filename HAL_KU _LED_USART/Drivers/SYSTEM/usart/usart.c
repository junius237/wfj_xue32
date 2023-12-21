

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"









/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x)
{
    x = x;
}

char *_sys_command_string(char *cmd, int len)
{
    return NULL;
}







uint8_t g_usart1_rx_flag=0;   /*串口接收到数据标志*/
uint8_t g_rx_buffer[1];  /*HAL使用串口接收数据缓冲区 */

UART_HandleTypeDef g_uart1_handle;  /* UART句柄 */


void usart_init(uint32_t baudrate)
{
    /*UART 初始化设置*/
    g_uart1_handle.Instance = USART1;                                       /* USART_UX */
    g_uart1_handle.Init.BaudRate = baudrate;                                  /* 波特率 */
    g_uart1_handle.Init.WordLength = UART_WORDLENGTH_8B;                      /* 字长为8位数据格式 */
    g_uart1_handle.Init.StopBits = UART_STOPBITS_1;                           /* 一个停止位 */
    g_uart1_handle.Init.Parity = UART_PARITY_NONE;                            /* 无奇偶校验位 */
    g_uart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;                      /* 无硬件流控 */
    g_uart1_handle.Init.Mode = UART_MODE_TX_RX;                               /* 收发模式 */
    HAL_UART_Init(&g_uart1_handle);                                           /* HAL_UART_Init()会使能UART1 */

    
    HAL_UART_Receive(&g_uart1_handle,(uint8_t*)g_rx_buffer,1,10);
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef gpio_init_struct;

    if (huart->Instance == USART1)                            /* 如果是串口1，进行串口1 MSP初始化 */
    {
        __HAL_RCC_USART1_CLK_DISABLE();
        __HAL_RCC_GPIOA_CLK_DISABLE();
        
        gpio_init_struct.Pin = GPIO_PIN_9;               /* 串口发送引脚号 */
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* 推挽式复用 */
        gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* IO速度设置为高速 */
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);
                
        gpio_init_struct.Pin = GPIO_PIN_10;               /* 串口RX脚 模式设置 */
        gpio_init_struct.Mode = GPIO_MODE_AF_INPUT;     /*输入*/
        gpio_init_struct.Pull = GPIO_PULLUP;             /* 上拉 */

        HAL_GPIO_Init(GPIOA, &gpio_init_struct);   /* 串口RX脚 必须设置成输入模式 */
        
        HAL_NVIC_SetPriority(UART5_IRQn,3,3);
        HAL_NVIC_EnableIRQ(UART5_IRQn);

    }
    
}
void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(.&g_uart1_handle);
    HAL_UART_Receive(&g_uart1_handle,(uint8_t*)g_rx_buffer,1,10);
}




void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    g_usart1_rx_flag=1;
}
