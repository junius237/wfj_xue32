#include "WaveCtrl.h"
#include "tim.h"
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
extern TIM_HandleTypeDef htim3;

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

void TIM3_GeneratePWM(TIM_HandleTypeDef *htim, uint8_t wave, uint32_t freq_value, uint32_t counter)
{
    uint32_t auto_reload = __HAL_TIM_GET_AUTORELOAD(htim);
    uint32_t compare_value;

    switch (wave)
    {
    case 0:
        // 生成正弦波
        compare_value = (uint32_t)((float)auto_reload * (sin(2 * M_PI * counter / auto_reload) + 1) / 2);
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_1, compare_value);
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_2, compare_value);
        __HAL_TIM_SetCompare(htim, TIM_CHANNEL_3, compare_value);
        break;

    case 1:
        // 生成疏密波
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
        // 生成断续波
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