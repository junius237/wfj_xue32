#include "main_program.h"
#include "time_handle.h"
#include "tim.h"
#include "esp8266.h"
#include "main.h"

/*
**************************************
�궨��
**************************************
*/

/*
**************************************
��������
**************************************
*/
extern uint16_t index_led;
/*
**************************************
��������
**************************************
*/

/*
**************************************
��������
**************************************
*/

/*
**************************************
��������
**************************************
*/

/**
 * @brief  ��ʼ��������ֻ�ڳ�ʼ����ʱ��ִ��һ��
 * @param  None
 * @retval None
 */
extern uint16_t i;
void USER_Init_Program(void)
{
	/*
		����Ҫ��ʼ���ĺ���д���·�
	*/
	delay_init(72); // ��ʼ����ʱ����
	//	esp8266_init();

	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // ʹ��PWM���
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); // ʹ��PWM���
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); // ʹ��PWM���
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4); // ʹ��PWM���
	HAL_TIM_Base_Start_IT(&htim3);
	generateSparseDenseArray();
	generateDiscreteWaveArray(1, 500, 300, 300);
	i = 0;
}

/**
 * @brief  ��main.c while ��ִ��User_main_Program��
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
