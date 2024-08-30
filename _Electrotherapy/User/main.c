#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "PWM.h"
#include "Key.h"
#include "Serial.h"

uint8_t flag = 0;
uint8_t KEY_MODE = 0;

int main(void)
{
	Key_Init();
	Serial_Init();
	PWM_Init();

	while (1)
	{
		KEY_MODE = Key_Get_Mode();
		switch (KEY_MODE)
		{
		case 1:
			flag = 0;
			while (flag == 0)
			{
				/*****************************/
				PWM_Mode1_SIN();
				printf("MODE1:SINE");
				/*****************************/
				Key_Get_Mode(); // 判断按键是否按下 若按下flag = 1，KEY_MODE++
			}
			TIM_SetCompare1(TIM2, 0);
			break;
		case 2:
			flag = 0;
			while (flag == 0)
			{
				/*****************************/
				// PWM_SetCompare2(666 - 1);
				/*****************************/

				/***********断续波************/
				PWM_SetCompare2(666-1);
				/************end*************/

				Key_Get_Mode();
			}
			TIM_SetCompare2(TIM2, 0);
			break;
		case 3:
			flag = 0; // 重置标志位
			while (flag == 0)
			{
				/*****************************/
				PWM_SetCompare3(1800); // 占空比100%  1800
				/*****************************/
				Key_Get_Mode();
			}
			TIM_SetCompare3(TIM2, 0);
			break;
		default:
			printf("没有按键按下");
		}
	}
}
