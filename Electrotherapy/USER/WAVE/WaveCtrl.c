#include "WaveCtrl.h"
#include "tim.h"
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
extern TIM_HandleTypeDef htim3;

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

void TIM3_GeneratePWM(TIM_HandleTypeDef *htim, uint8_t wave, uint32_t freq_value, uint32_t counter)
{
    uint32_t auto_reload = __HAL_TIM_GET_AUTORELOAD(htim);
    uint32_t compare_value;

    switch (wave)
    {
    case 0:
        // �������Ҳ�
        compare_value = (uint32_t)((float)auto_reload * (sin(2 * M_PI * counter / auto_reload) + 1) / 2);
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_1, compare_value);
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, compare_value);
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_3, compare_value);
        break;

    case 1:
        // �������ܲ�
        if (counter % (auto_reload / freq_value) == 0)
        {
            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_1, auto_reload / 2);
            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, auto_reload / 2);
            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_3, auto_reload / 2);
        }
        else
        {
            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_1, 0);
            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, 0);
            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_3, 0);
        }
        break;

    case 2:
        // ���ɶ�����
        if (counter % (auto_reload / (freq_value / 2)) < (auto_reload / freq_value) / 2)
        {
            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_1, auto_reload / 2);
            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, auto_reload / 2);
            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_3, auto_reload / 2);
        }
        else
        {
            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_1, 0);
            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, 0);
            __HAL_TIM_SetCompare(htim, TIM_CHANNEL_3, 0);
        }
        break;
    default:
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_1, 0);
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, 0);
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_3, 0);
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