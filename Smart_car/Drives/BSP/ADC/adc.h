#ifndef __ADC_H
#define __ADC_H

#include "./SYSTEM/sys/sys.h"
#define ADC_BAT		1		//电池电压的AD检测通道
uint32_t adc_get_result(uint32_t ch);
void adc_init(void);
void Check_Battery_Voltage(void);
uint16_t Get_Battery_Voltage(void);
void adc_dma_init(uint32_t mar);
void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch, uint32_t rank, uint32_t stime);
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc);
#endif

