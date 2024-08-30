#include "time_handle.h"
#include "main.h"
#include "tim.h"

/*
*************************************
宏定义
*************************************
*/
/*
*************************************
宏定义
*************************************
*/
/*
*************************************
变量声明
*************************************
*/
/*
*************************************
变量定义
*************************************
*/
static uint16_t index_10ms = 0;  //用于记录10ms定时器的计数值
uint16_t index_led = 0;          //给Led的标志位

/**
  * @brief          定时器2中断服务函数,10ms进一次中断
  * @param[in]      htim:定时器
  * @retval         none
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)	
{
	if (htim->Instance == htim2.Instance)
	{
		index_10ms++;	 
		if(index_10ms%100==0)
		{
			index_led=1;
		}
		if(index_10ms>=200)
		{
			index_10ms = 0;
		}
	}
	
	
}



