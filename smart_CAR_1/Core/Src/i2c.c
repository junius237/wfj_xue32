/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
#include "i2c.h"
#include "delay.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);  

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */
    delay_ms(2);

    /* USER CODE END I2C1_MspInit 1 */
  }
}

/* USER CODE BEGIN 1 */
static void iic_delay(void)
{
    delay_us(2); /* 2us 的延时, 读写速度在 250Khz 以内 */
}
/**
 * @brief 产生 IIC 起始信号
 * @param 无
 * @retval 无
 */
void iic_start(void)
{
    IIC_SDA(1);
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(0); /* START 信号: 当 SCL 为高时, SDA 从高变成低, 表示起始信号 */
    iic_delay();
    IIC_SCL(0); /* 钳住 I2C 总线，准备发送或接收数据 */
    iic_delay();
}
/**
 * @brief 产生 IIC 停止信号
 * @param 无
 * @retval 无
 */
void iic_stop(void)
{
    IIC_SDA(0); /* STOP 信号: 当 SCL 为高时, SDA 从低变成高, 表示停止信号 */
    iic_delay();
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(1); /* 发送 I2C 总线结束信号 */
    iic_delay();
}
/*
* @brief IIC 读取一个字节
* @param ack: ack=1 时，发送 ack; ack=0 时，发送 nack
* @retval 接收到的数据
*/
uint8_t iic_read_byte(uint8_t ack)
{
    uint8_t i, receive = 0;
    for (i = 0; i < 8; i++) /* 接收 1 个字节数据 */
    {
        receive <<= 1; /* 高位先输出,所以先收到的数据位要左移 */
        IIC_SCL(1);
        iic_delay();
        if (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7))
        {
            receive++;
        }
        IIC_SCL(0);
        iic_delay();
    }
    if (!ack)
    {
        iic_nack(); /* 发送 nACK */
    }
    else
    {
        iic_ack(); /* 发送 ACK */
    }
    return receive;
}

/**
 * @brief 等待应答信号到来
 * @param 无
 * @retval 1，接收应答失败
 * 0，接收应答成功
 */
uint8_t iic_wait_ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;
    IIC_SDA(1); /* 主机释放 SDA 线(此时外部器件可以拉低 SDA 线) */
    iic_delay();
    IIC_SCL(1); /* SCL=1, 此时从机可以返回 ACK */
    iic_delay();
    while (HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)) /* 等待应答 */
    {
        waittime++;
        if (waittime > 250)
        {
            iic_stop();
            rack = 1;
            break;
        }
    }
    IIC_SCL(0); /* SCL=0, 结束 ACK 检查 */
    iic_delay();
    return rack;
}
/**
 * @brief 产生 ACK 应答
 * @param 无
 * @retval 无
 */
void iic_ack(void)
{
    IIC_SDA(0); /* SCL 0 -> 1 时 SDA = 0,表示应答 */
    iic_delay();
    IIC_SCL(1); /* 产生一个时钟 */
    iic_delay();
    IIC_SCL(0);
    iic_delay();
    IIC_SDA(1); /* 主机释放 SDA 线 */
    iic_delay();
}
/**
 * @brief 不产生 ACK 应答
 * @param 无
 * @retval 无
 */
void iic_nack(void)
{
    IIC_SDA(1); /* SCL 0 -> 1 时 SDA = 1,表示不应答 */
    iic_delay();
    IIC_SCL(1); /* 产生一个时钟 */
    iic_delay();
    IIC_SCL(0);
    iic_delay();
}


/* USER CODE END 1 */
