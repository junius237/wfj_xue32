#include "delay.h"
#include "stm32h7xx.h"

static uint32_t fac_us = 0;    // us��ʱ������
static uint32_t fac_ms = 0;    // ms��ʱ������

void delay_init(void)
{
    // ����STM32H743��SysTickʹ�ú���ʱ��HCLK�������Ƶ
    fac_us = SystemCoreClock / 1000000;  // ÿ΢���ʱ��������
    fac_ms = fac_us * 1000;              // ÿ�����ʱ��������
}

// ��ʱnus΢��
void delay_us(uint32_t nus)
{
    uint32_t temp;
    uint32_t ticks;
    uint32_t reload = SysTick->LOAD;   // ����ԭʼ��SysTick����ֵ

    ticks = nus * fac_us;              // ��Ҫ��ʱ����ʱ��������

    // ���������������SysTick�������װֵ��24λ������ֶ����ʱ
    while (ticks > 0)
    {
        if (ticks > 0xFFFFFF)
        {
            SysTick->LOAD = 0xFFFFFF;    // SysTick�������װֵ��24λ��
            ticks -= 0xFFFFFF;
        }
        else
        {
            SysTick->LOAD = ticks;
            ticks = 0;
        }
        SysTick->VAL = 0x00;             // ��ռ�����
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  // ��ʼ����ʱ

        // �ȴ���������
        do
        {
            temp = SysTick->CTRL;
        } while ((temp & SysTick_CTRL_ENABLE_Msk) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));

        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // �رռ�����
    }

    SysTick->LOAD = reload;   // �ָ�ԭʼ��SysTick����ֵ
}

// ��ʱnms����
void delay_ms(uint32_t nms)
{
    uint32_t temp;
    uint32_t ticks;
    uint32_t reload = SysTick->LOAD;   // ����ԭʼ��SysTick����ֵ

    ticks = nms * fac_ms;              // ��Ҫ��ʱ����ʱ��������

    // ���������������SysTick�������װֵ��24λ������ֶ����ʱ
    while (ticks > 0)
    {
        if (ticks > 0xFFFFFF)
        {
            SysTick->LOAD = 0xFFFFFF;    // SysTick�������װֵ��24λ��
            ticks -= 0xFFFFFF;
        }
        else
        {
            SysTick->LOAD = ticks;
            ticks = 0;
        }
        SysTick->VAL = 0x00;             // ��ռ�����
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  // ��ʼ����ʱ

        // �ȴ���������
        do
        {
            temp = SysTick->CTRL;
        } while ((temp & SysTick_CTRL_ENABLE_Msk) && !(temp & SysTick_CTRL_COUNTFLAG_Msk));

        SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // �رռ�����
    }

    SysTick->LOAD = reload;   // �ָ�ԭʼ��SysTick����ֵ
}
