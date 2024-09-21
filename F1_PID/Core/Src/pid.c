#include "pid.h"
#include "control.h"

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
    pid[a].target_val = 0; // Ŀ��ֵ
    pid[a].output_val = 0; // ���ֵ
    pid[a].err = 0;        // ƫ��ֵ
    pid[a].err_last = 0;   // ��һ��ƫ��ֵ
    pid[a].Kp = p[a];      // ����ϵ��
    pid[a].Ki = i[a];      // ����ϵ��
    pid[a].Kd = d[a];      // ΢��ϵ��
    pid[a].integral = 0;   // ����ֵ
  }
}

/**
 * @brief  ���ñ��������֡�΢��ϵ��
 * @param  p������ϵ�� P
 * @param  i������ϵ�� i
 * @param  d��΢��ϵ�� d
 *	@note 	��
 * @retval ��
 */
void set_p_i_d(int index, float p[], float i[], float d[])
{
  if (index >= 0 && index <= 3)
  {
    pid[index].Kp = p[index];
    pid[index].Ki = i[index];
    pid[index].Kd = d[index];
  }
}

/**
 * @brief  PID�㷨ʵ��
 * @param  index: PID����������
 * @param  actual_val: ��ǰʵ��ֵ
 * @note   ��
 * @retval ͨ��PID���������
 */
float PID_realize(int index)
{

  if (index < 0 || index >= 4)
    return 0; // ȷ��������Ч
  int actual_val;
  actual_val = Get_Speed(index); // ��ȡ��ǰʵ��ֵ

  pid[index].actual_val = actual_val; // ��ȡ��ǰʵ��ֵ
  /* ����Ŀ��ֵ��ʵ��ֵ����� */
  pid[index].err = pid[index].target_val - actual_val;
  /* ����ۻ� */
  pid[index].integral += pid[index].err;
  // ������������Ƹ�Ϊ�ԳƵ����޺�����
  if (pid[index].integral > 100)
  {
    pid[index].integral = 100;
  }
  if (pid[index].integral < -150)
  {
    pid[index].integral = -150;
  }
  /* PID�㷨ʵ�� */
  pid[index].output_val = pid[index].Kp * pid[index].err +
                          pid[index].Ki * pid[index].integral +
                          pid[index].Kd * (pid[index].err - pid[index].err_last);
  /* ���� */
  pid[index].err_last = pid[index].err;
  /* ���ؼ��������ֵ */
  return pid[index].output_val;
}
