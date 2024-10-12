#include <control.h>
#include <tim.h>
#include <stdio.h>
#include <pid.h>

/************************efine****************/
#define PWM_MAX 1000
#define PWM_MIN 0

float Motor1_Speed;
PID motor_pid;
void Motor_Init(void)
{
    /***1 */
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    /***2 */
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);

    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);
    HAL_TIM_Base_Start_IT(&htim4);
    
}

void Set_Speed(uint16_t Speed)
{
    Motor1_SetSpeed(1, Speed);
    Motor2_SetSpeed(1, Speed);
}

void car_balance(void)
{

  //  Get_Speed(200);

    Set_Speed(motor_pid.output_val);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if (htim->Instance == TIM4)
    {
//        Get_Speed();
       // printf("Motor1_Speed: %f\r\n", Motor1_Speed);
    }
}


void Motor1_SetSpeed(uint8_t Dir, uint16_t Speed)
{
    if (Speed >= 1000)
        Speed = 1000;

    if (Dir)
    {
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, Speed);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 0);
    }

    else if (!Dir)
    {
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, Speed);
    }
}

void Motor2_SetSpeed(uint8_t Dir, uint16_t Speed)
{
    if (Speed >= 1000)
        Speed = 1000;

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

/****************************��??��????******************************* */
// ????
void Car_stop()
{
    Motor1_SetSpeed(0, 0);
    Motor2_SetSpeed(0, 0);
}
// ?��??
void Car_forward(uint16_t Speed)
{
    Motor1_SetSpeed(1, Speed);
    Motor2_SetSpeed(1, Speed);
}
// ?��??
void Car_backward(uint16_t Speed)
{
    Motor1_SetSpeed(0, Speed);
    Motor2_SetSpeed(0, Speed);
}
// ������?
void Car_left(uint16_t Speed)
{
    Motor1_SetSpeed(0, Speed);
    Motor2_SetSpeed(1, Speed);
}
// ??��?
void Car_right(uint16_t Speed)
{
    Motor1_SetSpeed(1, Speed);
    Motor2_SetSpeed(0, Speed);
}

// ??????
void Car_right_shift(uint16_t Speed)
{
    Motor1_SetSpeed(1, Speed);
    Motor2_SetSpeed(0, Speed);
}
// ����????
void Car_left_shift(uint16_t Speed)
{
    Motor1_SetSpeed(0, Speed);
    Motor2_SetSpeed(1, Speed);
}

/************************************************************ */
