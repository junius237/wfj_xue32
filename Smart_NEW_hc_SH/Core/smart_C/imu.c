/**
 * @file imu.c
 * @author 2023320969@qq.com
 * @brief
 * @version 1.0
 * @date 2024-10-22
 *
 * ��̬����
 *
 */
#include "imu.h"
#include "i2cnew.h"
#include "sh3001.h"
#include "delay.h"
#include "usart.h"
#include <math.h>

#define ALPHA 0.5f         // Weight for complementary filter
#define IMU_DELTA_T 0.005f /* 5ms����һ�� */
#define IMU_M_PI 3.1425f

#define IMU_NEW_WEIGHT 0.35f /* ��ֵȨ�� */
#define IMU_OLD_WEIGHT 0.65f /* ��ֵȨ�� */

static float previous_yaw = 0.0f;
quater_info_t g_q_info = {1, 0, 0}; /* ȫ����Ԫ�� */

float g_param_kp = 50.0f; /* ���ٶȼ�(������)���������ʱ�������50 */
float g_param_ki = 0.20f; /* �������������ʵĻ������� 0.2 */
float roll;               /* ����� */
float pitch;              /* ������ */
float yaw_test;                /* ƫ���� */
short g_acc_avg[3];       /* ���ٶ�ƽ��ֵ */
short g_gyro_avg[3];      /* ������ƽ��ֵ */

/**
 * @brief       ��������
 * @param       x           : ��������ֵ
 * @retval      �������
 */
static float imu_inv_sqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}

/**
 * @brief       ����ת��
 * @note        �Լ��ٶ�������һ�׵�ͨ�˲�(�ο�����)����gyroת�ɻ���ÿ��(2000dps)
 * @param       gx, gy, gz  : 3������������ָ��
 * @param       ax, ay, az  : 3����ٶ�����ָ��
 * @retval      ��
 */
static void imu_data_transform(float *gx, float *gy, float *gz, float *ax, float *ay, float *az)
{
    static double lastax = 0;
    static double lastay = 0;
    static double lastaz = 0;

    *ax = *ax * IMU_NEW_WEIGHT + lastax * IMU_OLD_WEIGHT;
    *ay = *ay * IMU_NEW_WEIGHT + lastay * IMU_OLD_WEIGHT;
    *az = *az * IMU_NEW_WEIGHT + lastaz * IMU_OLD_WEIGHT;

    lastax = *ax;
    lastay = *ay;
    lastaz = *az;

    *gx = *gx * IMU_M_PI / 180 / 16.4f;
    *gy = *gy * IMU_M_PI / 180 / 16.4f;
    *gz = *gz * IMU_M_PI / 180 / 16.4f;
}

/**
 * @brief       ��̬�����ں�, �����㷨
 * @note        ʹ�õ��ǻ����˲��㷨��û��ʹ��Kalman�˲��㷨
 *              ������֤�ú����ĵ���Ƶ��Ϊ: IMU_DELTA_T , ����YAW����Ӧ��ƫ��/ƫС
 * @param       gx, gy, gz  : 3������������
 * @param       ax, ay, az  : 3����ٶ�����
 * @retval      ��
 */
static void imu_ahrsupdate_nomagnetic(float gx, float gy, float gz, float ax, float ay, float az)
{
    static float i_ex, i_ey, i_ez; /* ������ */

    float half_t = 0.25f * IMU_DELTA_T;
    float vx, vy, vz; /* ��ǰ�Ļ�������ϵ�ϵ�������λ���� */
    float ex, ey, ez; /* ��Ԫ������ֵ����ٶȼƲ���ֵ����� */
    float q0 = g_q_info.q0;
    float q1 = g_q_info.q1;
    float q2 = g_q_info.q2;
    float q3 = g_q_info.q3;
    float q0q0 = q0 * q0;
    float q0q1 = q0 * q1;
    float q0q2 = q0 * q2;
    float q0q3 = q0 * q3;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q3q3 = q3 * q3;
    float delta_2 = 0;

    /* �Լ��ٶ����ݽ��й�һ�� �õ���λ���ٶ� */
    float norm = imu_inv_sqrt(ax * ax + ay * ay + az * az);
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;
    vx = 2 * (q1q3 - q0q2);
    vy = 2 * (q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    ex = ay * vz - az * vy;
    ey = az * vx - ax * vz;
    ez = ax * vy - ay * vx;

    /* �ò���������PI������������ƫ��
     * ͨ������ g_param_kp��g_param_ki ����������
     * ���Կ��Ƽ��ٶȼ����������ǻ�����̬���ٶȡ�*/
    i_ex += IMU_DELTA_T * ex; /* integral error scaled by Ki */
    i_ey += IMU_DELTA_T * ey;
    i_ez += IMU_DELTA_T * ez;

    gx = gx + g_param_kp * ex + g_param_ki * i_ex;
    gy = gy + g_param_kp * ey + g_param_ki * i_ey;
    gz = gz + g_param_kp * ez + g_param_ki * i_ez;

    /*����������ɣ���������Ԫ��΢�ַ���*/

    /* ��Ԫ��΢�ַ��̣�����half_tΪ�������ڵ�1/4��gx gy gzΪ�����ǽ��ٶȣ�
       ���¶�����֪��������ʹ����һ��������������Ԫ��΢�ַ��� */
    //    q0 = q0 + (-q1 * gx - q2 * gy - q3 * gz) * half_t;
    //    q1 = q1 + ( q0 * gx + q2 * gz - q3 * gy) * half_t;
    //    q2 = q2 + ( q0 * gy - q1 * gz + q3 * gx) * half_t;
    //    q3 = q3 + ( q0 * gz + q1 * gy - q2 * gx) * half_t;
    delta_2 = (2 * half_t * gx) * (2 * half_t * gx) + (2 * half_t * gy) * (2 * half_t * gy) + (2 * half_t * gz) * (2 * half_t * gz);

    /* ������Ԫ����    ��Ԫ��΢�ַ���  ��Ԫ�������㷨�����ױϿ��� */
    q0 = (1 - delta_2 / 8) * q0 + (-q1 * gx - q2 * gy - q3 * gz) * half_t;
    q1 = (1 - delta_2 / 8) * q1 + (q0 * gx + q2 * gz - q3 * gy) * half_t;
    q2 = (1 - delta_2 / 8) * q2 + (q0 * gy - q1 * gz + q3 * gx) * half_t;
    q3 = (1 - delta_2 / 8) * q3 + (q0 * gz + q1 * gy - q2 * gx) * half_t;

    /* normalise quaternion */
    norm = imu_inv_sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    g_q_info.q0 = q0 * norm;
    g_q_info.q1 = q1 * norm;
    g_q_info.q2 = q2 * norm;
    g_q_info.q3 = q3 * norm;
}

/**
 * @brief       �õ���̬������ŷ����
 * @param       gx, gy, gz  : 3������������
 * @param       ax, ay, az  : 3����ٶ�����
 * @retval      ����ֵ : ŷ����
 */
eulerian_angles_t imu_get_eulerian_angles(float gx, float gy, float gz, float ax, float ay, float az)
{
    eulerian_angles_t eulerangle;

    imu_data_transform(&gx, &gy, &gz, &ax, &ay, &az);  /* ����ת�� */
    imu_ahrsupdate_nomagnetic(gx, gy, gz, ax, ay, az); /* ��̬���� */

    float q0 = g_q_info.q0;
    float q1 = g_q_info.q1;
    float q2 = g_q_info.q2;
    float q3 = g_q_info.q3;

    eulerangle.pitch = -asin(-2 * q1 * q3 + 2 * q0 * q2) * 180 / IMU_M_PI;
    eulerangle.roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 180 / IMU_M_PI;
    eulerangle.yaw = -atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 180 / IMU_M_PI;

    /* ���Բ�������̬�޶ȵ����� */
    if (eulerangle.roll > 90 || eulerangle.roll < -90)
    {
        if (eulerangle.pitch > 0)
        {
            eulerangle.pitch = 180 - eulerangle.pitch;
        }

        if (eulerangle.pitch < 0)
        {
            eulerangle.pitch = -(180 + eulerangle.pitch);
        }
    }

    if (eulerangle.yaw > 180)
    {
        eulerangle.yaw -= 360;
    }
    else if (eulerangle.yaw < -180)
    {
        eulerangle.yaw += 360;
    }

    return eulerangle;
}

// /**
//  * @brief ��ȡ�Ƕ�����
//  *
//  * �ú�����IMU��������ȡ���ٶȼƺ������ǵ�ԭʼ���ݣ�������ת��Ϊ�Ƕȣ���λ���ȣ���
//  * �ǶȰ�����ת�ǣ�Roll���������ǣ�Pitch����ƫ���ǣ�Yaw����
//  *
//  * ��Ҫ�������£�
//  * 1. ��ȡ���ٶȼƺ������ǵ�ԭʼ���ݣ�X, Y, Z����
//  * 2. ��ԭʼ����ת��Ϊ�Ƕȣ���λ���ȣ���
//  * 3. �����Ƕȷ�Χ��0-360�ȡ�
//  * 4. ʹ�û����˲���ƽ��ƫ���ǡ�
//  *
//  * @note �ú����������ȫ�ֱ���previous_yaw�ͳ���ALPHA���ڻ����˲�����
//  */
// void extract_angles(void)
// {
//     short gyrodata[3];
//     short accdata[3];
//     sh3001_get_imu_data(accdata, gyrodata); // Read the accelerometer and gyroscope raw data (X, Y, Z)

//     /* Convert the raw data to angles in degrees */
//     roll = atan2(accdata[1], accdata[2]) * 180 / IMU_M_PI;
//     pitch = atan2(-accdata[0], sqrt(accdata[1] * accdata[1] + accdata[2] * accdata[2])) * 180 / IMU_M_PI;
//     yaw_test = atan2(gyrodata[1], gyrodata[0]) * 180 / IMU_M_PI;

//     /* Adjust angles to be in the range 0-360 degrees */
//     if (roll < 0)
//         roll += 360;
//     if (pitch < 0)
//         pitch += 360;
//     if (yaw_test < 0)
//         yaw_test += 360;
//     /* Apply complementary filter to smooth Yaw angle */
//     yaw_test = ALPHA * previous_yaw + (1 - ALPHA) * yaw_test;
//     previous_yaw = yaw_test;

//     printf("ƫ���� Yaw: %.2f\r\n",  yaw_test);
// }

/**
 * @brief       ����У׼
 * @note        �����ݼ�ȥ��ֵ, ���ٶȼ�ȥ�������ٶ�Ӱ��
 * @param       gx, gy, gz  : 3������������ָ��
 * @param       ax, ay, az  : 3����ٶ�����ָ��
 * @retval      ��
 */
void imu_data_calibration(short *gx, short *gy, short *gz, short *ax, short *ay, short *az)
{
    /* ��ȥ��ֵ(ȥ����Ʈ) */
    *gx -= g_gyro_avg[0];
    *gy -= g_gyro_avg[1];
    *gz -= g_gyro_avg[2];
    *ax -= g_acc_avg[0];
    *ay -= g_acc_avg[1];
    *az -= (g_acc_avg[2] - 2048); /* 2048���������ٶ�����ֵ */
}

/**
 * @brief       ��̬�����ʼ��.
 * @note        �ú��������ڴ�������ʼ��֮���ٵ���
 * @param       ��
 * @retval      ��
 */
void imu_init(void)
{
    uint16_t i = 0;
    int acc_sum[3] = {0}, gyro_sum[3] = {0};

    short acc_data[3];  /* ���ٶȴ�����ԭʼ���� */
    short gyro_data[3]; /* ������ԭʼ���� */

    delay_ms(100);
    for (i = 0; i < 250; i++) /* ѭ����ȡ250�� ȡƽ�� */
    {
        sh3001_get_imu_compdata(acc_data, gyro_data);

        acc_sum[0] += acc_data[0];
        acc_sum[1] += acc_data[1];
        acc_sum[2] += acc_data[2];
        gyro_sum[0] += gyro_data[0];
        gyro_sum[1] += gyro_data[1];
        gyro_sum[2] += gyro_data[2];

        delay_ms(5);
    }

    g_acc_avg[0] = acc_sum[0] / 250;
    g_acc_avg[1] = acc_sum[1] / 250;
    g_acc_avg[2] = acc_sum[2] / 250;
    g_gyro_avg[0] = gyro_sum[0] / 250;
    g_gyro_avg[1] = gyro_sum[1] / 250;
    g_gyro_avg[2] = gyro_sum[2] / 250;
}