#include "./BSP/Servo/servo.h"
#include <stdbool.h>


uint16_t ServoPwmDuty[8]={1500,1500,1500,500,500,900,1500,1500};
uint16_t ServoPwmDuty_Set[8]={1500,1500,1500,500,500,900,1500,1500};

float ServoPwmDuty_updata[8];   /*为控制速度，pwm波发生变化时，每2.5ms或者20ms递增pwm脉冲*/
bool ServoPwmDutyChange_Sign=1;               /*脉冲变化标志*/

uint16_t ServoTime = 2000;  /*舵机从当前角度运动到指定角度的时间，也就是控制速度*/


void ServoSetPluseAndTime(uint8_t id,uint16_t p,uint16_t time)
{
	if((id > 0 || id ==0) && id <= 7 && p >= 500 && p <= 2500)
	{
		if(time < 20)
			time = 20;
		if(time > 30000)
			time = 30000;

		ServoPwmDuty_Set[id] = p;
		ServoTime = time;
		ServoPwmDutyChange_Sign = 1;
	}
}

void ServoPwmDutyCompare(void)//脉宽变化比较及速度控制
{
	uint8_t i;
	
	static uint16_t ServoPwmDuty_updataTimes;	//需要递增的次数
	static bool ServoRunning = 0;	//舵机正在以指定速度运动到指定的脉宽对应的位置
	if(ServoPwmDutyChange_Sign)//停止运动并且脉宽发生变化时才进行计算      ServoRunning == 0 && 
	{
		ServoPwmDutyChange_Sign = 0;
		ServoPwmDuty_updataTimes = ServoTime/20;	//当每20ms调用一次ServoPwmDutyCompare()函数时用此句
		for(i=0;i<8;i++)
		{
			//if(ServoPwmDuty[i] != ServoPwmDuty_Set[i])
			{
				if(ServoPwmDuty_Set[i] > ServoPwmDuty[i])
				{
					ServoPwmDuty_updata[i] = ServoPwmDuty_Set[i] - ServoPwmDuty[i];
					ServoPwmDuty_updata[i] = -ServoPwmDuty_updata[i];
				}
				else
				{
					ServoPwmDuty_updata[i] = ServoPwmDuty[i] - ServoPwmDuty_Set[i];
					
				}
				ServoPwmDuty_updata[i] /= ServoPwmDuty_updataTimes;//每次递增的脉宽
			}
		}
		ServoRunning = 1;	//舵机开始动作
	}
	if(ServoRunning)
	{
		ServoPwmDuty_updataTimes--;
		for(i=0;i<8;i++)
		{
			if(ServoPwmDuty_updataTimes == 0)
			{		//最后一次递增就直接将设定值赋给当前值

				ServoPwmDuty[i] = ServoPwmDuty_Set[i];

				ServoRunning = 0;	//到达设定位置，舵机停止运动
			}
			else
			{

				ServoPwmDuty[i] = ServoPwmDuty_Set[i] + 
					(signed short int)(ServoPwmDuty_updata[i] * ServoPwmDuty_updataTimes);

			}
		}
		
	}
}




