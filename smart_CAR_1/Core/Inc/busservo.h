#ifndef __BUSSERVO_H
#define __BUSSERVO_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdint.h>
    /* USER CODE BEGIN Includes */

    void MX_USART2_UART_Init(void);
    void USART2_SendDataPacket(uint8_t tx[], uint32_t count);
    void USART2_IRQHandler(void);
    void BusServoCtrl(uint8_t id, uint8_t cmd, uint16_t prm1, uint16_t prm2);
    /* USER CODE END Includes */
    void HAL_UART2_MspInit(UART_HandleTypeDef *uartHandle);

    extern UART_HandleTypeDef huart2;

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */
#define SERVO_MOVE_TIME_WRITE 1 // 通过时间控制舵机打到指定角度

#define SERVO_MOVE_TIME_DATA_LEN 7

#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))

#define PAout(n) BIT_ADDR(GPIOA_BASE + 12, n) // 输出
#define PAin(n) BIT_ADDR(GPIOA_BASE + 8, n)   // 输入

#define UART_RX_ENABLE() \
    PAout(0) = 1;        \
    PAout(1) = 0
#define UART_TX_ENABLE() \
    PAout(0) = 0;        \
    PAout(1) = 1

    /* USER CODE BEGIN Prototypes */

    /* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
