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
#include "adc.h"
#include "gpio.h"
#include "servo.h"

/* USER CODE BEGIN 0 */
uint32_t ServoCount;
uint16_t BatteryVoltage;
/* USER CODE END 0 */

TIM_HandleTypeDef htim2;

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
  htim2.Init.Prescaler = 720 - 1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10 - 1;
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
  HAL_TIM_Base_Start_IT(&htim2); // 启动带中断的 TIM2 定时器

  /* USER CODE END TIM2_Init 2 */
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *tim_baseHandle)
{

  if (tim_baseHandle->Instance == TIM2)
  {
    /* USER CODE BEGIN TIM2_MspInit 0 */

    /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
    /* USER CODE BEGIN TIM2_MspInit 1 */
    HAL_NVIC_SetPriority(TIM2_IRQn, 2, 3);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    /* USER CODE END TIM2_MspInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void TIM2_IRQHandler(void)   //TIM2中断
{//定时器2中断  100us
	static uint32_t time = 0;
	Buzzer();       //蜂鸣器驱动
	MotorPwmCtrl(); //电机驱动
  //检查指定的TIM中断发生与否:TIM 中断源
	if(__HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) != RESET)  
	{
		__HAL_TIM_CLEAR_FLAG(&htim2, TIM_FLAG_UPDATE);    //清除TIMx的中断待处理位:TIM 中断源
		if(++time >= 10)
		{ 
			time = 0;
			ServoCount++;
		}
	}
}

void CheckBatteryVoltage(void)
{
  uint8_t i;
  uint32_t v = 0;
  for (i = 0; i < 8; i++)
  {
    v += GetADCResult(ADC_BAT);
  }
  v >>= 3;

  v = v * 2475 / 1024; // adc / 4096 * 3300 * 3(3代表放大3倍，因为采集电压时电阻分压了)
  BatteryVoltage = v;
}
void TaskTimeHandle(void)
{
  static uint32_t time = 10;
	static uint32_t times = 0;
	if(ServoCount > time)
	{
		//time += 10;
		times++;
		ServoCount= 0 ;
		if(times % 2 == 0)//20ms
		{
			ServoPwmDutyCompare();
		}
		if(times % 50 == 0)//500ms
		{
			CheckBatteryVoltage();  //检测电池
			if(BatteryVoltage<=6400)
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5); // 蜂鸣器标志位开
      else
				HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_RESET) ;        //蜂鸣器标志位关
		}
	}
}
/* USER CODE END 1 */
