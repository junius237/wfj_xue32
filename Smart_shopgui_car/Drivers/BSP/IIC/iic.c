#include "./BSP/IIC/iic.h"
#include "./SYSTEM/delay/delay.h"


void iic_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    IIC_SCL_GPIO_CLK_ENABLE(); /* SCL 引脚时钟使能 */
    IIC_SDA_GPIO_CLK_ENABLE(); /* SDA 引脚时钟使能 */
    gpio_init_struct.Pin = IIC_SCL_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;         /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                 /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;       /* 高速 */
    HAL_GPIO_Init(IIC_SCL_GPIO_PORT, &gpio_init_struct); /* SCL */
    /* SDA 引脚模式设置,开漏输出,上拉, 这样就不用再设置 IO 方向了, 开漏输出的时候(=1),
    也可以读取外部信号的高低电平 */
    gpio_init_struct.Pin = IIC_SDA_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;         /* 开漏输出 */
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &gpio_init_struct); /* SDA */
    iic_stop();                                          /* 停止总线上所有设备 */
}

/**
 * @brief IIC 延时函数,用于控制 IIC 读写速度
 * @param 无
 * @retval 无
 */
static void iic_delay(void)
{
    delay_us(2); /* 2us 的延时, 读写速度在 250Khz 以内 */
}
/**
 * @brief 产生 IIC 起始信号
 * @param 无
 * @retval 无
 */
void iic_start(void)
{
    IIC_SDA(1);
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(0); /* START 信号: 当 SCL 为高时, SDA 从高变成低, 表示起始信号 */
    iic_delay();
    IIC_SCL(0); /* 钳住 I2C 总线，准备发送或接收数据 */
    iic_delay();
}
/**
 * @brief 产生 IIC 停止信号
 * @param 无
 * @retval 无
 */
void iic_stop(void)
{
    IIC_SDA(0); /* STOP 信号: 当 SCL 为高时, SDA 从低变成高, 表示停止信号 */
    iic_delay();
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(1); /* 发送 I2C 总线结束信号 */
    iic_delay();
}
/*
* @brief IIC 读取一个字节
* @param ack: ack=1 时，发送 ack; ack=0 时，发送 nack
* @retval 接收到的数据
*/
uint8_t iic_read_byte(uint8_t ack)
{
    uint8_t i, receive = 0;
    for (i = 0; i < 8; i++) /* 接收 1 个字节数据 */
    {
        receive <<= 1; /* 高位先输出,所以先收到的数据位要左移 */
        IIC_SCL(1);
        iic_delay();
        if (IIC_READ_SDA)
        {
            receive++;
        }
        IIC_SCL(0);
        iic_delay();
    }
    if (!ack)
    {
        iic_nack(); /* 发送 nACK */
    }
    else
    {
        iic_ack(); /* 发送 ACK */
    }
    return receive;
}

/**
 * @brief 等待应答信号到来
 * @param 无
 * @retval 1，接收应答失败
 * 0，接收应答成功
 */
uint8_t iic_wait_ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;
    IIC_SDA(1); /* 主机释放 SDA 线(此时外部器件可以拉低 SDA 线) */
    iic_delay();
    IIC_SCL(1); /* SCL=1, 此时从机可以返回 ACK */
    iic_delay();
    while (IIC_READ_SDA) /* 等待应答 */
    {
        waittime++;
        if (waittime > 250)
        {
            iic_stop();
            rack = 1;
            break;
        }
    }
    IIC_SCL(0); /* SCL=0, 结束 ACK 检查 */
    iic_delay();
    return rack;
}
/**
 * @brief 产生 ACK 应答
 * @param 无
 * @retval 无
 */
void iic_ack(void)
{
    IIC_SDA(0); /* SCL 0 -> 1 时 SDA = 0,表示应答 */
    iic_delay();
    IIC_SCL(1); /* 产生一个时钟 */
    iic_delay();
    IIC_SCL(0);
    iic_delay();
    IIC_SDA(1); /* 主机释放 SDA 线 */
    iic_delay();
}
/**
 * @brief 不产生 ACK 应答
 * @param 无
 * @retval 无
 */
void iic_nack(void)
{
    IIC_SDA(1); /* SCL 0 -> 1 时 SDA = 1,表示不应答 */
    iic_delay();
    IIC_SCL(1); /* 产生一个时钟 */
    iic_delay();
    IIC_SCL(0);
    iic_delay();
}


