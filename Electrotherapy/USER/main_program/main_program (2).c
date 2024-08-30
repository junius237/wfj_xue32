#include "main_program.h"
#include "time_handle.h"
#include "tim.h"
#include "esp8266.h"
#include "main.h"
//#include "WaveCtrl.h"


/*
**************************************
�궨��
**************************************
*/

/*
**************************************
��������
**************************************
*/
extern uint16_t index_led;
/*
**************************************
��������
**************************************
*/

/*
**************************************
��������
**************************************
*/




/*
**************************************
��������
**************************************
*/



/**
	* @brief  ��ʼ��������ֻ�ڳ�ʼ����ʱ��ִ��һ��
	* @param  None
	* @retval None
*/

void USER_Init_Program(void)
{
/*
	����Ҫ��ʼ���ĺ���д���·�
*/ 
	delay_init(72);               	//��ʼ����ʱ����
//	esp8266_init();
	
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);		// ʹ��PWM���
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);		// ʹ��PWM���
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);		// ʹ��PWM���
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);		// ʹ��PWM���
	       
}

/**
	* @brief  ��main.c while ��ִ��User_main_Program��
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

#define SINE_TABLE_SIZE 100 // ���Ҳ���Ĵ�С
uint16_t sine_table[SINE_TABLE_SIZE]; // Ԥ�ȼ���õ����Ҳ���

void generate_sine_wave(uint16_t amplitude) {
    for (int i = 0; i < SINE_TABLE_SIZE; i++) {
        // �������Һ������㲨��ֵ������ sine_table ����
        sine_table[i] = amplitude * sin(2 * M_PI * i / SINE_TABLE_SIZE);
    }

    uint16_t index = 0; // ��������
    uint16_t freq = 10; // ����Ƶ��
    while (1) {
        // ����ǰ������Ӧ�Ĳ���ֵд�� TIM �� CCR �Ĵ���
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, sine_table[index]);
        index = (index + freq) % SINE_TABLE_SIZE; // ��������
        // ��������...
    }
}



#define SPARSE_TABLE_SIZE 100 // ���ܲ���Ĵ�С
uint16_t sparse_table[SPARSE_TABLE_SIZE];

void generate_sparse_wave(uint16_t amplitude) {
    for (int i = 0; i < SPARSE_TABLE_SIZE; i++) {
        // ÿ��һ���������Ϊ amplitude, ��������Ϊ 0
        sparse_table[i] = (i % 10 == 0) ? amplitude : 0;
    }

    uint16_t index = 0;
    uint16_t freq = 10;
    while (1) {
        __HAL_TIM_SetCompare(&htim, TIM_CHANNEL_1, sparse_table[index]);
        index = (index + freq) % SPARSE_TABLE_SIZE;
        // ��������...
    }
}






#define INTERMITTENT_TABLE_SIZE 100 // ��������Ĵ�С
uint16_t intermittent_table[INTERMITTENT_TABLE_SIZE];

void generate_intermittent_wave(uint16_t amplitude) {
    for (int i = 0; i < INTERMITTENT_TABLE_SIZE; i++) {
        // ÿ��һ���������Ϊ amplitude, ��������Ϊ 0
        intermittent_table[i] = (i % 20 < 10) ? amplitude : 0;
    }

    uint16_t index = 0;
    uint16_t freq = 10;
    while (1) {
        __HAL_TIM_SetCompare(&htim, TIM_CHANNEL_1, intermittent_table[index]);
        index = (index + freq) % INTERMITTENT_TABLE_SIZE;
        // ��������...
    }
}



*/