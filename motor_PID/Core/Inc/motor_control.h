#ifndef __MOTOR_CONTROL_H
#define	__MOTOR_CONTROL_H

#include  "stm32f1xx.h"
#include "tim.h"
/* ����������ö�� */
typedef enum
{
    MOTOR_FWD = 0,
    MOTOR_REV,
} motor_dir;


/* ��������������ڶ�ʱ��ʱ��ԴTIMxCLK=72MHz��Ԥ��Ƶ��Ϊ��7200-1 ����� */
#define SET_BASIC_TIM_PERIOD(T)     __HAL_TIM_SET_AUTORELOAD(&htim3, (T)*100 - 1)    // ���ö�ʱ�������ڣ�1~1000ms��
#define GET_BASIC_TIM_PERIOD()      ((__HAL_TIM_GET_AUTORELOAD(&htim3)+1)/100.0f)     // ��ȡ��ʱ�������ڣ���λms

/* �����ٶȣ�ռ�ձȣ� */
#define SET_FWD_COMPAER(ChannelPulse)     TIM4_SetPWM_pulse(TIM_CHANNEL_1,ChannelPulse)    // ���ñȽϼĴ�����ֵ
#define SET_REV_COMPAER(ChannelPulse)     TIM4_SetPWM_pulse(TIM_CHANNEL_2,ChannelPulse)    // ���ñȽϼĴ�����ֵ
void motor_pid_control(void);

#endif
 
