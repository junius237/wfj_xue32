#include "./BSP/BusServo/busservo.h"

void InitUart2(void)
{
    NVIC_InitTypeDef nvic_InitStructure;
    GPIO_InitTypeDef gpio_InitStructure;
    USART_InitTypeDef usart_InitStructure;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_UART2_CLK_ENABLE();

    /**********UART RX*********/
    gpio_InitStructure.GPIO_Pin = GPIO_Pin_2;
    gpio_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_InitStructure);

    /*********UART TX**********/
    gpio_InitStructure.GPIO_Pin = GPIO_PIN_3;   
    gpio_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &gpio_InitStructure);

    usart_InitStructure.USART_BaudRate = 115200;
    usart_InitStructure.USART_WordLength = USART_WordLength_8b;
    usart_InitStructure.USART_StopBits = USART_StopBits_1;
    usart_InitStructure.USART_Parity = USART_Parity_No;
    usart_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    usart_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    Usart_Init(USART2, &usart_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, Enable); /*使能中断*/
    USART_Cmd(USART2, Enable);                     /*使能串口*/

    nvic_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    nvic_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    nvic_InitStructure.NVIC_IRQChannelSubPriority = 2;
    nvic_InitStructure.NVIC_IRQChannelCmd = Enable;
    NVIC_Init(&nvic_InitStructure);
}
void InitBusServoCtrl(void)
{
    GPIO_TypeDef gpio_InitStructure;
    InitUart2(); /*串口2初始化*/
    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    gpio_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; /*推挽输出*/
    gpio_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_InitStructure);

    UART_TX_ENABLE();
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
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) // 接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res = USART_ReceiveData(USART2); //(USART1->DR);	//读取接收到的数据
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
    USART2SendDataPacket(tx, datalLen + 3);
}
