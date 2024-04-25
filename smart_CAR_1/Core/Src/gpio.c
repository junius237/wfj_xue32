/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   This file provides code for the configuration
 *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
int8_t M1Speed = 0;
int8_t M2Speed = 0;
uint8_t M1Dir = 0;
uint8_t M2Dir = 0;
/* USER CODE END 1 */

/** Configure pins as
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */
void MX_GPIO_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);

	/*Configure GPIO pins : PA4 PA5 PA6 PA7 */
	GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/* USER CODE BEGIN 2 */

void MotorControl(int8_t m1Speed, int8_t m2Speed)
{
	if (m1Speed >= 0)
	{
		M1Dir = 1;
		M1Speed = m1Speed;
	}
	else
	{
		M1Dir = 0;
		M1Speed = -m1Speed;
	}
	if (m2Speed >= 0)
	{
		M2Dir = 1;
		M2Speed = m2Speed;
	}
	else
	{
		M2Dir = 0;
		M2Speed = -m2Speed;
	}
	M1Speed /= 10;
	M2Speed /= 10;
}

void MotorPwmCtrl(void)
{ // 放到100us的定时中断里面
	static uint8_t time = 0;

	if (time < M1Speed) // 通过调节M1Speed的值调节占空比，从而调节速度
	{
		if (M1Dir)
		{
			OUT1 = 1;
			OUT2 = 0;
		}
		else
		{
			OUT1 = 0;
			OUT2 = 1;
		}
	}
	else
	{
		OUT1 = 0;
		OUT2 = 0;
	}
	if (time < M2Speed)
	{
		if (M2Dir)
		{
			OUT3 = 1;
			OUT4 = 0;
		}
		else
		{
			OUT3 = 0;
			OUT4 = 1;
		}
	}
	else
	{
		OUT3 = 0;
		OUT4 = 0;
	}
	if (++time >= 10)
	{ // 10档调速  PWM周期 = 100us * 10 = 1ms 频率 = 1KHz
		time = 0;
	}
}

/* USER CODE END 2 */
