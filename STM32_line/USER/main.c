/******************************************************
//	Company:深圳幻尔科技有限公司										   //	
//	我们的店铺:lobot-zone.taobao.com                  //
//	我们的官网：www.lobot-robot.com                   //
******************************************************/


#include "include.h"

#define CAM_DEFAULT_I2C_ADDRESS       (0x34)			//I2C地址
#define MOTOR_TYPE_ADDR               20 				//编码电机类型设置寄存器地址
#define MOTOR_FIXED_SPEED_ADDR       	51				//速度寄存器地址。属于闭环控制
#define MOTOR_ENCODER_POLARITY_ADDR   21				//电机编码方向极性地址
#define MOTOR_FIXED_PWM_ADDR      		31				//固定PWM控制地址，属于开环控制
#define MOTOR_ENCODER_TOTAL_ADDR  		60				//4个编码电机各自的总脉冲值
#define ADC_BAT_ADDR                  0					//电压地址



uint8_t data[3]; 																//用于暂存电压ADC数据


//读取多个字节的数据（Reg：地址  Buf：数据内容 Len：数据长度）
uint8_t I2C_Read_Len(uint8_t Reg,uint8_t *Buf,uint8_t Len)
{
	uint8_t i;
	IIC_start();																				//发送起始信号
	IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS << 1) | 0);	//发送元器件地址+写指令
	if(IIC_wait_ack() == 1)															//等待响应，如果失败，发送停止信号，返回1
	{
		IIC_stop();					
		return 1;					
	}
	IIC_send_byte(Reg);																	//发送寄存器地址
	if(IIC_wait_ack() == 1)															//等待响应，如果失败，发送停止信号，返回1
	{
		IIC_stop();
		return 1;
	}
	IIC_start();																				//发送起始信号
	IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS << 1) | 1);	//发送元器件地址+读指令
	if(IIC_wait_ack() == 1)															//等待响应，如果失败，发送停止信号，返回1
	{
		IIC_stop();
		return 1;
	}
	for(i=0;i<Len;i++)																	//for循环Len次读取
	{	
		if(i != Len-1)																		//如果不是最后一次
		{
			Buf[i] = IIC_read_byte(1);											//保存第I次的数据到数组的第I位，并发送答应信号
		}
		else
			Buf[i] = IIC_read_byte(0);											//保存第I次的数据到数组的第I位，并发送非答应信号
	}
	IIC_stop();																					//发送停止信号
	return 0;																						//读取成功，返回0
}

//循环发送一个数组的数据（addr：地址  buf：数据内容  leng：数据长度）
uint8_t I2C_Write_Len(uint8_t Reg,uint8_t *Buf,uint8_t Len)//I2C的写数据
{
	uint8_t i;
	IIC_start();																				//在起始信号后必须发送一个7位从机地址+1位方向位，用“0”表示主机发送数据，“1”表示主机接收数据。
	IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS << 1) | 0);	//发送 器件地址+写的命令
	if(IIC_wait_ack() == 1)															//等待响应，如果失败，发送停止信号，返回1
	{
		IIC_stop();					
		return 1;					
	}
	IIC_send_byte(Reg);																	//发送 寄存器地址
	if(IIC_wait_ack() == 1)															//等待响应，如果失败，发送停止信号，返回1
	{
		IIC_stop();
		return 1;
	}
	for(i =0;i<Len;i++)																	//循环 len 次写数据
	{
		IIC_send_byte(Buf[i]);														//发送第i位的8位数据
		if(IIC_wait_ack() == 1)														//等待响应，如果失败，发送停止信号，返回1
		{
			IIC_stop();
			return 1;
		}
	}
	IIC_stop();																					//发送结束，发送停止信号
	return 0;																						//返回 0，确定发送成功
}

//电机类型具体地址
#define MOTOR_TYPE_WITHOUT_ENCODER        0 		//无编码器的电机,磁环每转是44个脉冲减速比:90  默认
#define MOTOR_TYPE_TT                     1 		//TT编码电机
#define MOTOR_TYPE_N20                    2 		//N20编码电机
#define MOTOR_TYPE_JGB37_520_12V_110RPM   3 		//磁环每转是44个脉冲   减速比:90  默认

/*用数组传递电机速度，正数为设置前进速度，负数为设置后退速度
  以p1、p2为例：p1=4个电机以50的速度前进    p2=4个电机以20的速度后退*/
uint8_t p1[4]={10,10,10,10};      										//电机转速设置                       
uint8_t p2[4]={-10,-10,-10,-10};    									//电机转速设置 
uint8_t MotorEncoderPolarity = 1; 							  //电机极性控制变量
uint32_t EncodeTotal[4];													//用于暂存电机累积转动量的值，正转递增，反转递减
uint8_t MotorType = MOTOR_TYPE_TT; 								//设置电机类型
	
int main(void)
{
	
	u16 v;																  						//用于暂存电压值
	SystemInit(); 																			//系统时钟初始化为72M	  SYSCLK_FREQ_72MHz
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	InitDelay(72); 																			//初始化延时函数
	InitTimer2();																				//定时器2初始化
	IIC_init();																					//IIC初始化
	Usart1_Init(); 																			//串口初始化
	InitLED();        																	//初始化LED指示灯
	DelayMs(200);
	I2C_Write_Len(MOTOR_TYPE_TT,&MotorType,1);					//在电机类型地址中写入电机类型编号
	DelayMs(5);
	I2C_Write_Len(MOTOR_ENCODER_POLARITY_ADDR,&MotorEncoderPolarity,1);		//设置电机极性设置
	DelayMs(5);
	while(1)
	{
		I2C_Read_Len(ADC_BAT_ADDR,data,2);									//读取电机电压
		v = data[0] + (data[1]<<8); 												//转换电压
		printf("V = ");		printf("%d",v);  	printf("mV\n"); //打印电压
		I2C_Read_Len(MOTOR_ENCODER_TOTAL_ADDR,(uint8_t*)EncodeTotal,16);
		printf("Encode1 = %ld  Encode2 = %ld  Encode3 = %ld  Encode4 = %ld  \r\n", EncodeTotal[0], EncodeTotal[1], EncodeTotal[2], EncodeTotal[3]);
		/*在电机转速地址中写入电机的转动方向和速度：WireWriteDataArray（转速控制地址，电机转速数组，电机个数）*/
		I2C_Write_Len(MOTOR_FIXED_SPEED_ADDR,p1,4);						//控制电机转动
		DelayMs(1000);
		I2C_Write_Len(MOTOR_FIXED_SPEED_ADDR,p2,4);						//控制电机转动
		DelayMs(1000);
	}
}

