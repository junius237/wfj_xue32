#include "pid.h"
#include "control.h"

#define MAX_OUTPUT 500
#define MIN_OUTPUT -500

PID pid[4];

/**
 * @brief  PID������ʼ��
 *	@note 	��
 * @retval ��
 */
void PID_init(float p[], float i[], float d[])
{

  /* ��ʼ������ */
  for (int a = 0; a < 4; a++)
  {
    pid[a].target_val = 50; // Ŀ��ֵ
    pid[a].output_val = 0;  // ���ֵ
    pid[a].err = 0;         // ƫ��ֵ
    pid[a].err_last = 0;    // ��һ��ƫ��ֵ
    pid[a].err_prev = 0;    // ����һ��ƫ��ֵ
    pid[a].Kp = p[a];       // ����ϵ��
    pid[a].Ki = i[a];       // ����ϵ��
    pid[a].Kd = d[a];       // ΢��ϵ��
    pid[a].integral = 0;    // ����ֵ
  }
}

/**
 * @brief  PID�㷨ʵ��(����ʽ PID)
 * @param  index: PID����������
 * @param  actual_val: ��ǰʵ��ֵ
 * @note   ��
 * @retval ͨ��PID���������
 */

float PID_realize(int index)
{
  if (index < 0 || index >= 4)
    return HAL_ERROR; // ȷ��������Ч

  int actual_val;
  actual_val = Get_Speed(index); // ��ȡ��ǰʵ��ֵ

  pid[index].actual_val = actual_val;

  // ���㵱ǰ���
  pid[index].err = pid[index].target_val - actual_val;

  // ��������ʽ PID �Ŀ������� ��output
  float delta_output = pid[index].Kp * (pid[index].err - pid[index].err_last) +
                       pid[index].Ki * pid[index].err +
                       pid[index].Kd * (pid[index].err - 2 * pid[index].err_last + pid[index].err_prev);

  // �������ֵ
  pid[index].output_val += delta_output;

  //�������ֵ����ֹ����
  if (pid[index].output_val > MAX_OUTPUT)
    pid[index].output_val = MAX_OUTPUT;
  else if (pid[index].output_val < MIN_OUTPUT)
    pid[index].output_val = MIN_OUTPUT;

  // ���������ʷ
  pid[index].err_prev = pid[index].err_last;
  pid[index].err_last = pid[index].err;

  // ���ظ��º�����ֵ
  return pid[index].output_val;
}
