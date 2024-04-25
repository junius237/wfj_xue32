#include "led.h"
#include "stm32f1xx_hal_gpio.h"
void led_init(void){
    GPIO_InitTypeDef gpio_init_truct;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    gpio_init_truct.Pin=GPIO_PIN_5;
    gpio_init_truct.Mode=GPIO_MODE_OUTPUT_PP;
    gpio_init_truct.Speed=GPIO_SPEED_FREQ_LOW;
    
    HAL_GPIO_Init(GPIOB,&gpio_init_truct);
    
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,GPIO_PIN_SET);


}

