/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "delay.h"
#include "usart.h"
#include "gpio.h"
#include "servo.h"
#include "busservo.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define CAM_DEFAULT_I2C_ADDRESS (0x34) // I2C地址
#define MOTOR_TYPE_ADDR 20             // 编码电机类型设置寄存器地址
#define MOTOR_FIXED_SPEED_ADDR 51      // 速度寄存器地址。属于闭环控制
#define MOTOR_ENCODER_POLARITY_ADDR 21 // 电机编码方向极性地址
#define MOTOR_FIXED_PWM_ADDR 31        // 固定PWM控制地址，属于开环控制
#define MOTOR_ENCODER_TOTAL_ADDR 60    // 4个编码电机各自的总脉冲值
#define ADC_BAT_ADDR 0
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
// 电机类型具体地址
#define MOTOR_TYPE_WITHOUT_ENCODER 0      // 无编码器的电机,磁环每转是44个脉冲减速比:90  默认
#define MOTOR_TYPE_TT 1                   // TT编码电机
#define MOTOR_TYPE_N20 2                  // N20编码电机
#define MOTOR_TYPE_JGB37_520_12V_110RPM 3 // 磁环每转是44个脉冲   减速比:90  默认

/*用数组传递电机速度，正数为设置前进速度，负数为设置后退速度
  以p1、p2为例：p1=4个电机以50的速度前进    p2=4个电机以20的速度后退*/
uint8_t p1[4] = {10, 10, 10, 10};     // 电机转速设置
uint8_t p2[4] = {-10, -10, -10, -10}; // 电机转速设置
uint8_t MotorEncoderPolarity = 1;     // 电机极性控制变量
uint32_t EncodeTotal[4];              // 用于暂存电机累积转动量的值，正转递增，反转递减
uint8_t MotorType = MOTOR_TYPE_TT;    // 设置电机类型

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint8_t data[3]; // 用于暂存电压ADC数据

// 读取多个字节的数据（Reg：地址  Buf：数据内容 Len：数据长度）
uint8_t I2C_Read_Len(uint8_t Reg, uint8_t *Buf, uint8_t Len)
{
  uint8_t i;
  iic_stop();                                       // 发送起始信号
  IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS << 1) | 0); // 发送元器件地址+写指令
  if (iic_wait_ack() == 1)                           // 等待响应，如果失败，发送停止信号，返回1
  {
    iic_stop();
    return 1;
  }
  IIC_send_byte(Reg);      // 发送寄存器地址
  if (iic_wait_ack() == 1) // 等待响应，如果失败，发送停止信号，返回1
  {
    iic_stop();
    return 1;
  }
  iic_stop();                                       // 发送起始信号
  IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS << 1) | 1); // 发送元器件地址+读指令
  if (iic_wait_ack() == 1)                           // 等待响应，如果失败，发送停止信号，返回1
  {
    iic_stop();
    return 1;
  }
  for (i = 0; i < Len; i++) // for循环Len次读取
  {
    if (i != Len - 1) // 如果不是最后一次
    {
      Buf[i] = iic_read_byte(1); // 保存第I次的数据到数组的第I位，并发送答应信号
    }
    else
      Buf[i] = iic_read_byte(0); // 保存第I次的数据到数组的第I位，并发送非答应信号
  }
  iic_stop(); // 发送停止信号
  return 0;   // 读取成功，返回0
}

// 循环发送一个数组的数据（addr：地址  buf：数据内容  leng：数据长度）
uint8_t I2C_Write_Len(uint8_t Reg, uint8_t *Buf, uint8_t Len) // I2C的写数据
{
  uint8_t i;
  iic_stop();                                       // 在起始信号后必须发送一个7位从机地址+1位方向位，用“0”表示主机发送数据，“1”表示主机接收数据。
  IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS << 1) | 0); // 发送 器件地址+写的命令
  if (iic_wait_ack() == 1)                           // 等待响应，如果失败，发送停止信号，返回1
  {
    iic_stop();
    return 1;
  }
  IIC_send_byte(Reg);      // 发送 寄存器地址
  if (iic_wait_ack() == 1) // 等待响应，如果失败，发送停止信号，返回1
  {
    iic_stop();
    return 1;
  }
  for (i = 0; i < Len; i++) // 循环 len 次写数据
  {
    IIC_send_byte(Buf[i]);   // 发送第i位的8位数据
    if (iic_wait_ack() == 1) // 等待响应，如果失败，发送停止信号，返回1
    {
      iic_stop();
      return 1;
    }
  }
  iic_stop(); // 发送结束，发送停止信号
  return 0;   // 返回 0，确定发送成功
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  uint16_t v;
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_TIM2_Init();
  delay_init(72);

  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  delay_ms(200);
  I2C_Write_Len(MOTOR_TYPE_TT, &MotorType, 1); // 在电机类型地址中写入电机类型编号
  delay_ms(5);
  I2C_Write_Len(MOTOR_ENCODER_POLARITY_ADDR, &MotorEncoderPolarity, 1); // 设置电机极性设置
  delay_ms(5);

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    I2C_Read_Len(ADC_BAT_ADDR, data, 2); // 读取电机电压
    v = data[0] + (data[1] << 8);        // 转换电压
    // printf("V = ");
    // printf("%d", v);
    // printf("mV\n"); // 打印电压
    I2C_Read_Len(MOTOR_ENCODER_TOTAL_ADDR, (uint8_t *)EncodeTotal, 16);
    // printf("Encode1 = %ld  Encode2 = %ld  Encode3 = %ld  Encode4 = %ld  \r\n", EncodeTotal[0], EncodeTotal[1], EncodeTotal[2], EncodeTotal[3]);
    /*在电机转速地址中写入电机的转动方向和速度：WireWriteDataArray（转速控制地址，电机转速数组，电机个数）*/
    I2C_Write_Len(MOTOR_FIXED_SPEED_ADDR, p1, 4); // 控制电机转动
    delay_us(1000);
    I2C_Write_Len(MOTOR_FIXED_SPEED_ADDR, p2, 4); // 控制电机转动
    delay_us(1000);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
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
