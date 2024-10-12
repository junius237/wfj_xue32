/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/
extern	uint8_t RxByte;
extern	uint8_t RxByteArr[4];

uint8_t RxBuff[10];
uint16_t RxCount=0;
uint16_t DistanceValue;

uint8_t Flag = 0;
	
	
/* Private variables----------------------------------------------------------*/

/* Public variables-----------------------------------------------------------*/

/* Private function prototypes------------------------------------------------*/      

/********************************************************
  End Of File
********************************************************/

//��������ж�
// Flag:0----��Ч���ݣ�1----����̫����2----������࣬3----���ݴ���
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
 {
	 if(huart->Instance == USART2)
		{
			 if(RxByteArr[0] == 0X5A)
			 {
					//�жϽ��յ�������ȷ��  ģʽ1��ʮ�����ƽ��գ�4���ֽ�
					if(RxByteArr[3] == RxByteArr[0]^RxByteArr[1]^RxByteArr[2])
					{
						//�ж�������Ч�ԣ�������յ���2,3���ֽ���0XFF����ô��Ϊ�Ǵ�������
						if(RxByteArr[1]== 0xff && RxByteArr[2]== 0xff)//��Ϊ����Ч����
						{
								Flag = 0;					
						}
						else if(RxByteArr[1]== 0 && RxByteArr[2]== 0)//��Ϊ����̫��
						{
						  //�Ծ�����м���
							DistanceValue = 0;
							Flag = 1;		
						}
						else//�������������в��
						{
						  //�Ծ�����м���
							DistanceValue = RxByteArr[1]<<8 | RxByteArr[2];
							Flag = 2;		
						}
							
					}
					else//���ݴ���
						Flag = 3;			 
			 }
			HAL_UART_Receive_IT(&huart2,RxByteArr,4);
		}
 }
 
 


 