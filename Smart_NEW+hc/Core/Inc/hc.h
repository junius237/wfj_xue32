#ifndef __HC_H
#define __HC_H

#include "stm32h7xx.h"
#include "tim.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/**********************超声波模块数据定义***********************************/
extern unsigned char dat1[3], dat2[3];
extern unsigned char num1, num2;
extern int distanceL, distanceR;
extern	uint8_t RxByte;
extern uint16_t DistanceValue;
extern	uint8_t RxByteArr[4];
extern uint8_t RxBuff[10];
extern uint16_t RxCount;
extern uint8_t data[4]; 
extern uint8_t RxByteArr[4];

void USART2_IRQHandler_hc(void);
void USART3_IRQHandler_hc(void);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);


#endif
