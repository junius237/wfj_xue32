#include "delay.h"
#include "stm32h7xx.h"

static uint32_t fac_us = 0;    // us延时倍乘数
static uint32_t fac_ms = 0;    // ms延时倍乘数

void delay_init(void)
{
    // 对于STM32H743，SysTick使用核心时钟HCLK，无需分频
    fac_us = SystemCoreClock / 1000000;  // 每微秒的时钟周期数
    fac_ms = fac_us * 1000;              // 每毫秒的时钟周期数
}

// 延时nus微秒
void delay_us(uint32_t nus)
{
    uint32_t temp;
    uint32_t ticks;
    uint32_t reload = SysTick->LOAD;   // 备份原始的SysTick加载值

    ticks = nus * fac_us;              // 需要延时的总时钟周期数

    // 如果总周期数超过SysTick的最大重装值（24位），则分多次延时
    while (ticks > 0)
    {
        if (ticks > 0xFFFFFF)
        {
            SysTick->LOAD = 0xFFFFFF;    // SysTick的最大重装值（24位）
            ticks -= 0xFFFFFF;
        }
        else
        {
            SysTick->LOAD = ticks;
            ticks = 0;
        }
        SysTick->VAL = 0x00;             // 清空计数器
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  // 开始倒计时

        // 等待计数到零
        do
        {
            temp = SysTick->CTRL;
        } while ((temp & SysTick_CTRL_ENABLE_Msk) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));

        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 关闭计数器
    }

    SysTick->LOAD = reload;   // 恢复原始的SysTick加载值
}

// 延时nms毫秒
void delay_ms(uint32_t nms)
{
    uint32_t temp;
    uint32_t ticks;
    uint32_t reload = SysTick->LOAD;   // 备份原始的SysTick加载值

    ticks = nms * fac_ms;              // 需要延时的总时钟周期数

    // 如果总周期数超过SysTick的最大重装值（24位），则分多次延时
    while (ticks > 0)
    {
        if (ticks > 0xFFFFFF)
        {
            SysTick->LOAD = 0xFFFFFF;    // SysTick的最大重装值（24位）
            ticks -= 0xFFFFFF;
        }
        else
        {
            SysTick->LOAD = ticks;
            ticks = 0;
        }
        SysTick->VAL = 0x00;             // 清空计数器
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  // 开始倒计时

        // 等待计数到零
        do
        {
            temp = SysTick->CTRL;
        } while ((temp & SysTick_CTRL_ENABLE_Msk) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));

        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // 关闭计数器
    }

    SysTick->LOAD = reload;   // 恢复原始的SysTick加载值
}
