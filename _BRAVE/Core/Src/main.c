#include "main.h"
#include "delay.h"
#include "stdio.h"
#include "stm32f1xx_hal.h"

void SystemClock_Config(void);
void MX_GPIO_Init(void);

void EXTI_init();
void EXTI0_IRQHandler();
void EXTI3_IRQHandler();
void EXTI4_IRQHandler();

int main(void)
{

  HAL_Init();
  delay_init(72); /*时钟初始化*/
  MX_GPIO_Init();
  EXTI_init();
  EXTI4_IRQHandler();
  EXTI3_IRQHandler();
  EXTI0_IRQHandler();
  while (1)
  {

  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();

  /*PB5 and PE5  初始化*/
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);

  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/*******************配置EXTI********************/
void EXTI_init(void)
{
  GPIO_InitTypeDef GPIO_IninStruct = {0};
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*KEY1设置*/
  GPIO_IninStruct.Pin = GPIO_PIN_0;
  GPIO_IninStruct.Mode = GPIO_MODE_IT_RISING; /*上升沿触发*/
  GPIO_IninStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_IninStruct);

  /*KEY0设置*/
  GPIO_IninStruct.Pin = GPIO_PIN_4;
  GPIO_IninStruct.Mode = GPIO_MODE_IT_FALLING; /*下降沿触发*/
  GPIO_IninStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_IninStruct);
  /*KEY_UP设置*/
  GPIO_IninStruct.Pin = GPIO_PIN_3;
  GPIO_IninStruct.Mode = GPIO_MODE_IT_FALLING; /*下降沿触发*/
  GPIO_IninStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOE, &GPIO_IninStruct);

  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 2);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 2);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 2);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}
/*****************end**************************/

/*****************清除标志位*******************/
/*清除KEY_UP的标志位*/
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
}

/*清除KEY标志位*/
void EXTI3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
}

/*清除KEY0标志位*/
void EXTI4_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
}
/*********************end**********************/

/*******************回调函数*******************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

  switch (GPIO_Pin)
  {
  case GPIO_PIN_4:
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == 0)
    {
      
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    }
    break;
  case GPIO_PIN_3:
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == 0)
    {
      HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);
    }
    break;
  case GPIO_PIN_0:
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == 1)
    {
      HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_5);

      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    }
    break;
  }
}
/*********************end**********************/


void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
