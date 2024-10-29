#include "hc.h"
#include "stdlib.h"
#include "pid.h"
#include "delay.h"
#include "i2cnew.h"
#include "imu.h"
#include "control.h"

/*
 * ���hc--usart2   PA2--TX  PA3--RX
 * �ұ�hc--usart3   PB10--TX  PB11--RX
 */

// ������ݻ�����������Ϊ DATA_LENGTH
uint8_t data_buffer_L[DATA_LENGTH] = {0};

// �����������
uint8_t data_index_L = 0;

// ������ֵ
uint16_t Distance_L = 0;

/* �Ҳ����ݻ�����������Ϊ DATA_LENGTH
    �Ҳമ�� USART3
    PB10--TX  PB11--RX */
uint8_t data_buffer_R[DATA_LENGTH] = {0};

// �Ҳ���������
uint8_t data_index_R = 0;

// �Ҳ����ֵ
uint16_t Distance_R = 0;

// ����״̬ö������
typedef enum
{
    WAIT_FOR_START, // �ȴ���ʼ
    RECEIVE_DATA    // ��������
} ReceiveState;

// ������״̬
ReceiveState receive_state1 = WAIT_FOR_START;

// �Ҳ����״̬
ReceiveState receive_state2 = WAIT_FOR_START;
/*************************************************************/
/* ������泣�� */
uint16_t R = 150; // �������

/*ת���Ƕ�*/

static float YAW_STEP = 180;

eulerian_angles_t e_angles;

/**
 * @brief �����벢����ƫ���Ǻ;���ִ����Ӧ�Ķ���-���������
 *
 * �ú������ݴ�������õ����Ҿ����ƫ����������С�����˶�����
 *
 * - ���ƫ������30��90��֮�䣬��������������Ҳ���������Χ����������ڸ�����룬����ת��
 * - ���ƫ������-90��-30��֮�䣬�����Ҳ������������������Χ���Ҳ������ڸ�����룬����ת��
 * - ���ƫ������-30��30��֮�䣬�������Ҿ��붼��145��YAW_STEP֮�䣬��ֱ�С�
 * - ���ƫ������-30��30��֮�䣬�������Ҿ��붼��100��140֮���Ҹ������С��160������ˡ�
 * - ������Ҿ��붼��200��6553֮�䣬���߶���0��80֮�䣬��ֹͣ��
 *
 * @note �ú����������ȫ�ֱ��� e_angles, Distance_L, Distance_R, R �Լ����� PID_realize, car_right, car_left, car_forward, car_backward, car_stop, delay_ms��
 */
void check_distances(void)
{
    const int TOLERANCE = -3; // ������Χ

        // �������������Ҳ���������Χ��������������ڸ������
        if (Distance_L > Distance_R + TOLERANCE && Distance_L > R)
        {
            // ��ת
            car_right(PID_realize(0));
            if (e_angles.yaw * YAW_STEP >= 30 && e_angles.yaw * YAW_STEP <= 90)
            {
                car_stop();
            }
        }
        // ����Ҳ������������������Χ�������Ҳ������ڸ������
        else if (Distance_R > Distance_L + TOLERANCE && Distance_R > R)
        {
            // ��ת
            car_left(PID_realize(0));
            if (e_angles.yaw * YAW_STEP >= -90 && e_angles.yaw * YAW_STEP <= -30)
            {
                car_stop();
            }
        }
        // ������Ҿ��붼��145��180֮��
        else if ((Distance_L > 145 && Distance_L < 180) && (Distance_R > 145 && Distance_R < 180))
        {
            // ������Ҿ��붼��100��140֮�䣬���Ҹ������С��160
            if ((Distance_L > 100 && Distance_L < 140) && (Distance_R > 100 && Distance_R < 140) && R < 160)
            {
                // ����
                car_backward(PID_realize(0), PID_realize(1), PID_realize(2), PID_realize(3));
            }
            // ֱ��
            car_forward(PID_realize(0), PID_realize(1), PID_realize(2), PID_realize(3));
        }
        // ������Ҿ��붼��200��6553֮�䣬���߶���0��80֮��
        else if (((Distance_L > 250 && Distance_L < 6553) && (Distance_R > 250 && Distance_R < 6553)) ||
                 ((Distance_L > 0 && Distance_L < 80) && (Distance_R > 0 && Distance_R < 80)))
        {
            // ֹͣ
            car_stop();
           // Encdoer_STOP();
            
        }



    // // ���ƫ������30��90��֮��
    // if (e_angles.yaw * YAW_STEP >= 30 && e_angles.yaw * YAW_STEP <= 90)
    // {
    //     // �������������Ҳ���������Χ��������������ڸ������
    //     if (Distance_L > Distance_R + TOLERANCE && Distance_L > R)
    //     {
    //         // ��ת
    //         car_right(PID_realize(0), PID_realize(1), PID_realize(2), PID_realize(3));
    //     }
    // }
    // // ���ƫ������-90��-30��֮��
    // else if (e_angles.yaw * YAW_STEP >= -90 && e_angles.yaw * YAW_STEP <= -30)
    // {
    //     // ����Ҳ������������������Χ�������Ҳ������ڸ������
    //     if (Distance_R > Distance_L + TOLERANCE && Distance_R > R)
    //     {
    //         // ��ת
    //         car_left(PID_realize(0), PID_realize(1), PID_realize(2), PID_realize(3));
    //     }
    // }
    // // ���ƫ������-30��30��֮��
    // else if ((e_angles.yaw * YAW_STEP >= 0 && e_angles.yaw * YAW_STEP <= 30) || (e_angles.yaw * YAW_STEP >= -30 && e_angles.yaw * YAW_STEP <= 0))
    // {
    //     // ������Ҿ��붼��145��180֮��
    //     if ((Distance_L > 145 && Distance_L < 180) && (Distance_R > 145 && Distance_R < 180))
    //     {
    //         // ������Ҿ��붼��100��140֮�䣬���Ҹ������С��160
    //         if ((Distance_L > 100 && Distance_L < 140) && (Distance_R > 100 && Distance_R < 140) && R < 160)
    //         {
    //             // ����
    //             car_backward(PID_realize(0), PID_realize(1), PID_realize(2), PID_realize(3));
    //         }
    //         // ֱ��
    //         car_forward(PID_realize(0), PID_realize(1), PID_realize(2), PID_realize(3));
    //     }
    // }

    // // ������Ҿ��붼��200��6553֮�䣬���߶���0��80֮��
    // if (((Distance_L > 200 && Distance_L < 6553) && (Distance_R > 200 && Distance_R < 6553)) ||
    //     ((Distance_L > 0 && Distance_L < 80) && (Distance_R > 0 && Distance_R < 80)))
    // {
    //     // ֹͣ
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
                    // ���� Distance_L ����
                   // printf("Distance_L: %d\r\n", Distance_L);
                    

                }
                else
                {
                    // У��ʧ�ܴ���
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
                    // ���� Distance_R ����
                  //  printf("Distance_R: %d\r\n", Distance_R);
                   
                }
                else
                {
                    // У��ʧ�ܴ���
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
