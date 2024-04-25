#ifndef __IIC_H
#define __IIC_H

//*****软件模拟IIC，
//*****修改宏定义即可
//*****不同芯片时注意时钟和延时函数
#define    IIC_IO_SDA      GPIO_Pin_12  //SDA的IO口
#define    IIC_IO_SCL      GPIO_Pin_1  //SCL的IO口
#define    GPIOX           GPIOB       //GPIOx选择
#define    CLOCK		   RCC_APB2Periph_GPIOB //时钟信号
 
#define    IIC_SCL         PBout(1) //SCL
#define    IIC_SDA         PBout(12) //输出SDA
#define    READ_SDA        PBin(12)  //输入SDA


void I2C_SDA_OUT(void);
void I2C_SDA_IN(void);
void IIC_init(void);
void IIC_start(void);
void IIC_stop(void);
void IIC_ack(void);
void IIC_noack(void);
u8 IIC_wait_ack(void);
void IIC_send_byte(u8 txd);
u8 IIC_read_byte(u8 ack);
#endif
