#ifndef __IIC_H
#define __IIC_H

#include "./SYSTEM/sys/sys.h"
/* 引脚 定义 */
#define IIC_SCL_GPIO_PORT GPIOB
#define IIC_SCL_GPIO_PIN GPIO_PIN_6
#define IIC_SCL_GPIO_CLK_ENABLE()     \
    do                                \
    {                                 \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while (0)
#define IIC_SDA_GPIO_PORT GPIOB
#define IIC_SDA_GPIO_PIN GPIO_PIN_7
#define IIC_SDA_GPIO_CLK_ENABLE()     \
    do                                \
    {                                 \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while (0)
/* IO 操作 */
#define IIC_SCL(x)                                                                                                                                         \
    do                                                                                                                                                     \
    {                                                                                                                                                      \
        x ? HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(IIC_SCL_GPIO_PORT, IIC_SCL_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /* SCL */
#define IIC_SDA(x)                                                                                                                                         \
    do                                                                                                                                                     \
    {                                                                                                                                                      \
        x ? HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0) /* SDA */
/* 读取 SDA */
#define IIC_READ_SDA HAL_GPIO_ReadPin(IIC_SDA_GPIO_PORT, IIC_SDA_GPIO_PIN)


void iic_nack(void);
void iic_ack(void);
uint8_t iic_wait_ack(void);
uint8_t iic_read_byte(uint8_t ack);
void iic_stop(void);
void iic_start(void);
void iic_start(void);
static void iic_delay(void);
void iic_init(void);
#endif
