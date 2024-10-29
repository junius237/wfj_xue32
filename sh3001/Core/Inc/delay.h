#ifndef __DELAY_H
#define __DELAY_H

#include "stm32h7xx.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void delay_ms(uint32_t nms);
void delay_us(uint32_t nus);
void delay_init(void);

#endif
