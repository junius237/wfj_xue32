#ifndef __PID_H
#define __PID_H

#include "stm32f1xx.h"
#include "tim.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    float target_val; // Ŀ��ֵ
    int actual_val; // ʵ��ֵ
    float output_val; // ���ֵ
    float err;        // ����ƫ��ֵ
    float err_last;   // ������һ��ƫ��ֵ
    float Kp, Ki, Kd; // ������������֡�΢��ϵ��
    float integral;   // �������ֵ
} PID;

extern void PID_init(float p[], float i[], float d[]);
extern void set_p_i_d(int index, float p[], float i[], float d[]);
extern float PID_realize(int index);

#endif
