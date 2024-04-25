#ifndef __BUSSERVO_H
#define __BUSSERVO_H

#include <stdint.h>
void InitUart2(void);
void InitBusServoCtrl(void);
void USART2_SendDataPacket(uint8_t tx[], uint32_t count);
void USART2_IRQHandler(void);
void BusServoCtrl(uint8_t id,uint8_t cmd, uint16_t prm1, uint16_t prm2);

#endif


