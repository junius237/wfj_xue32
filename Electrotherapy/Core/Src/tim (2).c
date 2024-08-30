/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
/* Includes ------------------------------------------------------------------*/
#include "tim.h"

/* USER CODE BEGIN 0 */
uint16_t Freq = 0;

uint16_t sine_index;
uint16_t sparse_index;
uint16_t intermittent_index;

const uint16_t sine_wave[100] = {
    0x1000, 0x1045, 0x108B, 0x10D2, 0x1119, 0x1160, 0x11A8, 0x11F0,
    0x1238, 0x1280, 0x12C8, 0x1310, 0x1358, 0x13A0, 0x13E8, 0x1430,
    0x1478, 0x14C0, 0x1508, 0x1550, 0x1598, 0x15E0, 0x1628, 0x1670,
    0x16B8, 0x1700, 0x1748, 0x1790, 0x17D8, 0x1820, 0x1868, 0x18B0,
    0x18F8, 0x1940, 0x1988, 0x19D0, 0x1A18, 0x1A60, 0x1AA8, 0x1AF0,
    0x1B38, 0x1B80, 0x1BC8, 0x1C10, 0x1C58, 0x1CA0, 0x1CE8, 0x1D30,
    0x1D78, 0x1DC0, 0x1E08, 0x1E50, 0x1E98, 0x1EE0, 0x1F28, 0x1F70,
    0x1FB8, 0x2000, 0x2048, 0x2090, 0x20D8, 0x2120, 0x2168, 0x21B0,
    0x21F8, 0x2240, 0x2288, 0x22D0, 0x2318, 0x2360, 0x23A8, 0x23F0
};
const uint16_t sparse_wave[100] = {
    0x1000, 0x1100, 0x1200, 0x1300, 0x1400, 0x1500, 0x1600, 0x1700,
    0x1800, 0x1900, 0x1A00, 0x1B00, 0x1C00, 0x1D00, 0x1E00, 0x1F00,
    0x2000, 0x2100, 0x2200, 0x2300, 0x2400, 0x2500, 0x2600, 0x2700,
    0x2800, 0x2900, 0x2A00, 0x2B00, 0x2C00, 0x2D00, 0x2E00, 0x2F00,
    0x3000, 0x3100, 0x3200, 0x3300, 0x3400, 0x3500, 0x3600, 0x3700,
    0x3800, 0x3900, 0x3A00, 0x3B00, 0x3C00, 0x3D00, 0x3E00, 0x3F00,
    0x4000, 0x4100, 0x4200, 0x4300, 0x4400, 0x4500, 0x4600, 0x4700,
    0x4800, 0x4900, 0x4A00, 0x4B00, 0x4C00, 0x4D00, 0x4E00, 0x4F00,
    0x5000, 0x5100, 0x5200, 0x5300, 0x5400, 0x5500, 0x5600, 0x5700
};
const uint16_t intermittent_wave[100] = {
    0x1000, 0x0000, 0x2000, 0x0000, 0x3000, 0x0000, 0x4000, 0x0000,
    0x5000, 0x0000, 0x6000, 0x0000, 0x7000, 0x0000, 0x8000, 0x0000,
    0x9000, 0x0000, 0xA000, 0x0000, 0xB000, 0x0000, 0xC000, 0x0000,
    0xD000, 0x0000, 0xE000, 0x0000, 0xF000, 0x0000, 0x0000, 0x1000,
    0x0000, 0x2000, 0x0000, 0x3000, 0x0000, 0x4000, 0x0000, 0x5000,
    0x0000, 0x6000, 0x0000, 0x7000, 0x0000, 0x8000, 0x0000, 0x9000,
    0x0000, 0xA000, 0x0000, 0xB000, 0x0000, 0xC000, 0x0000, 0xD000,
    0x0000, 0xE000, 0x0000, 0xF000, 0x1000, 0x0000, 0x2000, 0x0000,
    0x3000, 0x0000, 0x4000, 0x0000, 0x5000, 0x0000, 0x6000, 0x0000
};



/* USER CODE END 0 */

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7200-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 100;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
	HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END TIM2_Init 2 */

}
/* TIM3 init function */
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 72-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000000/Freq-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspPostInit 0 */

  /* USER CODE END TIM3_MspPostInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PA6     ------> TIM3_CH1
    PA7     ------> TIM3_CH2
    PB0     ------> TIM3_CH3
    PB1     ------> TIM3_CH4
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM3_MspPostInit 1 */

  /* USER CODE END TIM3_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /* TIM2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/**
	* @brief  更新不同通道的波形
	* @param  None
	* @retval None
*/
void update_pwm_waveform(uint16_t* index_ptr, const uint16_t* wave_array, float amplitude, uint16_t freq, uint8_t channel,uint16_t Array_Max_value) {
    *index_ptr = (*index_ptr + freq) % 100;
    switch (channel) {
    case 0:
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, wave_array[*index_ptr] * amplitude / Array_Max_value);  //wave_array[*index_ptr] / Array_Max_value 将数组的值映射到0-1
        break;
    case 1:
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_2, wave_array[*index_ptr] * amplitude / Array_Max_value);
        break;
    case 2:
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, wave_array[*index_ptr] * amplitude / Array_Max_value);
        break;
    default:
        return; 
    }
}

/**
    	* @brief  用于动态或静态选择波形、调节频率、幅值（频率统一可调，幅值分开可调）
    	* @param  WAVE  用于波形选择// 0: Sine, 1: Sparse, 2: Intermittent
		  @param  FREQ  统一频率可调  取值在0-100（不可超过100）
		  @param  AMPL*  *可为1.2.3  用于调节不同通道幅值  取值范围为0-3.3V  通过(Freq*1000000-1)*(MODE->AMPL1)/3.3  映射到CCR
    	* @retval None
    */
void PWM_Mode1_SIN(PWM_WAVE_MODE* MODE) {
    const uint16_t* wave_array;
	uint16_t Array_Max_value;
    switch (MODE->WAVE) {
    case 0: // 正弦波
        wave_array = sine_wave;
		Array_Max_value = 0x5700;
        break;
    case 1: // 疏密波
        wave_array = sparse_wave;
		Array_Max_value = 0x5700;
        break;
    case 2: // 断续波
        wave_array = intermittent_wave;
		Array_Max_value = 0xF000;
        break;
    default:
        return; // 如果波形类型不正确,则返回
    }
	Freq = MODE->FREQ;
	htim3.Init.Period = Freq*1000000-1;
 	float AMP1 = (Freq*1000000-1)*(MODE->AMPL1)/3.3;
	float AMP2 = (Freq*1000000-1)*(MODE->AMPL2)/3.3;
 	float AMP3 = (Freq*1000000-1)*(MODE->AMPL3)/3.3;

    // 更新三个通道的波形 
    update_pwm_waveform(&sine_index, wave_array, AMP1, MODE->FREQ, 0,Array_Max_value);
    update_pwm_waveform(&sparse_index, wave_array, AMP2, MODE->FREQ, 1,Array_Max_value);
    update_pwm_waveform(&intermittent_index, wave_array,AMP3, MODE->FREQ, 2,Array_Max_value);
}



/* USER CODE END 1 */
