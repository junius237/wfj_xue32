#ifndef __CONTROL_H
#define __CONTROL_H

#include <tim.h>
#include <stdio.h>
#include <main.h>
#include <pid.h>
#include <gpio.h>

void Motor_Init(void);

void Motor1_SetSpeed(uint8_t Dir, uint16_t Speed);

void Motor2_SetSpeed(uint8_t Dir, uint16_t Speed);

void Motor3_SetSpeed(uint8_t Dir, uint16_t Speed);

void Motor4_SetSpeed(uint8_t Dir, uint16_t Speed);

extern uint16_t Get_Speed(int motor_index);

void Car_right_shift(uint16_t a, uint16_t b, uint16_t c, uint16_t d);

void Car_left_shift(uint16_t a, uint16_t b, uint16_t c, uint16_t d);

void Car_forward(uint16_t a, uint16_t b, uint16_t c, uint16_t d);

void Car_backward(uint16_t a, uint16_t b, uint16_t c, uint16_t d);

void Car_left(uint16_t a, uint16_t b, uint16_t c, uint16_t d);

void Car_right(uint16_t a, uint16_t b, uint16_t c, uint16_t d);

void Car_stop(void);

void Set_Motor_Speed(void);

// extern int get_encoder_count( void);

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

uint32_t Get_Encoder_Value(void);

#endif
