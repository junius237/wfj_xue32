#include "hc.h"
#include "stdlib.h"

/*******
 * ���hc--usart2   PA2--TX  PA3--RX
 * �ұ�hc--usart3   PB10--TX  PB11--RX
 */

uint8_t rxData[4];        // �������ݻ�����
uint16_t Distance = 0;    // �������

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        // У��У����
        if (rxData[3] == (rxData[0] ^ rxData[1] ^ rxData[2]))
        {
            // �������
            Distance = ((uint16_t)rxData[1] << 8) | rxData[2];
            // �˴�����Ӵ���������ݵĴ��룬������ʾ����
            printf("Distance: %d\r\n", Distance);
        }
        else
        {
           printf("error");
        }

        // ���¿�ʼ��������
        HAL_UART_Receive_IT(&huart1, rxData, 4);
    }
}