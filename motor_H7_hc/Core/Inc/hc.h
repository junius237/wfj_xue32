
#ifndef __HC_H__
#define __HC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "stdio.h"
#include "gpio.h"

#define TRIG_H  HAL_GPIO_WritePin(Trig_GPIO_Port,Trig_Pin,GPIO_PIN_SET)
#define TRIG_L  HAL_GPIO_WritePin(Trig_GPIO_Port,Trig_Pin,GPIO_PIN_RESET)
 
void delay_us(uint32_t us);
void SR04_GetData(void);

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

