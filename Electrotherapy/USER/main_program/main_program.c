#include "main_program.h"
#include "time_handle.h"
#include "tim.h"
#include "esp8266.h"
#include "main.h"

/*
**************************************
宏定义
**************************************
*/

/*
**************************************
变量声明
**************************************
*/
extern uint16_t index_led;
/*
**************************************
函数声明
**************************************
*/

/*
**************************************
变量定义
**************************************
*/

/*
**************************************
函数定义
**************************************
*/

/**
 * @brief  初始化函数，只在初始化的时候执行一次
 * @param  None
 * @retval None
 */
extern uint16_t i;
void USER_Init_Program(void)
{
	/*
		将需要初始化的函数写到下方
	*/
	delay_init(72); // 初始化延时函数
	//	esp8266_init();

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // 使能PWM输出
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); // 使能PWM输出
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); // 使能PWM输出
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4); // 使能PWM输出
	HAL_TIM_Base_Start_IT(&htim3);
	generateSparseDenseArray();
	generateDiscreteWaveArray(1, 500, 300, 300);
	i = 0;
}

/**
 * @brief  在main.c while 中执行User_main_Program。
 * @param  None
 * @retval None
 */
PWM_WAVE_MODE WAVE_channel;
void User_main_Program(void)
{

	WAVE_channel.WAVE = 2;	  // Wave_status;
	WAVE_channel.FREQ = 1;	  // Freq_status;
	WAVE_channel.AMPL1 = 20;  // AMP1_status;
	WAVE_channel.AMPL2 = 50;  // AMP2_status;
	WAVE_channel.AMPL3 = 100; // AMP3_status;


	//	__HAL_TIM_SET_AUTORELOAD(&htim3, 1000000/1000-1);
	//  __HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,50.0/100*(1000000/50));

	if (index_led == 1)
	{
		LED_status == 0 ? HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET) : HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
		index_led = 0;
	}

	if (esp8266_receive_msg() != 1)
	{
	}
	else
	{
	}
}
