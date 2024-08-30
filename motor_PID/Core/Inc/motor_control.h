#ifndef __MOTOR_CONTROL_H
#define	__MOTOR_CONTROL_H

#include  "stm32f1xx.h"
#include "tim.h"
/* 电机方向控制枚举 */
typedef enum
{
    MOTOR_FWD = 0,
    MOTOR_REV,
} motor_dir;


/* 以下两宏仅适用于定时器时钟源TIMxCLK=72MHz，预分频器为：7200-1 的情况 */
#define SET_BASIC_TIM_PERIOD(T)     __HAL_TIM_SET_AUTORELOAD(&htim3, (T)*100 - 1)    // 设置定时器的周期（1~1000ms）
#define GET_BASIC_TIM_PERIOD()      ((__HAL_TIM_GET_AUTORELOAD(&htim3)+1)/100.0f)     // 获取定时器的周期，单位ms

/* 设置速度（占空比） */
#define SET_FWD_COMPAER(ChannelPulse)     TIM4_SetPWM_pulse(TIM_CHANNEL_1,ChannelPulse)    // 设置比较寄存器的值
#define SET_REV_COMPAER(ChannelPulse)     TIM4_SetPWM_pulse(TIM_CHANNEL_2,ChannelPulse)    // 设置比较寄存器的值
void motor_pid_control(void);

#endif
 
