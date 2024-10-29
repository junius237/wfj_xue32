#include "hc.h"
#include "stdlib.h"
#include "pid.h"
#include "delay.h"
#include "i2cnew.h"
#include "imu.h"
#include "control.h"

/*
 * 左边hc--usart2   PA2--TX  PA3--RX
 * 右边hc--usart3   PB10--TX  PB11--RX
 */

// 左侧数据缓冲区，长度为 DATA_LENGTH
uint8_t data_buffer_L[DATA_LENGTH] = {0};

// 左侧数据索引
uint8_t data_index_L = 0;

// 左侧距离值
uint16_t Distance_L = 0;

/* 右侧数据缓冲区，长度为 DATA_LENGTH
    右侧串口 USART3
    PB10--TX  PB11--RX */
uint8_t data_buffer_R[DATA_LENGTH] = {0};

// 右侧数据索引
uint8_t data_index_R = 0;

// 右侧距离值
uint16_t Distance_R = 0;

// 接收状态枚举类型
typedef enum
{
    WAIT_FOR_START, // 等待开始
    RECEIVE_DATA    // 接收数据
} ReceiveState;

// 左侧接收状态
ReceiveState receive_state1 = WAIT_FOR_START;

// 右侧接收状态
ReceiveState receive_state2 = WAIT_FOR_START;
/*************************************************************/
/* 定义跟随常量 */
uint16_t R = 150; // 跟随距离

/*转化角度*/

static float YAW_STEP = 180;

eulerian_angles_t e_angles;

/**
 * @brief 检查距离并根据偏航角和距离执行相应的动作-跟随解析。
 *
 * 该函数根据传感器测得的左右距离和偏航角来决定小车的运动方向。
 *
 * - 如果偏航角在30到90度之间，并且左侧距离大于右侧距离加上误差范围且左侧距离大于跟随距离，则右转。
 * - 如果偏航角在-90到-30度之间，并且右侧距离大于左侧距离加上误差范围且右侧距离大于跟随距离，则左转。
 * - 如果偏航角在-30到30度之间，并且左右距离都在145到YAW_STEP之间，则直行。
 * - 如果偏航角在-30到30度之间，并且左右距离都在100到140之间且跟随距离小于160，则后退。
 * - 如果左右距离都在200到6553之间，或者都在0到80之间，则停止。
 *
 * @note 该函数假设存在全局变量 e_angles, Distance_L, Distance_R, R 以及函数 PID_realize, car_right, car_left, car_forward, car_backward, car_stop, delay_ms。
 */
void check_distances(void)
{
    const int TOLERANCE = -3; // 设置误差范围

        // 如果左侧距离大于右侧距离加上误差范围，并且左侧距离大于跟随距离
        if (Distance_L > Distance_R + TOLERANCE && Distance_L > R)
        {
            // 右转
            car_right(PID_realize(0));
            if (e_angles.yaw * YAW_STEP >= 30 && e_angles.yaw * YAW_STEP <= 90)
            {
                car_stop();
            }
        }
        // 如果右侧距离大于左侧距离加上误差范围，并且右侧距离大于跟随距离
        else if (Distance_R > Distance_L + TOLERANCE && Distance_R > R)
        {
            // 左转
            car_left(PID_realize(0));
            if (e_angles.yaw * YAW_STEP >= -90 && e_angles.yaw * YAW_STEP <= -30)
            {
                car_stop();
            }
        }
        // 如果左右距离都在145到180之间
        else if ((Distance_L > 145 && Distance_L < 180) && (Distance_R > 145 && Distance_R < 180))
        {
            // 如果左右距离都在100到140之间，并且跟随距离小于160
            if ((Distance_L > 100 && Distance_L < 140) && (Distance_R > 100 && Distance_R < 140) && R < 160)
            {
                // 后退
                car_backward(PID_realize(0), PID_realize(1), PID_realize(2), PID_realize(3));
            }
            // 直行
            car_forward(PID_realize(0), PID_realize(1), PID_realize(2), PID_realize(3));
        }
        // 如果左右距离都在200到6553之间，或者都在0到80之间
        else if (((Distance_L > 250 && Distance_L < 6553) && (Distance_R > 250 && Distance_R < 6553)) ||
                 ((Distance_L > 0 && Distance_L < 80) && (Distance_R > 0 && Distance_R < 80)))
        {
            // 停止
            car_stop();
           // Encdoer_STOP();
            
        }



    // // 如果偏航角在30到90度之间
    // if (e_angles.yaw * YAW_STEP >= 30 && e_angles.yaw * YAW_STEP <= 90)
    // {
    //     // 如果左侧距离大于右侧距离加上误差范围，并且左侧距离大于跟随距离
    //     if (Distance_L > Distance_R + TOLERANCE && Distance_L > R)
    //     {
    //         // 右转
    //         car_right(PID_realize(0), PID_realize(1), PID_realize(2), PID_realize(3));
    //     }
    // }
    // // 如果偏航角在-90到-30度之间
    // else if (e_angles.yaw * YAW_STEP >= -90 && e_angles.yaw * YAW_STEP <= -30)
    // {
    //     // 如果右侧距离大于左侧距离加上误差范围，并且右侧距离大于跟随距离
    //     if (Distance_R > Distance_L + TOLERANCE && Distance_R > R)
    //     {
    //         // 左转
    //         car_left(PID_realize(0), PID_realize(1), PID_realize(2), PID_realize(3));
    //     }
    // }
    // // 如果偏航角在-30到30度之间
    // else if ((e_angles.yaw * YAW_STEP >= 0 && e_angles.yaw * YAW_STEP <= 30) || (e_angles.yaw * YAW_STEP >= -30 && e_angles.yaw * YAW_STEP <= 0))
    // {
    //     // 如果左右距离都在145到180之间
    //     if ((Distance_L > 145 && Distance_L < 180) && (Distance_R > 145 && Distance_R < 180))
    //     {
    //         // 如果左右距离都在100到140之间，并且跟随距离小于160
    //         if ((Distance_L > 100 && Distance_L < 140) && (Distance_R > 100 && Distance_R < 140) && R < 160)
    //         {
    //             // 后退
    //             car_backward(PID_realize(0), PID_realize(1), PID_realize(2), PID_realize(3));
    //         }
    //         // 直行
    //         car_forward(PID_realize(0), PID_realize(1), PID_realize(2), PID_realize(3));
    //     }
    // }

    // // 如果左右距离都在200到6553之间，或者都在0到80之间
    // if (((Distance_L > 200 && Distance_L < 6553) && (Distance_R > 200 && Distance_R < 6553)) ||
    //     ((Distance_L > 0 && Distance_L < 80) && (Distance_R > 0 && Distance_R < 80)))
    // {
    //     // 停止
    //     car_stop();
    // }
}

/************************************************************/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        uint8_t received_byte = data_buffer_L[data_index_L];

        switch (receive_state1)
        {
        case WAIT_FOR_START:
            if (received_byte == START_BYTE)
            {
                data_index_L = 1;
                receive_state1 = RECEIVE_DATA;
            }
            else
            {
                data_index_L = 0;
            }
            break;

        case RECEIVE_DATA:
            data_index_L++;

            if (data_index_L >= DATA_LENGTH)
            {
                uint8_t checksum = data_buffer_L[0] ^ data_buffer_L[1] ^ data_buffer_L[2];

                if (data_buffer_L[3] == checksum)
                {
                    Distance_L = ((data_buffer_L[1] << 8) | data_buffer_L[2]) / 10;
                    // 处理 Distance_L 数据
                   // printf("Distance_L: %d\r\n", Distance_L);
                    

                }
                else
                {
                    // 校验失败处理
                    printf("Checksum error\r\n");
                }

                data_index_L = 0;
                receive_state1 = WAIT_FOR_START;
            }
            break;

        default:
            receive_state1 = WAIT_FOR_START;
            data_index_L = 0;
            break;
        }

        HAL_UART_Receive_IT(&huart2, &data_buffer_L[data_index_L], 1);
    }
    else if (huart->Instance == USART3)
    {
        uint8_t received_byte = data_buffer_R[data_index_R];

        switch (receive_state2)
        {
        case WAIT_FOR_START:
            if (received_byte == START_BYTE)
            {
                data_index_R = 1;
                receive_state2 = RECEIVE_DATA;
            }
            else
            {
                data_index_R = 0;
            }
            break;

        case RECEIVE_DATA:
            data_index_R++;

            if (data_index_R >= DATA_LENGTH)
            {
                uint8_t checksum = data_buffer_R[0] ^ data_buffer_R[1] ^ data_buffer_R[2];

                if (data_buffer_R[3] == checksum)
                {
                    Distance_R = ((data_buffer_R[1] << 8) | data_buffer_R[2]) / 10;
                    // 处理 Distance_R 数据
                  //  printf("Distance_R: %d\r\n", Distance_R);
                   
                }
                else
                {
                    // 校验失败处理
                    printf("Checksum error\r\n");
                }

                data_index_R = 0;
                receive_state2 = WAIT_FOR_START;
            }
            break;

        default:
            receive_state2 = WAIT_FOR_START;
            data_index_R = 0;
            break;
        }

        HAL_UART_Receive_IT(&huart3, &data_buffer_R[data_index_R], 1);
    }
}
