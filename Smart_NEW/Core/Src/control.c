#include "control.h"
#include "pid.h"
#include "usart1.h"

void All_Init(void)
{
    /*printf*/
    usart_init(115200);

    /*pwm*/
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);  // PA6
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);  // PA7
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);  // PB0
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);  // PB1
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1); // PH6
    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2); // PH9
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_1); // PE5
    HAL_TIM_PWM_Start(&htim15, TIM_CHANNEL_2); // PE6

    /*encoder*/
    HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL); // PE9   PE11
    HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL); // PA5   PB3
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL); // PD12  PD13
    HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL); // PH10  PH11

    HAL_TIM_Base_Start_IT(&htim6);
    __HAL_TIM_CLEAR_FLAG(&htim6, TIM_FLAG_UPDATE);

    /*PID*/
}

void Run(void)
{
    car_forward(PID_realize(0), PID_realize(1), PID_realize(2), PID_realize(3));
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM6)

    {
        printf("M1:%d\r\n", Get_Encoder(1));
        printf("M2:%d\r\n", Get_Encoder(2));
        printf("M3:%d\r\n", Get_Encoder(3));
        printf("M4:%d\r\n", Get_Encoder(4));
    }
}

uint16_t Get_Encoder(uint8_t Motor_indx)
{
    uint16_t encoder = 0;
    switch (Motor_indx)
    {
    case 1:
        encoder = __HAL_TIM_GET_COUNTER(&htim1);
        __HAL_TIM_SET_COUNTER(&htim1, 0);
        break;
    case 2:
        encoder = __HAL_TIM_GET_COUNTER(&htim2);
        __HAL_TIM_SET_COUNTER(&htim2, 0);
        break;
    case 3:
        encoder = __HAL_TIM_GET_COUNTER(&htim4);
        __HAL_TIM_SET_COUNTER(&htim4, 0);
        break;
    case 4:
        encoder = __HAL_TIM_GET_COUNTER(&htim5);
        __HAL_TIM_SET_COUNTER(&htim5, 0);
        break;
    default:
        break;
    }
    return encoder;
}

void Motor1_Speed(uint8_t Dir, uint16_t a)
{
    if (a > 1000)
        a = 1000;
    if (Dir)
    {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, a);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, 0);
    }
    else if (!Dir)
    {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, a);
    }
}

void Motor2_Speed(uint8_t Dir, uint16_t b)
{
    if (b > 1000)
        b = 1000;
    if (Dir)
    {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, b);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, 0);
    }
    else if (!Dir)
    {
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, 0);
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, b);
    }
}

void Motor3_Speed(uint8_t Dir, uint16_t c)
{
    if (c > 1000)
        c = 1000;
    if (Dir)
    {
        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, c);
        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, 0);
    }
    else if (!Dir)
    {
        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim12, TIM_CHANNEL_2, c);
    }
}

void Motor4_Speed(uint8_t Dir, uint16_t d)
{
    if (d > 1000)
        d = 1000;
    if (Dir)
    {
        __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, d);
        __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_2, 0);
    }
    else if (!Dir)
    {
        __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_1, 0);
        __HAL_TIM_SET_COMPARE(&htim15, TIM_CHANNEL_2, d);
    }
}

/*****�˶����� */
void car_forward(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    Motor1_Speed(1, a);
    Motor2_Speed(1, b);
    Motor3_Speed(1, c);
    Motor4_Speed(1, d);
}

void car_backward(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    Motor1_Speed(0, a);
    Motor2_Speed(0, b);
    Motor3_Speed(0, c);
    Motor4_Speed(0, d);
}

void car_stop(void)
{
    Motor1_Speed(1, 0);
    Motor2_Speed(1, 0);
    Motor3_Speed(1, 0);
    Motor4_Speed(1, 0);
}

void car_left(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    Motor1_Speed(1, a);
    Motor2_Speed(0, b);
    Motor3_Speed(1, c);
    Motor4_Speed(0, d);
}

void car_right(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    Motor1_Speed(0, a);
    Motor2_Speed(1, b);
    Motor3_Speed(0, c);
    Motor4_Speed(1, d);
}

void car_Left_shift(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    Motor1_Speed(0, a);
    Motor2_Speed(1, b);
    Motor3_Speed(1, c);
    Motor4_Speed(0, d);
}

void car_Right_shift(uint16_t a, uint16_t b, uint16_t c, uint16_t d)
{
    Motor1_Speed(1, a);
    Motor2_Speed(0, b);
    Motor3_Speed(0, c);
    Motor4_Speed(1, d);
}
