#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/PWM/pwm.h"

extern TIM_HandleTypeDef g_timx_pwm_chy_handle; /*句柄声明*/

int main(void)
{

  uint16_t ledrpwmval = 0; /*设置比较值变量*/
  uint8_t dir = 1;         /*比较值加减方向变量*/

  HAL_Init();                         /* 初始化HAL库 */
  sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
  delay_init(72);                     /* 延时初始化 */
  led_init();
  pwm_timx_chy_init(500 - 1, 72 - 1); /*初始化定时器*/

  while (1)
  {
    delay_ms(10);   /*每隔10ms往CCR2写入比较值*/

    if (dir)
      ledrpwmval++; /*dir==1 ledrpwmval递增*/
    else
      ledrpwmval--; /*dir==0 ledrpwmval递减*/

    if (ledrpwmval > 150)
      dir = 0; /*ledrpwmval达到300后，方向改为递减*/
    if (ledrpwmval == 0)
      dir = 1; /*ledrpwmval减到0后，方向改为递增*/

    /*修改比较直控制占空比*/
    __HAL_TIM_SET_COMPARE(&g_timx_pwm_chy_handle, TIM_CHANNEL_2, ledrpwmval);
  }
}
