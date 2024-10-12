/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/
uint8_t RxByte;
uint8_t RxByteArr[4];
/* Private variables----------------------------------------------------------*/
static void Peripheral_Set(void); 

/* Public variables-----------------------------------------------------------*/
MyInit_t MyInit = 
{
	Peripheral_Set
};

/* Private function prototypes------------------------------------------------*/      


/*
	* @name   Peripheral_Set
	* @brief  Õ‚…Ë…Ë÷√
	* @param  None
	* @retval None      
*/
static void Peripheral_Set()
{

	HAL_UART_Receive_IT(&huart2,RxByteArr,4);

	OLED_Init();
	OLED_Clear();
	OLED_ShowString(6,3,"Ultrasonic TEST",16);
	HAL_Delay(500);
	OLED_Clear();
}

/********************************************************
  End Of File
********************************************************/
