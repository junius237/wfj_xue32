/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.h
  * @brief   This file contains all the function prototypes for
  *          the gpio.c file
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
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "main.h"
//引脚定义
/*******************************************************/
#define KEY1_PIN                  GPIO_PIN_3                 
#define KEY1_GPIO_PORT            GPIOH                      
#define KEY1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOH_CLK_ENABLE()

#define KEY2_PIN                  GPIO_PIN_2                 
#define KEY2_GPIO_PORT            GPIOH                     
#define KEY2_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOH_CLK_ENABLE()
/*******************************************************/

 /** 按键按下标置宏
	* 按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
	* 若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
	*/
#define KEY_ON	1
#define KEY_OFF	0

void Key_GPIO_init(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

#ifdef __cplusplus
}
#endif
#endif /*__ GPIO_H__ */

