#include "gpio.h"
#include "delay.h"
#include "tim.h"

/* USER CODE BEGIN 0 */
float HCSR04_Distance1;
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

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
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin : PG9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pin : PG10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

void start_mode1(void)
{
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_SET);

  delay_us(20);

  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_RESET);
}

void HCSR04_GetDistance1(void)
{
  unsigned int time = 0;

  __HAL_TIM_SET_COUNTER(&htim5, 0);
  start_mode1();

  while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_10) == 0)
    ;
  HAL_TIM_Base_Start(&htim5);

  while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_10) == 1)
    ;
  HAL_TIM_Base_Stop(&htim5);

  // µ¥Î»cm
  // v = 340m/s = 34000cm/s = 34000cm/10^6us = 0.034cm/us
  // s = vt/2
  time = __HAL_TIM_GET_COUNTER(&htim5);
  HCSR04_Distance1 = (int)time * 340 / 100 / 2; // cm

  __HAL_TIM_SET_COUNTER(&htim5, 0);
}
