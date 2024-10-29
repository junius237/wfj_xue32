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
#ifndef __I2CNEW_H__
#define __I2CNEW_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>  
/******************************************************************************************/

/* ���� ���� */
#define IIC_SCL_GPIO_PORT               GPIOH
#define IIC_SCL_GPIO_PIN                GPIO_PIN_4
#define IIC_SCL_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0)   /* PH��ʱ��ʹ�� */

#define IIC_SDA_GPIO_PORT               GPIOH
#define IIC_SDA_GPIO_PIN                GPIO_PIN_5
#define IIC_SDA_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOH_CLK_ENABLE(); }while(0)   /* PH��ʱ��ʹ�� */

/******************************************************************************************/

/* IO���� */
#define IIC_SCL(x)        do{ x ? \
                              HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)       /* SCL */

#define IIC_SDA(x)        do{ x ? \
                              HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_RESET); \
                          }while(0)       /* SDA */

#define IIC_READ_SDA     HAL_GPIO_ReadPin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN) /* ��ȡSDA */

/******************************************************************************************/

/* IIC���в������� */
void iic_init(void);               /* ��ʼ��IIC��IO�� */
void iic_start(void);              /* ����IIC��ʼ�ź� */
void iic_stop(void);               /* ����IICֹͣ�ź� */
void iic_ack(void);                /* IIC����ACK�ź� */
void iic_nack(void);               /* IIC������ACK�ź� */
uint8_t iic_wait_ack(void);        /* IIC�ȴ�ACK�ź� */
void iic_send_byte(uint8_t data);  /* IIC����һ���ֽ� */
uint8_t iic_read_byte(uint8_t ack);/* IIC��ȡһ���ֽ� */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

