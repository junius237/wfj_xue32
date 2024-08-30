/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    **.h
  * @brief   This file contains all the function prototypes for
  *          the ** file
  ******************************************************************************
  */
/* USER CODE END Header */
#ifndef __WAVECTRL_H
#define __WAVECTRL_H
/* USER CODE BEGIN Includes */
#include "main.h"
/* USER CODE END Includes */


/* USER CODE BEGIN Private defines */
	
/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */
void TIM3_GeneratePWM(TIM_HandleTypeDef* htim, uint8_t wave, uint32_t freq_value, uint32_t counter);

/* USER CODE END Prototypes */


#endif /*  */

