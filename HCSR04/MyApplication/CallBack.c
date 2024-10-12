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

//接收完成中断
// Flag:0----无效数据，1----距离太近，2----正常测距，3----数据错误
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
 {
	 if(huart->Instance == USART2)
		{
			 if(RxByteArr[0] == 0X5A)
			 {
					//判断接收的数据正确性  模式1，十六进制接收，4个字节
					if(RxByteArr[3] == RxByteArr[0]^RxByteArr[1]^RxByteArr[2])
					{
						//判断数据有效性，如果接收到第2,3个字节是0XFF，那么认为是错误数据
						if(RxByteArr[1]== 0xff && RxByteArr[2]== 0xff)//认为是无效数据
						{
								Flag = 0;					
						}
						else if(RxByteArr[1]== 0 && RxByteArr[2]== 0)//认为距离太近
						{
						  //对距离进行计算
							DistanceValue = 0;
							Flag = 1;		
						}
						else//数据正常，进行测距
						{
						  //对距离进行计算
							DistanceValue = RxByteArr[1]<<8 | RxByteArr[2];
							Flag = 2;		
						}
							
					}
					else//数据错误
						Flag = 3;			 
			 }
			HAL_UART_Receive_IT(&huart2,RxByteArr,4);
		}
 }
 
 


 