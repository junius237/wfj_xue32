#include "stm32f10x.h"
#include "delay.h"
#include <math.h>
/***********���Ҳ�����*************/
#define M_PI 3.14159265358979323846
#define PWM_Period 50 // 50us PWM ����
#define PWM_Freq 20
// #define PWM_Freq   (1000000 / PWM_Period)  // ����Ƶ��
#define SINE_Amplitude 3.3 // ���Ҳ���ֵΪ PWM ���ڵ� 50%
/***********end*******************/

/**********������******************/
#define SHORT_BURST_FREQUENCY 1.0f // ������Ƶ�ʣ���λΪHz
#define BURST_AMPLITUDE 3.3f // ���������ȣ���λΪ����
#define BURST_ON_TIME 500 // ��������ʱ�䣬��λΪms
#define BURST_OFF_TIME 500 // �������ر�ʱ�䣬��λΪms
/***********end******************/

void PWM_Init(void)
{
    // ʹ�� TIM2 �� GPIOA ����ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // ���� PA0��PA1��PA2 Ϊ TIM2 PWM ���
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ���� TIM2 Ϊ PWM ģʽ
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    TIM_TimeBaseInitStructure.TIM_Period = (uint16_t)(72000000 / (PWM_Freq * 2 * 1000)); // ����Ϊ 1000 ������
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;                                         // Ԥ��ƵΪ 72
    TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // ��ʼ�Ƚ�ֵΪ 0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    // ʹ�� TIM2
    TIM_Cmd(TIM2, ENABLE);
}
float time = 0;
void PWM_Mode1_SIN(void)
{
    TIM_SetCompare1(TIM2, (uint16_t)(TIM2->ARR * (1.0f + sin(2.0f * 3.1416f * PWM_Freq * (float)TIM2->CNT / TIM2->ARR)) / 2.0f * SINE_Amplitude / 3.3f));
}

/*
void PWM_SetCompare2(uint16_t Compare2)
{
    TIM_SetCompare2(TIM2, Compare2);
}
*/

void PWM_SetCompare2(uint16_t Compare2)
{
    static uint8_t burst_on = 1;
    static uint32_t last_time = 0;
    uint32_t current_time = TIM_GetCounter(TIM2);
    
    if (burst_on) {
        if (current_time - last_time >= BURST_ON_TIME) {
            last_time = current_time;
            TIM_SetCompare1(TIM2, 0); // �ر�PWM
            burst_on = 0;
        } else {
            TIM_SetCompare1(TIM2, Compare2); // ʹ�� Compare3 ��Ϊ PWM �Ƚ�ֵ
        }
    } else {
        if (current_time - last_time >= BURST_OFF_TIME) {
            last_time = current_time;
            burst_on = 1;
        }
    }
}

void PWM_SetCompare3(uint16_t Compare3)
{
    TIM_SetCompare3(TIM2, Compare3);
}
