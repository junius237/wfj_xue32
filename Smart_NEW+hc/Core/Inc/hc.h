#ifndef __HC_H
#define __HC_H

#include "stm32h7xx.h"
#include "tim.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define START_BYTE 0x5A
#define DATA_LENGTH 4
/**********************超声波模块数据定义***********************************/
extern uint8_t data_buffer_L[DATA_LENGTH];
extern uint8_t data_buffer_R[DATA_LENGTH];
extern uint8_t data_index_L;
extern uint8_t data_index_R;
extern uint16_t Distance_R;
extern uint16_t Distance_L;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void check_distances(void);

#endif
