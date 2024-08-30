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
/*(O)1��Ӧtim2��(O)2��Ӧtim3��(O)3��Ӧtim4��(O)4��Ӧtim5��*/

/************Dir=1,��ת��Dir=0����ת****** */

void Motor_Init(void)
{
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_1);
    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);

}
// PA2->TIM2_CH3--A
// PA3->TIM2_CH4--B
void Motor1_SetSpeed(uint8_t Dir, uint16_t Speed)
{
    if (Speed >= 100)
        Speed = 100;

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

// PB0->TIM3_CH3--A
// PB1->TIM3_CH4--B
void Motor2_SetSpeed(uint8_t Dir, uint16_t Speed)
{
    if (Speed >= 100)
        Speed = 100;

    if (Dir)
    {
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, Speed);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, 0);
    }

    else if (!Dir)
    {
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_3, 0);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_4, Speed);
    }
}

// PD14->TIM4_CH3--A
// PD15->TIM4_CH4--B
void Motor3_SetSpeed(uint8_t Dir, uint16_t Speed)
{
    if (Speed >= 100)
        Speed = 100;

    if (Dir)
    {
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, Speed);
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 0);
    }

    else if (!Dir)
    {
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 0);
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, Speed);
    }
}

// PH12->TIM5_CH3--A
// PI0->TIM5_CH4--B
void Motor4_SetSpeed(uint8_t Dir, uint16_t Speed)
{
    if (Speed >= 100)
        Speed = 100;

    if (Dir)
    {
        __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, Speed);
        __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_4, 0);
    }

    else if (!Dir)
    {
        __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, 0);
        __HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_4, Speed);
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

/************************************************************ */




