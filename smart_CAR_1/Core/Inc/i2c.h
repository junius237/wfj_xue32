/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    i2c.h
 * @brief   This file contains all the function prototypes for
 *          the i2c.c file
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
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

    /* USER CODE BEGIN Includes */

    /* USER CODE END Includes */

    extern I2C_HandleTypeDef hi2c1;

    /* USER CODE BEGIN Private defines */

    /* USER CODE END Private defines */
void iic_nack(void);
void iic_ack(void);
void iic_stop(void);
void iic_start(void);
void MX_I2C1_Init(void);
uint8_t iic_wait_ack(void);
uint8_t iic_read_byte(uint8_t ack);
static void iic_delay(void);

/* USER CODE BEGIN Prototypes */
/* IO 操作 */
#define IIC_SCL(x)                                                                                                                                         \
    do                                                                                                                                                     \
    {                                                                                                                                                      \
        x ? HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); \
    } while (0) /* SCL */
#define IIC_SDA(x)                                                                                                                                         \
    do                                                                                                                                                     \
    {                                                                                                                                                      \
        x ? HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET) : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET); \
    } while (0) /* SDA */
/* 读取 SDA */
#define IIC_READ_SDA HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)

    /* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */
