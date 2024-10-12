/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void LED_ON(uint8_t); 
static void LED_OFF(uint8_t);
static void LED_Flip(uint8_t);



/* Public variables-----------------------------------------------------------*/
LED_t LED = 
{
	LED_ON,
	LED_OFF,
	LED_Flip
};

/* Private function prototypes------------------------------------------------*/      

/*
	* @name   LED_ON
	* @brief  打开LED
	* @param  Num -> 编号
	* @retval None      
*/
static void LED_ON(uint8_t LED_Num)
{
	//条件选择语句
	switch(LED_Num)
	{
		case LED1: HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET); break;
//		case LED2: HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET); break;
//		case LED3: HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_SET); break;
		default: System.Assert_Failed();
	}
}

/*
	* @name   LED_OFF
	* @brief  关闭LED
	* @param  Num -> 编号
	* @retval None      
*/
static void LED_OFF(uint8_t LED_Num)
{
	//条件选择语句
	switch(LED_Num)
	{
		case LED1: HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET); break;
//		case LED2: HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET); break;
//		case LED3: HAL_GPIO_WritePin(LED3_GPIO_Port,LED3_Pin,GPIO_PIN_RESET); break;
		default: System.Assert_Failed();
	}
}

/*
	* @name   LED_Flip
	* @brief  取反LED
	* @param  Num -> 编号
	* @retval None      
*/
static void LED_Flip(uint8_t LED_Num)
{
	//条件选择语句
	switch(LED_Num)
	{
		case LED1: HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin); break;
//		case LED2: HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin); break;
//		case LED3: HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin); break;
		default: System.Assert_Failed();
	}
}
/********************************************************
  End Of File
********************************************************/
