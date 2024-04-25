/******************************************************
//	Company:���ڻö��Ƽ����޹�˾										   //	
//	���ǵĵ���:lobot-zone.taobao.com                  //
//	���ǵĹ�����www.lobot-robot.com                   //
******************************************************/


#include "include.h"

#define CAM_DEFAULT_I2C_ADDRESS       (0x34)			//I2C��ַ
#define MOTOR_TYPE_ADDR               20 				//�������������üĴ�����ַ
#define MOTOR_FIXED_SPEED_ADDR       	51				//�ٶȼĴ�����ַ�����ڱջ�����
#define MOTOR_ENCODER_POLARITY_ADDR   21				//������뷽���Ե�ַ
#define MOTOR_FIXED_PWM_ADDR      		31				//�̶�PWM���Ƶ�ַ�����ڿ�������
#define MOTOR_ENCODER_TOTAL_ADDR  		60				//4�����������Ե�������ֵ
#define ADC_BAT_ADDR                  0					//��ѹ��ַ



uint8_t data[3]; 																//�����ݴ��ѹADC����


//��ȡ����ֽڵ����ݣ�Reg����ַ  Buf���������� Len�����ݳ��ȣ�
uint8_t I2C_Read_Len(uint8_t Reg,uint8_t *Buf,uint8_t Len)
{
	uint8_t i;
	IIC_start();																				//������ʼ�ź�
	IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS << 1) | 0);	//����Ԫ������ַ+дָ��
	if(IIC_wait_ack() == 1)															//�ȴ���Ӧ�����ʧ�ܣ�����ֹͣ�źţ�����1
	{
		IIC_stop();					
		return 1;					
	}
	IIC_send_byte(Reg);																	//���ͼĴ�����ַ
	if(IIC_wait_ack() == 1)															//�ȴ���Ӧ�����ʧ�ܣ�����ֹͣ�źţ�����1
	{
		IIC_stop();
		return 1;
	}
	IIC_start();																				//������ʼ�ź�
	IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS << 1) | 1);	//����Ԫ������ַ+��ָ��
	if(IIC_wait_ack() == 1)															//�ȴ���Ӧ�����ʧ�ܣ�����ֹͣ�źţ�����1
	{
		IIC_stop();
		return 1;
	}
	for(i=0;i<Len;i++)																	//forѭ��Len�ζ�ȡ
	{	
		if(i != Len-1)																		//����������һ��
		{
			Buf[i] = IIC_read_byte(1);											//�����I�ε����ݵ�����ĵ�Iλ�������ʹ�Ӧ�ź�
		}
		else
			Buf[i] = IIC_read_byte(0);											//�����I�ε����ݵ�����ĵ�Iλ�������ͷǴ�Ӧ�ź�
	}
	IIC_stop();																					//����ֹͣ�ź�
	return 0;																						//��ȡ�ɹ�������0
}

//ѭ������һ����������ݣ�addr����ַ  buf����������  leng�����ݳ��ȣ�
uint8_t I2C_Write_Len(uint8_t Reg,uint8_t *Buf,uint8_t Len)//I2C��д����
{
	uint8_t i;
	IIC_start();																				//����ʼ�źź���뷢��һ��7λ�ӻ���ַ+1λ����λ���á�0����ʾ�����������ݣ���1����ʾ�����������ݡ�
	IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS << 1) | 0);	//���� ������ַ+д������
	if(IIC_wait_ack() == 1)															//�ȴ���Ӧ�����ʧ�ܣ�����ֹͣ�źţ�����1
	{
		IIC_stop();					
		return 1;					
	}
	IIC_send_byte(Reg);																	//���� �Ĵ�����ַ
	if(IIC_wait_ack() == 1)															//�ȴ���Ӧ�����ʧ�ܣ�����ֹͣ�źţ�����1
	{
		IIC_stop();
		return 1;
	}
	for(i =0;i<Len;i++)																	//ѭ�� len ��д����
	{
		IIC_send_byte(Buf[i]);														//���͵�iλ��8λ����
		if(IIC_wait_ack() == 1)														//�ȴ���Ӧ�����ʧ�ܣ�����ֹͣ�źţ�����1
		{
			IIC_stop();
			return 1;
		}
	}
	IIC_stop();																					//���ͽ���������ֹͣ�ź�
	return 0;																						//���� 0��ȷ�����ͳɹ�
}

//������;����ַ
#define MOTOR_TYPE_WITHOUT_ENCODER        0 		//�ޱ������ĵ��,�Ż�ÿת��44��������ٱ�:90  Ĭ��
#define MOTOR_TYPE_TT                     1 		//TT������
#define MOTOR_TYPE_N20                    2 		//N20������
#define MOTOR_TYPE_JGB37_520_12V_110RPM   3 		//�Ż�ÿת��44������   ���ٱ�:90  Ĭ��

/*�����鴫�ݵ���ٶȣ�����Ϊ����ǰ���ٶȣ�����Ϊ���ú����ٶ�
  ��p1��p2Ϊ����p1=4�������50���ٶ�ǰ��    p2=4�������20���ٶȺ���*/
uint8_t p1[4]={10,10,10,10};      										//���ת������                       
uint8_t p2[4]={-10,-10,-10,-10};    									//���ת������ 
uint8_t MotorEncoderPolarity = 1; 							  //������Կ��Ʊ���
uint32_t EncodeTotal[4];													//�����ݴ����ۻ�ת������ֵ����ת��������ת�ݼ�
uint8_t MotorType = MOTOR_TYPE_TT; 								//���õ������
	
int main(void)
{
	
	u16 v;																  						//�����ݴ��ѹֵ
	SystemInit(); 																			//ϵͳʱ�ӳ�ʼ��Ϊ72M	  SYSCLK_FREQ_72MHz
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	InitDelay(72); 																			//��ʼ����ʱ����
	InitTimer2();																				//��ʱ��2��ʼ��
	IIC_init();																					//IIC��ʼ��
	Usart1_Init(); 																			//���ڳ�ʼ��
	InitLED();        																	//��ʼ��LEDָʾ��
	DelayMs(200);
	I2C_Write_Len(MOTOR_TYPE_TT,&MotorType,1);					//�ڵ�����͵�ַ��д�������ͱ��
	DelayMs(5);
	I2C_Write_Len(MOTOR_ENCODER_POLARITY_ADDR,&MotorEncoderPolarity,1);		//���õ����������
	DelayMs(5);
	while(1)
	{
		I2C_Read_Len(ADC_BAT_ADDR,data,2);									//��ȡ�����ѹ
		v = data[0] + (data[1]<<8); 												//ת����ѹ
		printf("V = ");		printf("%d",v);  	printf("mV\n"); //��ӡ��ѹ
		I2C_Read_Len(MOTOR_ENCODER_TOTAL_ADDR,(uint8_t*)EncodeTotal,16);
		printf("Encode1 = %ld  Encode2 = %ld  Encode3 = %ld  Encode4 = %ld  \r\n", EncodeTotal[0], EncodeTotal[1], EncodeTotal[2], EncodeTotal[3]);
		/*�ڵ��ת�ٵ�ַ��д������ת��������ٶȣ�WireWriteDataArray��ת�ٿ��Ƶ�ַ�����ת�����飬���������*/
		I2C_Write_Len(MOTOR_FIXED_SPEED_ADDR,p1,4);						//���Ƶ��ת��
		DelayMs(1000);
		I2C_Write_Len(MOTOR_FIXED_SPEED_ADDR,p2,4);						//���Ƶ��ת��
		DelayMs(1000);
	}
}

