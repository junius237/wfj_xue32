#ifndef __IMU_H
#define __IMU_H

#include "stm32h7xx.h"
#include "tim.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ��Ԫ���ṹ������ */
typedef struct{
    float q0;
    float q1;
    float q2;
    float q3;
}quater_info_t;

/* ŷ���ǽṹ������ */
typedef struct{
    float pitch;
    float roll;
    float yaw;
}eulerian_angles_t;

/******************************************************************************************/
extern short g_acc_avg[3];                    /* ���ٶ�ƽ��ֵ */
extern short g_gyro_avg[3];                   /* ������ƽ��ֵ */
extern float yaw_test;                             /* ƫ���� */
extern float pitch;                           /* ������ */
extern float roll;                            /* ����� */
void imu_init(void);
//void extract_angles(void);
eulerian_angles_t imu_get_eulerian_angles(float gx, float gy, float gz, float ax, float ay, float az);
void imu_data_calibration(short *gx, short *gy, short *gz, short *ax, short *ay, short *az);

#endif

