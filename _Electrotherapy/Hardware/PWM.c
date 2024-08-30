#include "stm32f10x.h"
#include "delay.h"
#include <math.h>
/***********正弦波参数*************/
#define M_PI 3.14159265358979323846
#define PWM_Period 50 // 50us PWM 周期
#define PWM_Freq 20
// #define PWM_Freq   (1000000 / PWM_Period)  // 计算频率
#define SINE_Amplitude 3.3 // 正弦波幅值为 PWM 周期的 50%
/***********end*******************/

/**********断续波******************/
#define SHORT_BURST_FREQUENCY 1.0f // 短续波频率，单位为Hz
#define BURST_AMPLITUDE 3.3f // 短续波幅度，单位为伏特
#define BURST_ON_TIME 500 // 短续波打开时间，单位为ms
#define BURST_OFF_TIME 500 // 短续波关闭时间，单位为ms
/***********end******************/

void PWM_Init(void)
{
    // 使能 TIM2 和 GPIOA 外设时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置 PA0、PA1、PA2 为 TIM2 PWM 输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置 TIM2 为 PWM 模式
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    TIM_TimeBaseInitStructure.TIM_Period = (uint16_t)(72000000 / (PWM_Freq * 2 * 1000)); // 周期为 1000 个计数
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;                                         // 预分频为 72
    TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // 初始比较值为 0
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);

    // 使能 TIM2
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
            TIM_SetCompare1(TIM2, 0); // 关闭PWM
            burst_on = 0;
        } else {
            TIM_SetCompare1(TIM2, Compare2); // 使用 Compare3 作为 PWM 比较值
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
