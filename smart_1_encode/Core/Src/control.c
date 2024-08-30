#include <control.h>
#include <tim.h>
#include <stdio.h>
#include <pid.h>

/*���̳�������*/
/****************ǰ*********************** */
/***************************************** */
/*********(O)1***********(O)2************* */
/*********|****************|************** */
/****��***|****************|****��******** */
/*********|****************|************** */
/*********|****************|************** */
/*********(O)3***********(O)4************* */
/***************************************** */
/***************************************** */
/***************************************** */

/************Dir=1,��ת��Dir=0����ת****** */

void Motor_Init(void)
{
    /***1 */
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // PA0��
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2); // PA1��
    /***2 */
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); // PA2��
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4); // PA3��
    /**3 */
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1); // PH6��
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2); // PH9��
    /**4 */
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1); // PE5��
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2); // PE6��

    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL);
    HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
    
}
//   PA0     ------> TIM2_CH1
//   PA1     ------> TIM2_CH2
void Motor1_SetSpeed(uint8_t Dir, uint16_t Speed)
{
    if (Speed >= 1000)
        Speed = 1000;

    if (Dir)
    {
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, Speed);
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 0);
    }

    else if (!Dir)
    {
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_1, 0);
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, Speed);
    }
}

//  PA2     ------> TIM2_CH3
//   PA3     ------> TIM2_CH4
void Motor2_SetSpeed(uint8_t Dir, uint16_t Speed)
{
    if (Speed >= 1000)
        Speed = 1000;

    if (Dir)
    {
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, Speed);
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, 0);
    }

    else if (!Dir)
    {
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_3, 0);
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_4, Speed);
    }
}

//  PH6     ------> TIM12_CH1
//  PH9     ------> TIM12_CH2
void Motor3_SetSpeed(uint8_t Dir, uint16_t Speed)
{
    if (Speed >= 1000)
        Speed = 1000;

    if (Dir)
    {
        __HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_1, Speed);
        __HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_2, 0);
    }

    else if (!Dir)
    {
        __HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_1, 0);
        __HAL_TIM_SetCompare(&htim12, TIM_CHANNEL_2, Speed);
    }
}

//   PE5     ------> TIM15_CH1
//  PE6     ------> TIM15_CH2
void Motor4_SetSpeed(uint8_t Dir, uint16_t Speed)
{
    if (Speed >= 1000)
        Speed = 1000;

    if (Dir)
    {
        __HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1, Speed);
        __HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_2, 0);
    }

    else if (!Dir)
    {
        __HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_1, 0);
        __HAL_TIM_SetCompare(&htim15, TIM_CHANNEL_2, Speed);
    }
}

/****************************�������******************************* */
// ֹͣ
void Car_stop()
{
    Motor1_SetSpeed(0, 0);
    Motor2_SetSpeed(0, 0);
    Motor3_SetSpeed(0, 0);
    Motor4_SetSpeed(0, 0);
}
// ǰ��
void Car_forward(uint16_t Speed)
{
    Motor1_SetSpeed(1, Speed);
    Motor2_SetSpeed(1, Speed);
    Motor3_SetSpeed(1, Speed);
    Motor4_SetSpeed(1, Speed);
}
// ����
void Car_backward(uint16_t Speed)
{
    Motor1_SetSpeed(0, Speed);
    Motor2_SetSpeed(0, Speed);
    Motor3_SetSpeed(0, Speed);
    Motor4_SetSpeed(0, Speed);
}
// ��ת
void Car_left(uint16_t Speed)
{
    Motor1_SetSpeed(0, Speed);
    Motor2_SetSpeed(1, Speed);
    Motor3_SetSpeed(0, Speed);
    Motor4_SetSpeed(1, Speed);
}
// ��ת
void Car_right(uint16_t Speed)
{
    Motor1_SetSpeed(1, Speed);
    Motor2_SetSpeed(0, Speed);
    Motor3_SetSpeed(1, Speed);
    Motor4_SetSpeed(0, Speed);
}

// ��ƽ��
void Car_right_shift(uint16_t Speed)
{
    Motor1_SetSpeed(1, Speed);
    Motor2_SetSpeed(0, Speed);
    Motor3_SetSpeed(0, Speed);
    Motor4_SetSpeed(4, Speed);
}
// ��ƽ��
void Car_left_shift(uint16_t Speed)
{
    Motor1_SetSpeed(0, Speed);
    Motor2_SetSpeed(1, Speed);
    Motor3_SetSpeed(1, Speed);
    Motor4_SetSpeed(0, Speed);
}

/************************************************************ */
