#ifndef __HCSR04_H
#define __HCSR04_H

#include "stm32h7xx.h"
#include "tim.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern void HC_Start(void);

extern double hc_sr04_get_distance(void);


#endif
