#include "hc.h"
#include "stdlib.h"

/*******
 * 左边hc--usart2   PA2--TX  PA3--RX
 * 右边hc--usart3   PB10--TX  PB11--RX
 */

uint8_t rxData[4];        // 接收数据缓冲区
uint16_t Distance = 0;    // 距离变量

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        // 校验校验码
        if (rxData[3] == (rxData[0] ^ rxData[1] ^ rxData[2]))
        {
            // 计算距离
            Distance = ((uint16_t)rxData[1] << 8) | rxData[2];
            // 此处可添加处理距离数据的代码，例如显示或传输
            printf("Distance: %d\r\n", Distance);
        }
        else
        {
           printf("error");
        }

        // 重新开始接收数据
        HAL_UART_Receive_IT(&huart1, rxData, 4);
    }
}