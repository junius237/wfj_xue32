#include <control.h>
#include <tim.h>
#include <stdio.h>
#include <pid.h>
#include <stm32h7xx_hal_def.h>

#define MIN_TARGET -100 // ��������Ӧ����������
#define MAX_TARGET 100  // ʾ�����ֵ

/****************ǰ*********************** */
/***************************************** */
/*********(O)a**********(O)b************** */
/*********|****************|************** */
/****��***|****************|****��******** */
/*********|****************|************** */
/*********|****************|************** */
/*********(O)c***********(O)d************* */
/***************************************** */
/***************************************** */
/***************************************** */

void Motor_Init(void)
{
    /***1 */
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // PA0
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // PA1
    /***2 */
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); // PA2
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4); // PA3
    /**3 */
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1); // PH6
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2); // PH9
    /**4 */
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1); // PE5
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2); // PE6

    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);

    __HAL_TIM_CLEAR_FLAG(&htim13, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&htim13);
}

void Set_Motor_Speed(void)
{

    Car_backward(PID_realize(0), PID_realize(1), PID_realize(2), PID_realize(3));
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if (htim->Instance == TIM13)
    {
    }
}
/**
 * @brief ��ȡָ��������ٶ�
 *
 * �˺������ݵ��������ȡ��Ӧ������ٶȣ������ö�ʱ����������
 *
 * @param motor_index ���������ȡֵ��ΧΪ0��3
 * @return int ���ص���ٶȣ����������Ч�򷵻�0
 *
 * @note ʹ���˶�Ӧ�Ķ�ʱ������ȡ����ٶȣ����ڻ�ȡ�󽫼���������Ϊ0��
 *       - motor_indexΪ0ʱ��ʹ��htim3��ʱ��    0
 *       - motor_indexΪ1ʱ��ʹ��htim4��ʱ��    1
 *       - motor_indexΪ2ʱ��ʹ��htim5��ʱ��    2
 *       - motor_indexΪ3ʱ��ʹ��htim8��ʱ��    3
 */

uint16_t Get_Speed(int motor_index)
{
    uint16_t Motor_Speed = 0;
    switch (motor_index)
    {
    case 0:
        Motor_Speed = __HAL_TIM_GET_COUNTER(&htim3); // ʹ�ö�Ӧ�Ķ�ʱ��
        __HAL_TIM_SET_COUNTER(&htim3, 0);
        break;
    case 1:
        Motor_Speed = __HAL_TIM_GET_COUNTER(&htim4);
        __HAL_TIM_SET_COUNTER(&htim4, 0);
        break;
    case 2:
        Motor_Speed = __HAL_TIM_GET_COUNTER(&htim5);
        __HAL_TIM_SET_COUNTER(&htim5, 0);
        break;
    case 3:
        Motor_Speed = __HAL_TIM_GET_COUNTER(&htim8);
        __HAL_TIM_SET_COUNTER(&htim8, 0);
        break;
    default:
        Motor_Speed = 0; // ��ֹ��Ч�������
        break;
    }
    return Motor_Speed;
}

/**
 * @brief ���õ����Ŀ��ֵ
 *
 * @param motor_index �����������ΧΪ0��3
 * @param target_val Ŀ��ֵ����ΧΪMIN_TARGET��MAX_TARGET
 * @return int ���������Ŀ��ֵ��Ч������HAL_ERROR�����򷵻����õ�Ŀ��ֵ
 */
int Set_target_val(int motor_index, int target_val)
{
    if (motor_index < 0 || motor_index > 3)
        return HAL_ERROR;
    if (target_val < MIN_TARGET || target_val > MAX_TARGET)
        return HAL_ERROR;
    pid[motor_index].target_val = target_val;
    return target_val;
}

//   PA0     ------> TIM2_CH1
//   PA1     ------> TIM2_CH2
void Motor1_SetSpeed(uint8_t Dir, uint16_t a)
{
    if (a >= 1000)
        a = 1000;

    if (Dir)
    {
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, a);
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 0);
    }

    else if (!Dir)
    {
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 0);
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, a);
    }
}

//  PA2     ------> TIM2_CH3
//   PA3     ------> TIM2_CH4
void Motor2_SetSpeed(uint8_t Dir, uint16_t b)
{
    if (b >= 1000)
        b = 1000;

    if (Dir)
    {
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, b);
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 0);
    }

    else if (!Dir)
    {
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 0);
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, b);
    }
}

//  PH6     ------> TIM12_CH1
//  PH9     ------> TIM12_CH2
void Motor3_SetSpeed(uint8_t Dir, uint16_t c)
{
    if (c >= 1000)
        c = 1000;

    if (Dir)
    {
        __HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_1, c);
        __HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_2, 0);
    }

    else if (!Dir)
    {
        __HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_1, 0);
        __HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_2, c);
    }
}

//   PE5     ------> TIM15_CH1
//  PE6     ------> TIM15_CH2
void Motor4_SetSpeed(uint8_t Dir, uint16_t d)
{
    if (d >= 1000)
        d = 1000;

    if (Dir)
    {
        __HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1, d);
        __HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_2, 0);
    }

    else if (!Dir)
    {
        __HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1, 0);
        __HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_2, d);
    }
}

/***************************�������***************************** */

void Car_stop()
{
    Motor1_SetSpeed(0, 0);
    Motor2_SetSpeed(0, 0);
    Motor3_SetSpeed(0, 0);
    Motor4_SetSpeed(0, 0);
}

void Car_forward(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    Motor1_SetSpeed(1, a);
    Motor2_SetSpeed(1, b);
    Motor3_SetSpeed(1, c);
    Motor4_SetSpeed(1, d);
}

void Car_backward(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    Motor1_SetSpeed(0, a);
    Motor2_SetSpeed(0, b);
    Motor3_SetSpeed(0, c);
    Motor4_SetSpeed(0, d);
}

void Car_left(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    Motor1_SetSpeed(0, a);
    Motor2_SetSpeed(1, b);
    Motor3_SetSpeed(0, c);
    Motor4_SetSpeed(1, d);
}

void Car_right(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    Motor1_SetSpeed(1, a);
    Motor2_SetSpeed(0, b);
    Motor3_SetSpeed(1, c);
    Motor4_SetSpeed(0, d);
}

void Car_right_shift(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    Motor1_SetSpeed(1, a);
    Motor2_SetSpeed(0, b);
    Motor3_SetSpeed(0, c);
    Motor4_SetSpeed(1, d);
}

void Car_left_shift(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    Motor1_SetSpeed(0, a);
    Motor2_SetSpeed(1, b);
    Motor3_SetSpeed(1, c);
    Motor4_SetSpeed(0, d);
}

/************************************************************ */
