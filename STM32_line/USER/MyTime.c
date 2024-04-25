#include "include.h"

u32 ServoCount;
extern uint8 BuzzerState;
void InitTimer2(void)		//100us
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = (10 - 1); //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =(720-1); //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
	    TIM2, //TIM2
	    TIM_IT_Update  |  //TIM �ж�Դ
	    TIM_IT_Trigger,   //TIM �����ж�Դ
	    ENABLE  //ʹ��
	);

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

void TIM2_IRQHandler(void)   //TIM2�ж�
{//��ʱ��2�ж�  100us
	static uint32 time = 0;
	Buzzer();       //����������
	MotorPwmCtrl(); //�������
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //���ָ����TIM�жϷ������:TIM �ж�Դ
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);    //���TIMx���жϴ�����λ:TIM �ж�Դ
		if(++time >= 10)
		{
			time = 0;
			ServoCount++;
		}
	}
}

void TaskTimeHandle(void)
{
	static uint32 time = 10;
	static uint32 times = 0;
	if(ServoCount > time)
	{
		//time += 10;
		times++;
		ServoCount= 0 ;
		if(times % 2 == 0)//20ms
		{
			ServoPwmDutyCompare();
		}
		if(times % 50 == 0)//500ms
		{
			CheckBatteryVoltage();  //�����
			if(GetBatteryVoltage()<6400)
				BuzzerState = 1;        //��������־λ��
			else
				BuzzerState = 0;        //��������־λ��
		}
	}
}
