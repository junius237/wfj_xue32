#include "time_handle.h"
#include "main.h"
#include "tim.h"

/*
*************************************
�궨��
*************************************
*/
/*
*************************************
�궨��
*************************************
*/
/*
*************************************
��������
*************************************
*/
/*
*************************************
��������
*************************************
*/
static uint16_t index_10ms = 0;  //���ڼ�¼10ms��ʱ���ļ���ֵ
uint16_t index_led = 0;          //��Led�ı�־λ

/**
  * @brief          ��ʱ��2�жϷ�����,10ms��һ���ж�
  * @param[in]      htim:��ʱ��
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



