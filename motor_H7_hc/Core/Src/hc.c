#include "hc.h"
#include "tim.h"
#include "gpio.h"

float distant;				   // ��������
uint32_t measure_Buf[3] = {0}; // ��Ŷ�ʱ������ֵ������
uint8_t measure_Cnt = 0;	   // ״̬��־λ
uint32_t high_time;			   // ������ģ�鷵�صĸߵ�ƽʱ��

//===============================================��ȡ����
void SR04_GetData(void)
{
	switch (measure_Cnt)
	{
	case 0:
		TRIG_H;
		HAL_Delay(20);
		TRIG_L;

		measure_Cnt++;
		__HAL_TIM_SET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
		HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1); // �������벶��  ����: __HAL_TIM_ENABLE(&htim5);
		break;
	case 3:
		high_time = measure_Buf[1] - measure_Buf[0]; // �ߵ�ƽʱ��
		printf("\r\ntim=%d-us\r\n", high_time);
		distant = (high_time * 0.034) / 2; // ��λcm
		printf("\r\ns=%.2f-cm\r\n", distant);
		measure_Cnt = 0; // ��ձ�־λ
		TIM5->CNT = 0;	 // ��ռ�ʱ������
		break;
	}
}

//===============================================us��ʱ����
void delay_us(uint32_t us) 
{
	uint32_t delay = (HAL_RCC_GetHCLKFreq() / 1000000 * us);
	while (delay--)
	{
		__NOP();
	}
}

//===============================================�жϻص�����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) //
{

	if (TIM5 == htim->Instance) // �жϴ������жϵĶ�ʱ��ΪTIM5
	{
		switch (measure_Cnt)
		{
		case 1:
			measure_Buf[0] = HAL_TIM_ReadCapturedValue(&htim5, TIM_CHANNEL_1);			  // ��ȡ��ǰ�Ĳ���ֵ.
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING); // ����Ϊ�½��ز���
			measure_Cnt++;
			break;
		case 2:
			measure_Buf[1] = HAL_TIM_ReadCapturedValue(&htim5, TIM_CHANNEL_1); // ��ȡ��ǰ�Ĳ���ֵ.
			HAL_TIM_IC_Stop_IT(&htim5, TIM_CHANNEL_1);						   // ֹͣ����   ����: __HAL_TIM_DISABLE(&htim5);
			measure_Cnt++;
		}
	}
}
