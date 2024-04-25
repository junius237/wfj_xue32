#include "./BSP/ADC/adc.h"
#include "stm32f1xx_hal.h"
uint8_t BuzzerState = 0;
uint16_t BatteryVoltage;



                
DMA_HandleTypeDef g_dma_adc_handle; /*定义搬运ADC数据的DMA句柄*/
ADC_HandleTypeDef g_adc_dam_handle; /*定义ADC句柄*/

/**
 * @brief       ADC初始化函数
 *   @note      本函数支持ADC1/ADC2任意通道, 但是不支持ADC3
 *              我们使用12位精度, ADC采样时钟=12M, 转换时间为: 采样周期 + 12.5个ADC周期
 *              设置最大采样周期: 239.5, 则转换时间 = 252 个ADC周期 = 21us
 * @param       无
 * @retval      无
 */
void adc_init(void)
{
    g_adc_dam_handle.Instance = ADC1;                            /* 选择哪个ADC */
    g_adc_dam_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;       /* 数据对齐方式：右对齐 */
    g_adc_dam_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;       /* 非扫描模式，仅用到一个通道 */
    g_adc_dam_handle.Init.ContinuousConvMode = DISABLE;          /* 关闭连续转换模式 */
    g_adc_dam_handle.Init.NbrOfConversion = 1;                   /* 赋值范围是1~16，本实验用到1个规则通道序列 */
    g_adc_dam_handle.Init.DiscontinuousConvMode = DISABLE;       /* 禁止规则通道组间断模式 */
    g_adc_dam_handle.Init.NbrOfDiscConversion = 0;               /* 配置间断模式的规则通道个数，禁止规则通道组间断模式后，此参数忽略 */
    g_adc_dam_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START; /* 触发转换方式：软件触发 */
    HAL_ADC_Init(&g_adc_dam_handle);                             /* 初始化 */

    HAL_ADCEx_Calibration_Start(&g_adc_dam_handle); /* 校准ADC */
}

/**
 * @brief       ADC底层驱动，引脚配置，时钟使能
                此函数会被HAL_ADC_Init()调用
 * @param       hadc:ADC句柄
 * @retval      无
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        GPIO_InitTypeDef gpio_init_structure;
        RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
        __HAL_RCC_ADC1_CLK_ENABLE(); /*ADC1时钟使能*/
        __HAL_RCC_GPIOA_CLK_ENABLE();

        /* 设置ADC时钟 */
        adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC; /* ADC外设时钟 */
        adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;    /* 分频因子6时钟为72M/6=12MHz */
        HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);              /* 设置ADC时钟 */

        /* 设置AD采集通道对应IO引脚工作模式 */
        gpio_init_structure.Pin = GPIO_PIN_1;        /* ADC通道IO引脚 */
        gpio_init_structure.Mode = GPIO_MODE_ANALOG; /* 模拟 */
        HAL_GPIO_Init(GPIOA, &gpio_init_structure);
    }
}

/*adc-dam读取函数  mar参数*/
void adc_dma_init(uint32_t mar)
{

    GPIO_InitTypeDef gpio_init_structure;
    RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
    ADC_ChannelConfTypeDef adc_ch_conf = {0};
    __HAL_RCC_ADC1_CLK_ENABLE(); /*ADC1时钟使能*/
    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio_init_structure.Pin = GPIO_PIN_1;        /*ADC对应的通道*/
    gpio_init_structure.Mode = GPIO_MODE_ANALOG; /*模拟*/
    HAL_GPIO_Init(GPIOA, &gpio_init_structure);

    adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC; /*设置ADC外设时钟*/
    adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;    /*分频因子===72M/6=12Mhz*/
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);

    g_dma_adc_handle.Instance = DMA1_Channel1;
    g_dma_adc_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;              /*内存到外设*/
    g_dma_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;                  /*非增量*/
    g_dma_adc_handle.Init.MemInc = DMA_MINC_ENABLE;                      /*内存增量模式*/
    g_dma_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; /*外设数据长度16位*/
    g_dma_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    /*内存数据16位*/
    g_dma_adc_handle.Init.Mode = DMA_NORMAL;                             /*不循环*/
    g_dma_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                /*优先级中*/
    HAL_DMA_Init(&g_dma_adc_handle);

    __HAL_LINKDMA(&g_adc_dam_handle, DMA_Handle, g_dma_adc_handle);

    g_adc_dam_handle.Instance = ADC1;
    g_adc_dam_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;       /*右对齐*/
    g_adc_dam_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;       /*非扫描模式*/
    g_adc_dam_handle.Init.ContinuousConvMode = DISABLE;          /*单次转换*/
    g_adc_dam_handle.Init.NbrOfConversion = 1;                   /*顺序进行规则转换的ADC通道的数目*/
    g_adc_dam_handle.Init.DiscontinuousConvMode = DISABLE;       /*禁止规则通道组间断模式*/
    g_adc_dam_handle.Init.NbrOfDiscConversion = 0;               /*配置间断模式的规则通道个数，禁止规则通道组间断模式后，此参数忽略*/
    g_adc_dam_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START; /*由软件触发*/

    HAL_ADC_Init(&g_adc_dam_handle);

    HAL_ADCEx_Calibration_Start(&g_adc_dam_handle); /*校准ADC*/

    /*ADC配置*/
    adc_ch_conf.Channel = ADC_CHANNEL_1;                   /*通道*/
    adc_ch_conf.Rank = ADC_REGULAR_RANK_1;                 /*序列*/
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5; /*采样时间，设置最大采样时间239.5*/
    HAL_ADC_ConfigChannel(&g_adc_dam_handle, &adc_ch_conf);

    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 3, 3); /*配置dma数据流中断优先级*/
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

    HAL_DMA_Start_IT(&g_dma_adc_handle, (uint32_t)&ADC1->DR, mar, 0); /*启动DMA，开启中断*/
    HAL_ADC_Start_DMA(&g_adc_dam_handle, &mar, 0);                    /*开启ADC，通过DMA传输结果*/
}

/**
 * @brief       设置ADC通道采样时间
 * @param       adcx : adc句柄指针,ADC_HandleTypeDef
 * @param       ch   : 通道号, ADC_CHANNEL_0~ADC_CHANNEL_17
 * @param       stime: 采样时间  0~7, 对应关系为:
 *   @arg       ADC_SAMPLETIME_1CYCLE_5, 1.5个ADC时钟周期        ADC_SAMPLETIME_7CYCLES_5, 7.5个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_13CYCLES_5, 13.5个ADC时钟周期     ADC_SAMPLETIME_28CYCLES_5, 28.5个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_41CYCLES_5, 41.5个ADC时钟周期     ADC_SAMPLETIME_55CYCLES_5, 55.5个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_71CYCLES_5, 71.5个ADC时钟周期     ADC_SAMPLETIME_239CYCLES_5, 239.5个ADC时钟周期
 * @param       rank: 多通道采集时需要设置的采集编号,
                假设你定义channle1的rank=1，channle2 的rank=2，
                那么对应你在DMA缓存空间的变量数组AdcDMA[0] 就i是channle1的转换结果，AdcDMA[1]就是通道2的转换结果。
                单通道DMA设置为 ADC_REGULAR_RANK_1
 *   @arg       编号1~16：ADC_REGULAR_RANK_1~ADC_REGULAR_RANK_16
 * @retval      无
 */
void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch, uint32_t rank, uint32_t stime)
{
    ADC_ChannelConfTypeDef adc_ch_conf;

    adc_ch_conf.Channel = ch;                        /* 通道 */
    adc_ch_conf.Rank = rank;                         /* 序列 */
    adc_ch_conf.SamplingTime = stime;                /* 采样时间 */
    HAL_ADC_ConfigChannel(adc_handle, &adc_ch_conf); /* 通道配置 */
}

/**
 * @brief       获得ADC转换后的结果
 * @param       ch: 通道值 0~17，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_17
 * @retval      无
 */
uint32_t adc_get_result(uint32_t ch)
{
    adc_channel_set(&g_adc_dam_handle, ch, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_239CYCLES_5); /* 设置通道，序列和采样时间 */

    HAL_ADC_Start(&g_adc_dam_handle);                     /* 开启ADC */
    HAL_ADC_PollForConversion(&g_adc_dam_handle, 10);     /* 轮询转换 */
    return (uint16_t)HAL_ADC_GetValue(&g_adc_dam_handle); /* 返回最近一次ADC1规则组的转换结果 */
}

/********电压检测************/
void Check_Battery_Voltage(void)
{
    uint8_t i;
    uint32_t v = 0;
    for (i = 0; i < 8; i++)
    {
        v += adc_get_result(ADC_BAT);
    }
    v >>= 3;

    v = v * 2475 / 1024; // adc / 4096 * 3300 * 3(3代表放大3倍，因为采集电压时电阻分压了)
    BatteryVoltage = v;
}

uint16_t Get_Battery_Voltage(void)
{ // 电压毫伏
    return BatteryVoltage;
}
/*******************************************************************************************
蜂鸣器
void Buzzer(void)
{ // 放到100us的定时中断里面
    static bool fBuzzer = 0;
    static uint32 t1 = 0;
    static uint32 t2 = 0;
    if (fBuzzer)
    {
        if (++t1 >= 2)
        {
            t1 = 0;
            BUZZER = !BUZZER; // 2.5KHz
        }
    }
    else
    {
        BUZZER = 0;
    }

    if (BuzzerState == 0)
    {
        fBuzzer = 0;
        t2 = 0;
    }
    else if (BuzzerState == 1)
    {
        t2++;
        if (t2 < 5000)
        {
            fBuzzer = 1;
        }
        else if (t2 < 10000)
        {
            fBuzzer = 0;
        }
        else
        {
            t2 = 0;
        }
    }
}

*/
