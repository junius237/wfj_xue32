#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"
void key_init(void)
    {
    GPIO_InitTypeDef gpio_init_truct;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();
    
    gpio_init_truct.Pin=GPIO_PIN_4;
    gpio_init_truct.Mode=GPIO_MODE_INPUT;
    gpio_init_truct.Pull=GPIO_PULLUP;
    
    HAL_GPIO_Init(GPIOE,&gpio_init_truct);
    
}
uint8_t key_saomiao(void)
{

    if (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)==0)
    {
        
       delay_ms(10);    /*软件消抖*/

        if (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)==0)
        {

            while (HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4)==0);
            return 1;   /*按键按下*/

        }
        
    }
    return 0;   /*按键未按下*/


}

