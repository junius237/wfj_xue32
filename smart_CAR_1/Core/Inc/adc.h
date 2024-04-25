/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    adc.h
 * @brief   This file contains all the function prototypes for
 *          the adc.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

  /* USER CODE BEGIN Includes */

  /* USER CODE END Includes */

  extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */
#define ADC_BAT 13 // 电池电压的AD检测通道

#define TRUE 1
#define FALSE 0
#define NULL 0
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))

//#define PAout(n) BIT_ADDR(GPIOA_BASE + 12, n) // 输出
#define PAin(n) BIT_ADDR(GPIOA_BASE + 8, n)   // 输入
#define BUZZER 		BIT_ADDR(GPIOA_BASE + 12, 9) // 输出
  /* USER CODE END Private defines */

  void MX_ADC1_Init(void);
  void Buzzer(void);
  void CheckBatteryVoltage(void);
  uint16_t GetADCResult(uint8_t ch);
  /* USER CODE BEGIN Prototypes */

  /* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /* __ADC_H__ */
