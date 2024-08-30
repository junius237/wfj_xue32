#ifndef __TIME_HANDLE_H__
#define __TIME_HANDLE_H__

#include "main.h"

typedef struct {
    uint8_t WAVE; 
    float FREQ; 
    float AMPL1; // 通道1的幅值
    float AMPL2; // 通道2的幅值
    float AMPL3; // 通道3的幅值
} PWM_WAVE_MODE; 

void time_slot_start(void);
void PWM_Mode1_SIN(PWM_WAVE_MODE* MODE);
void update_pwm_waveform(const uint16_t* wave_array, float amplitude, uint16_t freq, uint8_t channel,uint16_t Array_Max_value);
void generateSparseDenseArray(void);
void generateDiscreteWaveArray(int numPulses, int pulseWidth, int pulseMin, int pulseMax);
//uint16_t sine_index;
//uint16_t sparse_index;
//uint16_t intermittent_index;

#endif

    