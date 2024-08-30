#include "pid.h"

PID pid;

/**
 * @brief  PID������ʼ��
 *	@note 	��
 * @retval ��
 */
void PID_param_init()
{
  /* ��ʼ������ */
  pid.target_val = 300.0;
  pid.actual_val = 0.0;
  pid.err = 0.0;
  pid.err_last = 0.0;
  pid.integral = 0.0;

  pid.Kp = 5.0;
  pid.Ki = 2.8;
  pid.Kd = 2.1;
}

/**
 * @brief  ����Ŀ��ֵ
 * @param  val		Ŀ��ֵ
 *	@note 	��
 * @retval ��
 */
void setpid_target(float temp_val)
{
  pid.target_val = temp_val; // ���õ�ǰ��Ŀ��ֵ
}

/**
 * @brief  ��ȡĿ��ֵ
 * @param  ��
 *	@note 	��
 * @retval Ŀ��ֵ
 */
float getpid_target(void)
{
  return pid.target_val; // ���õ�ǰ��Ŀ��ֵ
}

/**
 * @brief  ���ñ��������֡�΢��ϵ��
 * @param  p������ϵ�� P
 * @param  i������ϵ�� i
 * @param  d��΢��ϵ�� d
 *	@note 	��
 * @retval ��
 */
void set_p_i_d(float p, float i, float d)
{
  pid.Kp = p; // ���ñ���ϵ�� P
  pid.Ki = i; // ���û���ϵ�� I
  pid.Kd = d; // ����΢��ϵ�� D
}

/**
 * @brief  PID�㷨ʵ��
 * @param  actual_val:ʵ��ֵ
 *	@note 	��
 * @retval ͨ��PID���������
 */
float PID_realize(float actual_val)
{
  /*����Ŀ��ֵ��ʵ��ֵ�����*/
  pid.err = pid.target_val - actual_val;
  /*����ۻ�*/
  pid.integral += pid.err;
  /*PID�㷨ʵ��*/
  pid.actual_val = pid.Kp * pid.err + pid.Ki * pid.integral + pid.Kd * (pid.err - pid.err_last);
  /*����*/
  pid.err_last = pid.err;
  /*���ص�ǰʵ��ֵ*/
  return pid.actual_val;
}
