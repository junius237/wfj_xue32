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

void Car_stop();

void Car_forward(uint16_t Speed);

void Car_backward(uint16_t Speed);

void Car_left(uint16_t Speed);

void Car_right(uint16_t Speed);

void Car_left_shift(uint16_t Speed);

void Car_right_shift(uint16_t Speed);



#endif
