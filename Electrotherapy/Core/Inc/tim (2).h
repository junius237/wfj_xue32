/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
/**
    	* @brief  ���ڶ�̬��̬ѡ���Ρ�����Ƶ�ʡ���ֵ��Ƶ��ͳһ�ɵ�����ֵ�ֿ��ɵ���
    	* @param  WAVE  ���ڲ���ѡ��// 0: Sine, 1: Sparse, 2: Intermittent
		  @param  FREQ  ͳһƵ�ʿɵ�  ȡֵ��0-100�����ɳ���100��
		  @param  AMPL*  *��Ϊ1.2.3  ���ڵ��ڲ�ͬͨ����ֵ  ȡֵ��ΧΪ0-3.3V  ͨ��(Freq*1000000-1)*(MODE->AMPL1)/3.3  ӳ�䵽CCR
    	* @retval None
    */	
typedef struct {
    uint8_t WAVE; 
    uint16_t FREQ; 
    float AMPL1; // ͨ��1�ķ�ֵ
    float AMPL2; // ͨ��2�ķ�ֵ
    float AMPL3; // ͨ��3�ķ�ֵ
} PWM_WAVE_MODE;    
/* USER CODE END Includes */

extern TIM_HandleTypeDef htim2;

extern TIM_HandleTypeDef htim3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_TIM2_Init(void);
void MX_TIM3_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */

void PWM_Mode1_SIN(PWM_WAVE_MODE* MODE);
void update_pwm_waveform(uint16_t* index_ptr, const uint16_t* wave_array, float amplitude, uint16_t freq, uint8_t channel,uint16_t Array_Max_value);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

