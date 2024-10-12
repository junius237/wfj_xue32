/* Includes ------------------------------------------------------------------*/
#include "MyApplication.h"

/* Private define-------------------------------------------------------------*/

/* Private variables----------------------------------------------------------*/
static void Run(void); 
static void Error_Handler(void);
static void Assert_Failed(void);


extern uint16_t DistanceValue;
uint8_t OledArr[10];	
extern uint8_t Flag;
/* Public variables-----------------------------------------------------------*/
System_t System = 
{
	Run,
	Error_Handler,
	Assert_Failed
};

/* Private function prototypes------------------------------------------------*/      

/*
	* @name   Run
	* @brief  系统运行
	* @param  None
	* @retval None      
*/
static void Run()
{
   HAL_Delay(20);
	 if(Flag == 2)
	 {
			if(DistanceValue>=10000)
			{
				OledArr[0] = DistanceValue/10000+0x30;
				OledArr[1] = DistanceValue/1000%10+0x30;
				OledArr[2] = DistanceValue/100%10+0x30;
				OledArr[3] = DistanceValue/10%10+0x30;
				OledArr[4] = DistanceValue%10+0x30;
				OledArr[5] = 'm';
				OledArr[6] = 'm';
				OledArr[7] = ' ';
				OledArr[8] = ' ';
				OledArr[9] = ' ';
			}
			else if(DistanceValue>=1000)
			{
				OledArr[0] = DistanceValue/1000+0x30;
				OledArr[1] = DistanceValue/100%10+0x30;
				OledArr[2] = DistanceValue/10%10+0x30;
				OledArr[3] = DistanceValue%10+0x30;
				OledArr[4] = 'm';
				OledArr[5] = 'm';
				OledArr[6] = ' ';
				OledArr[7] = ' ';
				OledArr[8] = ' ';
				OledArr[9] = ' ';
			}
			else if(DistanceValue>=100)
			{
				OledArr[0] = DistanceValue/100+0x30;
				OledArr[1] = DistanceValue/10%10+0x30;
				OledArr[2] = DistanceValue%10+0x30;
				OledArr[3] = 'm';
				OledArr[4] = 'm';
				OledArr[5] = ' ';
				OledArr[6] = ' ';
				OledArr[7] = ' ';
				OledArr[8] = ' ';
				OledArr[9] = ' ';
			}
			else if(DistanceValue>=10)
			{
				OledArr[0] = DistanceValue/10+0x30;
				OledArr[1] = DistanceValue%10+0x30;
				OledArr[2] = 'm';
				OledArr[3] = 'm';
				OledArr[4] = ' ';
				OledArr[5] = ' ';
				OledArr[6] = ' ';
				OledArr[7] = ' ';
				OledArr[8] = ' ';
				OledArr[9] = ' ';
			}
			else
			{
				OledArr[0] = 'E';
				OledArr[1] = 'r';
				OledArr[2] = 'r';
				OledArr[3] = 'o';
				OledArr[4] = 'r';
				OledArr[5] = ' ';
				OledArr[6] = ' ';
				OledArr[7] = ' ';
				OledArr[8] = ' ';
				OledArr[9] = ' ';
			}
			OLED_ShowString(20,2,"Now Distance",16);
			OLED_ShowString(30,4,OledArr,16);	 		 
			printf("实时距离：%d\r\n",DistanceValue);						
	 }
	 else if(Flag == 1)
	 {
	   OLED_ShowString(20,2,"Data invalid",16);
		 OLED_ShowString(30,4,"Too close ",16);	 
		 printf("距离太近\r\n");	
	 }
	 else if(Flag == 0)
	 {
	   OLED_ShowString(20,2,"Data invalid",16);
		 OLED_ShowString(30,4,"----------",16);	 
		 printf("距离无效\r\n");	
	 }
	 else if(Flag == 3)
	 {
	   OLED_ShowString(20,2,"Data invalid",16);
		 OLED_ShowString(30,4,"Data Error",16);	 
	   printf("数据错误\r\n");			 
	 }
}

/*
	* @name   Error_Handler
	* @brief  系统错误处理
	* @param  None
	* @retval None      
*/
static void Error_Handler()
{
	/* User can add his own implementation to report the HAL error return state */
}

/*
	* @name   Assert_Failed
	* @brief  函数参数错误处理
	* @param  None
	* @retval None      
*/
static void Assert_Failed()
{
	/* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
/********************************************************
  End Of File
********************************************************/
