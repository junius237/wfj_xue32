#include "sys.h"
#include "usart.h"
                               

USART_HandleTypeDef g_usart1_handle; /*usart句柄*/

void Usart_Init(void)
{
    GPIO_InitTypeDef gpio_init_structure;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    // USART1_TX PA9
    gpio_init_structure.Pin = GPIO_PIN_9;
    gpio_init_structure.Mode = GPIO_MODE_AF_PP;
    gpio_init_structure.Pull = GPIO_PULLUP;
    gpio_init_structure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio_init_structure);

    // USART1_RX PA10
    gpio_init_structure.Pin = GPIO_PIN_10;
    gpio_init_structure.Mode = GPIO_MODE_INPUT;
    gpio_init_structure.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &gpio_init_structure);

    /*UART 初始化设置*/
    g_usart1_handle.Instance = USART1;                    /* USART_UX */
    g_usart1_handle.Init.BaudRate = 9600;                 /* 波特率 */
    g_usart1_handle.Init.WordLength = UART_WORDLENGTH_8B; /* 字长为8位数据格式 */
    g_usart1_handle.Init.StopBits = UART_STOPBITS_1;      /* 一个停止位 */
    g_usart1_handle.Init.Parity = UART_PARITY_NONE;       /* 无奇偶校验位 */
    // g_usart1_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;       /* 无硬件流控 */
    g_usart1_handle.Init.Mode = UART_MODE_TX_RX; /* 收发模式 */

    HAL_USART_Init(&g_usart1_handle);
    HAL_USART_IRQHandler(&g_usart1_handle);

    HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}
