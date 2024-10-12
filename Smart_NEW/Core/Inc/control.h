#ifndef __CONTROL_H
#define __CONTROL_H

#include "stm32h7xx.h"
#include "tim.h"
#include "pid.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "usart1.h"
void All_Init(void);

uint16_t Get_Encoder(uint8_t Motor_indx);

void Motor1_Speed(uint8_t Dir , uint16_t a);

void Motor2_Speed(uint8_t Dir , uint16_t b );

void Motor3_Speed(uint8_t Dir , uint16_t c);

void Motor4_Speed(uint8_t Dir , uint16_t d);

void car_forward(uint16_t a, uint16_t b, uint16_t c, uint16_t d);

void car_backward(uint16_t a, uint16_t b, uint16_t c, uint16_t d);

void car_left(uint16_t a, uint16_t b, uint16_t c, uint16_t d);

void car_right(uint16_t a, uint16_t b, uint16_t c, uint16_t d);

void car_Left_shift(uint16_t a, uint16_t b, uint16_t c, uint16_t d);

void car_Right_shift(uint16_t a, uint16_t b, uint16_t c, uint16_t d);

void car_stop(void);

void Run(void);

#endif
