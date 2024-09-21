#include <control.h>
#include <tim.h>
#include <stdio.h>
#include <pid.h>
#include <stm32h7xx_hal_def.h>

#define MIN_TARGET -100 // 根据您的应用需求设置
#define MAX_TARGET 100  // 示例最大值

/****************前*********************** */
/***************************************** */
/*********(O)a**********(O)b************** */
/*********|****************|************** */
/****左***|****************|****右******** */
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
 * @brief 获取指定电机的速度
 *
 * 此函数根据电机索引获取对应电机的速度，并重置定时器计数器。
 *
 * @param motor_index 电机索引，取值范围为0到3
 * @return int 返回电机速度，如果索引无效则返回0
 *
 * @note 使用了对应的定时器来获取电机速度，并在获取后将计数器重置为0。
 *       - motor_index为0时，使用htim3定时器    0
 *       - motor_index为1时，使用htim4定时器    1
 *       - motor_index为2时，使用htim5定时器    2
 *       - motor_index为3时，使用htim8定时器    3
 */

uint16_t Get_Speed(int motor_index)
{
    uint16_t Motor_Speed = 0;
    switch (motor_index)
    {
    case 0:
        Motor_Speed = __HAL_TIM_GET_COUNTER(&htim3); // 使用对应的定时器
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
        Motor_Speed = 0; // 防止无效电机索引
        break;
    }
    return Motor_Speed;
}

/**
 * @brief 设置电机的目标值
 *
 * @param motor_index 电机索引，范围为0到3
 * @param target_val 目标值，范围为MIN_TARGET到MAX_TARGET
 * @return int 如果索引或目标值无效，返回HAL_ERROR；否则返回设置的目标值
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

/***************************方向控制***************************** */

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
