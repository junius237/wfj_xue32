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

/* USER CODE END 0 */

TIM_HandleTypeDef htim5;

/* TIM5 init function */
void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM5_Init 1 */
  __HAL_RCC_TIM5_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 72 - 1;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 65535;

  if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigIC.ICFilter = 0;
  sConfigIC.ICPolarity = TIM_ICPOLARITY_RISING;       /*上升沿捕获*/
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;             /*不分频*/
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI; /*映射到TTI*/

  if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */
  HAL_TIM_MspPostInit(&htim5);
  HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);
  __HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE); /*使能更新中断*/
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (timHandle->Instance == TIM5)
  {
    /* USER CODE BEGIN TIM5_MspPostInit 0 */

    /* USER CODE END TIM5_MspPostInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM5 GPIO Configuration
    PA0-WKUP     ------> TIM5_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN TIM5_MspPostInit 1 */

    HAL_NVIC_SetPriority(TIM5_IRQn, 1, 3);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);
    /* USER CODE END TIM5_MspPostInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void TIM5_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim5);
}
/* USER CODE END 1 */

/**
 * @brief 定时器输入捕获中断处理回调函数
 * @param htim:定时器句柄指针
 * @note 该函数在 HAL_TIM_IRQHandler 中会被调用
 * @retval 无
 */
uint8_t g_tim5_sta = 0;  /*输入捕获状态*/
uint16_t g_tim5_val = 0; /*输入捕获值*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (0 == (g_tim5_sta & 0x80)) /*没有捕获成功*/
  {
    if (g_tim5_sta & 0x40) /*捕获到下降沿*/
    {
      g_tim5_sta |= 0x80;
      g_tim5_val = HAL_TIM_ReadCapturedValue(&htim5, TIM_CHANNEL_1); /*捕获当前值*/
      TIM_RESET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1);              /*清除原来设置*/
      TIM_SET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1,
                              TIM_ICPOLARITY_RISING); /*TIM5设置为上升沿捕获*/
    }

    else
    {
      g_tim5_sta = 0;
      g_tim5_val = 0;
      g_tim5_sta |= 0x40; /*捕获到上升沿*/
      __HAL_TIM_DISABLE(&htim5);
      __HAL_TIM_SET_COUNTER(&htim5, 0);
      TIM_RESET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1); /*清除原来设置*/
      TIM_SET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1,
                              TIM_ICPOLARITY_FALLING); /*TIM5设置为下降沿捕获*/

      __HAL_TIM_ENABLE(&htim5);
    }
  }
}

/**
 * @brief 定时器更新中断回调函数
 * @param htim:定时器句柄指针
 * @note 此函数会被定时器中断函数共同调用的
 * @retval 无
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM5)
  {
    if (0 == (g_tim5_sta & 0x80))
    {
      if (g_tim5_sta & 0x40)
      {
        if (0x3F == (g_tim5_sta & 0x3F))
        {
          TIM_RESET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1); /*清除原来设置*/
          g_tim5_sta |= 0x80;
          g_tim5_val = 0xFFFF;
        }
        else
        {
          g_tim5_sta++;
        }
      }
    }
  }
}
