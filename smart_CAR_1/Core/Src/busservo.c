#include "busservo.h"
#include <stm32f1xx_hal_usart.h>

UART_HandleTypeDef huart2;

void MX_USART2_UART_Init(void)
{

    /* USER CODE BEGIN USART2_Init 0 */

    /* USER CODE END USART2_Init 0 */

    /* USER CODE BEGIN USART2_Init 1 */

    /* USER CODE END USART2_Init 1 */
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN USART2_Init 2 */
    __HAL_USART_ENABLE_IT(&huart2, USART_IT_RXNE); /*开启中断*/
    __HAL_USART_ENABLE(&huart2);                   /*开启串口*/
    /* USER CODE END USART2_Init 2 */
    HAL_NVIC_SetPriority(USART2_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
}
void HAL_UART2_MspInit(UART_HandleTypeDef *uartHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (uartHandle->Instance == USART2)
    {
        /* USER CODE BEGIN USART2_MspInit 0 */
        MX_USART2_UART_Init(); /*串口2初始化*/
        /* USER CODE END USART2_MspInit 0 */
        /* USART2 clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART2 GPIO Configuration
        PA2     ------> USART2_TX
        PA3     ------> USART2_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_2;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN USART2_MspInit 1 */

        /* USER CODE END USART2_MspInit 1 */
    }
}

void USART2_SendDataPacket(uint8_t tx[], uint32_t count)
{
    uint32_t i;
    for (i = 0; i < count; i++)
    {
        while ((USART2->SR & 0X40) == 0)
            ; // 循环发送,直到发送完毕
        USART2->DR = tx[i];
        while ((USART2->SR & 0X40) == 0)
            ; // 循环发送,直到发送完毕
    }
}

void USART2_IRQHandler(void) // 串口2中断服务程序
{
    uint8_t Res;
    // if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
    //  {
    //     HAL_UART_Receive_IT(&huart2, rData, 1);
    //      Res = USART_ReceiveData(USART2); //(USART1->DR);	//读取接收到的数据
    //  }

    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE))
    {
        // 从USART1数据寄存器中读取接收到的字节
        Res = (uint8_t)(huart2.Instance->DR & (uint8_t)0xFF);

        // 将接收到的字节通过USART1发送回去
        HAL_UART_Transmit(&huart2, &Res, 1, HAL_MAX_DELAY);
    }
}

void BusServoCtrl(uint8_t id, uint8_t cmd, uint16_t prm1, uint16_t prm2)
{
    uint32_t i;
    uint8_t tx[20];
    uint8_t datalLen = 4;
    uint32_t checkSum = 0;

    switch (cmd)
    {
    case SERVO_MOVE_TIME_WRITE:
        datalLen = SERVO_MOVE_TIME_DATA_LEN;
        break;
    }
    tx[0] = 0x55;
    tx[1] = 0x55;
    tx[2] = id;
    tx[3] = datalLen;
    tx[4] = cmd;
    tx[5] = prm1;
    tx[6] = prm1 >> 8;
    tx[7] = prm2;
    tx[8] = prm2 >> 8;
    for (i = 2; i <= datalLen + 1; i++)
    {
        checkSum += tx[i];
    }
    tx[datalLen + 2] = ~checkSum;
    UART_TX_ENABLE();
    USART2_SendDataPacket(tx, datalLen + 3);
}
