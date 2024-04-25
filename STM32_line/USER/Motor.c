#include "include.h"

int8 M1Speed = 0;
int8 M2Speed = 0;
uint8 M1Dir = 0;
uint8 M2Dir = 0;

void InitMotor(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


void MotorControl(int8 m1Speed,int8 m2Speed)
{
	if(m1Speed >= 0)
	{
		M1Dir = 1;
		M1Speed = m1Speed;
	}
	else
	{
		M1Dir = 0;
		M1Speed = -m1Speed;
	}
	if(m2Speed >= 0)
	{
		M2Dir = 1;
		M2Speed = m2Speed;
	}
	else
	{
		M2Dir = 0;
		M2Speed = -m2Speed;
	}
	M1Speed /= 10;
	M2Speed /= 10;
}

void MotorPwmCtrl(void)
{//�ŵ�100us�Ķ�ʱ�ж�����
	static uint8 time = 0;
	
	
	if(time < M1Speed)//ͨ������M1Speed��ֵ����ռ�ձȣ��Ӷ������ٶ�
	{
		if(M1Dir)
		{
			OUT1 = 1;
			OUT2 = 0;
		}
		else
		{
			OUT1 = 0;
			OUT2 = 1;
		}
	}
	else
	{
		OUT1 = 0;
		OUT2 = 0;
	}
	if(time < M2Speed)
	{
		if(M2Dir)
		{
			OUT3 = 1;
			OUT4 = 0;
		}
		else
		{
			OUT3 = 0;
			OUT4 = 1;
		}
	}
	else
	{
		OUT3 = 0;
		OUT4 = 0;
	}
	if(++time >= 10)
	{//10������  PWM���� = 100us * 10 = 1ms Ƶ�� = 1KHz
		time = 0;
	}
}

