#ifndef __PID_H
#define __PID_H

#include "stm32h7xx.h"
#include "tim.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    float target_val; // 目标值
    int actual_val;   // 实际值
    float output_val; // 输出值
    float err;        // 定义偏差值
    float err_last;   // 定义上一个偏差值
    float err_prev;   // 定义上上一个偏差值
    float Kp, Ki, Kd; // 定义比例、积分、微分系数
    float integral;   // 定义积分值
} PID;
extern PID pid[4];

extern void PID_init(float p[], float i[], float d[]);

extern int Set_target_val(int motor_index, int target_val);

extern float PID_realize(int index);

extern int Set_target_val(int motor_index, int target_val);

#endif
