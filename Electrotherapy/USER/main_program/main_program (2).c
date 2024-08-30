#include "main_program.h"
#include "time_handle.h"
#include "tim.h"
#include "esp8266.h"
#include "main.h"
//#include "WaveCtrl.h"


/*
**************************************
宏定义
**************************************
*/

/*
**************************************
变量声明
**************************************
*/
extern uint16_t index_led;
/*
**************************************
函数声明
**************************************
*/

/*
**************************************
变量定义
**************************************
*/




/*
**************************************
函数定义
**************************************
*/



/**
	* @brief  初始化函数，只在初始化的时候执行一次
	* @param  None
	* @retval None
*/

void USER_Init_Program(void)
{
/*
	将需要初始化的函数写到下方
*/ 
	delay_init(72);               	//初始化延时函数
//	esp8266_init();
	
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);		// 使能PWM输出
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);		// 使能PWM输出
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);		// 使能PWM输出
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);		// 使能PWM输出
	       
}

/**
	* @brief  在main.c while 中执行User_main_Program。
	* @param  None
	* @retval None
*/

void User_main_Program(void)
{
	

	PWM_WAVE_MODE WAVE_channel;
	WAVE_channel.WAVE  =2;		//Wave_status;	
	WAVE_channel.FREQ  =100;	//Freq_status;	
	WAVE_channel.AMPL1 =0.2;	//AMP1_status;	
	WAVE_channel.AMPL2 =3.0;  	//AMP2_status;	
	WAVE_channel.AMPL3 =3.3;  	//AMP3_status;	
	PWM_Mode1_SIN(&WAVE_channel);

                  

	
	if(index_led == 1)
	{
		LED_status==0?HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET):HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_2,GPIO_PIN_SET);
		index_led =0;
	}
	
	
	if(esp8266_receive_msg() != 1)
	{
		
	}
	else
	{
		
	}
	
	
		
}







/*

#define SINE_TABLE_SIZE 100 // 正弦波表的大小
uint16_t sine_table[SINE_TABLE_SIZE]; // 预先计算好的正弦波表

void generate_sine_wave(uint16_t amplitude) {
    for (int i = 0; i < SINE_TABLE_SIZE; i++) {
        // 根据正弦函数计算波形值并存入 sine_table 数组
        sine_table[i] = amplitude * sin(2 * M_PI * i / SINE_TABLE_SIZE);
    }

    uint16_t index = 0; // 波形索引
    uint16_t freq = 10; // 波形频率
    while (1) {
        // 将当前索引对应的波形值写入 TIM 的 CCR 寄存器
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, sine_table[index]);
        index = (index + freq) % SINE_TABLE_SIZE; // 更新索引
        // 其他代码...
    }
}



#define SPARSE_TABLE_SIZE 100 // 疏密波表的大小
uint16_t sparse_table[SPARSE_TABLE_SIZE];

void generate_sparse_wave(uint16_t amplitude) {
    for (int i = 0; i < SPARSE_TABLE_SIZE; i++) {
        // 每隔一定间隔设置为 amplitude, 其余设置为 0
        sparse_table[i] = (i % 10 == 0) ? amplitude : 0;
    }

    uint16_t index = 0;
    uint16_t freq = 10;
    while (1) {
        __HAL_TIM_SetCompare(&htim, TIM_CHANNEL_1, sparse_table[index]);
        index = (index + freq) % SPARSE_TABLE_SIZE;
        // 其他代码...
    }
}






#define INTERMITTENT_TABLE_SIZE 100 // 断续波表的大小
uint16_t intermittent_table[INTERMITTENT_TABLE_SIZE];

void generate_intermittent_wave(uint16_t amplitude) {
    for (int i = 0; i < INTERMITTENT_TABLE_SIZE; i++) {
        // 每隔一定间隔设置为 amplitude, 其余设置为 0
        intermittent_table[i] = (i % 20 < 10) ? amplitude : 0;
    }

    uint16_t index = 0;
    uint16_t freq = 10;
    while (1) {
        __HAL_TIM_SetCompare(&htim, TIM_CHANNEL_1, intermittent_table[index]);
        index = (index + freq) % INTERMITTENT_TABLE_SIZE;
        // 其他代码...
    }
}



*/