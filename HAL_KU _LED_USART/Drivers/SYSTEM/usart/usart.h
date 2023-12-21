/**
 ****************************************************************************************************
 * @file        usart.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-20
 * @brief       串口初始化代码(一般是串口1)，支持printf
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211103
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "./SYSTEM/sys/sys.h"




extern UART_HandleTypeDef g_uart1_handle;       /* HAL UART句柄 */
extern uint8_t g_usart1_rx_flag;   /*串口接收到数据标志*/
extern uint8_t g_rx_buffer[1];  /*HAL使用串口接收数据缓冲区 */
    void USART1_IRQHandler(void);
void usart_init(uint32_t bound);                /* 串口初始化函数 */

#endif


