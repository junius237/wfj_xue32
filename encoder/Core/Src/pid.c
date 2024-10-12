#include "pid.h"
#include "control.h"

#define MAX_OUTPUT 500
#define MIN_OUTPUT -500
#define MIN_TARGET -100 // ���Ŀ��ֵ��Χ
#define MAX_TARGET 100
#define PWM_MAX_VALUE 1000 // PWM ������ֵ
#define PID_MAX_OUTPUT 500 // PID ���������ֵ
#define SCALE_FACTOR ((float)PWM_MAX_VALUE / (float)PID_MAX_OUTPUT)

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
    pid[a].output_val = 0;  // ���ֵ
    pid[a].actual_val = 0;  // ʵ��ֵ
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
 * @brief ���õ����Ŀ��ֵ
 *
 * @param motor_index �����������ΧΪ0��3
 * @param target_val Ŀ��ֵ����ΧΪMIN_TARGET��MAX_TARGET
 * @return int ���������Ŀ��ֵ��Ч������HAL_ERROR�����򷵻����õ�Ŀ��ֵ
 */
int Set_target_val(int motor_index, int target_val)
{
  if (motor_index < 0 || motor_index > 3)
    return HAL_ERROR;
  if (target_val < MIN_TARGET || target_val > MAX_TARGET)
    return HAL_ERROR;
  pid[motor_index].target_val = target_val;
  return target_val;
}

/**
 * @brief  PID�㷨ʵ��(����ʽ PID)
 * @param  index: PID����������
 * @param  actual_val: ��ǰʵ��ֵ
 * @note   ��
 * @retval ͨ��PID���������
 */

int PID_realize(int index)
{
  if (index < 0 || index >= 4)
    return HAL_ERROR; // ȷ��������Ч

  int pwm_output[index];
  int actual_val[index];
  actual_val[index] = Get_Speed(index); // ��ȡ��ǰʵ��ֵ
  pid[index].actual_val = actual_val[index];

  // ���㵱ǰ���
  pid[index].err = pid[index].target_val - actual_val[index];

  // ��������ʽ PID �Ŀ������� ��output
  float delta_output = pid[index].Kp * (pid[index].err - pid[index].err_last) +
                       pid[index].Ki * pid[index].err +
                       pid[index].Kd * (pid[index].err - 2 * pid[index].err_last + pid[index].err_prev);

  // �������ֵ
  pid[index].output_val += delta_output;

  // �������ֵ����ֹ����
  if (pid[index].output_val > MAX_OUTPUT)
    pid[index].output_val = MAX_OUTPUT;
  else if (pid[index].output_val < MIN_OUTPUT)
    pid[index].output_val = MIN_OUTPUT;

  // �����ֵת��Ϊ PWM ���
  pwm_output[index] = (int)(pid[index].output_val * SCALE_FACTOR);
  
  if (pwm_output[index] > PWM_MAX_VALUE)
    pwm_output[index] = PWM_MAX_VALUE;
  else if (pwm_output[index] < 0)
    pwm_output[index] = 0;
  // �������
  pid[index].err_prev = pid[index].err_last;
  pid[index].err_last = pid[index].err;

  // ���ظ��º�����ֵ
  return pwm_output[index];
}
