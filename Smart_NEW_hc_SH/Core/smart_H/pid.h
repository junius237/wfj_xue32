#ifndef __PID_H
#define __PID_H

#include "stm32h7xx.h"
#include "tim.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    float target_val; // Ŀ��ֵ
    int actual_val;   // ʵ��ֵ
    float output_val; // ���ֵ
    float err;        // ����ƫ��ֵ
    float err_last;   // ������һ��ƫ��ֵ
    float err_prev;   // ��������һ��ƫ��ֵ
    float Kp, Ki, Kd; // ������������֡�΢��ϵ��
    float integral;   // �������ֵ
} PID;
extern PID pid[4];

extern void PID_init(float p[], float i[], float d[]);

extern int Set_target_val(int motor_index, int target_val);

extern float PID_realize(int index);

extern int Set_target_val(int motor_index, int target_val);

#endif
