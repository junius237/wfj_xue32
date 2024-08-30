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

/** Configure pins
     PH0-OSC_IN (PH0)   ------> RCC_OSC_IN
     PH1-OSC_OUT (PH1)   ------> RCC_OSC_OUT
     PA13 (JTMS/SWDIO)   ------> DEBUG_JTMS-SWDIO
     PA14 (JTCK/SWCLK)   ------> DEBUG_JTCK-SWCLK
*/

void Key_GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*��������GPIO�ڵ�ʱ��*/
    KEY1_GPIO_CLK_ENABLE();
    KEY2_GPIO_CLK_ENABLE();
      __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
    /*ѡ�񰴼�������*/	
    GPIO_InitStructure.Pin = KEY1_PIN; 

    /*��������Ϊ����ģʽ*/
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT; 

   /*�������Ų�����Ҳ������*/
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    /*ʹ������Ľṹ���ʼ������*/
    HAL_GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
    /*ѡ�񰴼�������*/
    GPIO_InitStructure.Pin = KEY2_PIN; 

    /*ʹ������Ľṹ���ʼ������*/
    HAL_GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);

}

/**
  * @brief   ����Ƿ��а�������
  * @param   ����Ķ˿ںͶ˿�λ
  *		@arg GPIOx: x�����ǣ�A...G�� 
  *		@arg GPIO_PIN ������GPIO_PIN_x��x������1...16��
  * @retval  ������״̬
  *		@arg KEY_ON:��������
  *		@arg KEY_OFF:����û����
  */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/*����Ƿ��а������� */
	if(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		/*�ȴ������ͷ� */
		while(HAL_GPIO_ReadPin(GPIOx,GPIO_Pin) == KEY_ON);   
		return 	KEY_ON;	 
	}
	else
		return KEY_OFF;
}
