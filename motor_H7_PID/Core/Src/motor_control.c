#include "motor_control.h"
#include "tim.h"
#include "pid.h"
#include "stm32H7xx_it.h"

static motor_dir motor_fangxian = MOTOR_FWD; // ��¼����
static uint16_t duty = 0;                    // ��¼ռ�ձ�
// static uint8_t motor_en = 0;                 // ���ʹ��

/**
 * @brief  ���õ���ٶ�
 * @param  v: �ٶȣ�ռ�ձȣ�
 * @retval ��
 */
void set_motor_speed(uint16_t v)
{
  v = (v > 3600) ? 3600 : v; // ���޴���

  duty = v;

  if (motor_fangxian == MOTOR_FWD)
  {
    SET_FWD_COMPAER(duty); // �����ٶ�
  }
  else
  {
    SET_REV_COMPAER(duty); // �����ٶ�
  }
}

/**
 * @brief  ���õ������
 * @param  ��
 * @retval ��
 */
void set_motor_fangxiang(motor_dir dir)
{
  motor_fangxian = dir;

  if (motor_fangxian == MOTOR_FWD)
  {
    SET_FWD_COMPAER(duty); // �����ٶ�
    SET_REV_COMPAER(0);    // �����ٶ�
  }
  else
  {
    SET_FWD_COMPAER(0);    // �����ٶ�
    SET_REV_COMPAER(duty); // �����ٶ�
  }
}

/**
 * @brief  ���λ��ʽ PID ����ʵ��(��ʱ����)
 * @param  ��
 * @retval ��
 */
void motor_pid_control(void)
{
  if (1) // �����ʹ��״̬�²Ž��п��ƴ���
  {
    float cont_val = 0;                    // ��ǰ����ֵ
    static __IO int32_t Capture_Count = 0; // ��ǰʱ���ܼ���ֵ
    static __IO int32_t Last_Count = 0;    // ��һʱ���ܼ���ֵ
    int32_t actual_speed = 0;              // ʵ�ʲ���ٶ�

    /* ��ǰʱ���ܼ���ֵ = ������ֵ + ����������� * ENCODER_TIM_PERIOD  */
    Capture_Count = __HAL_TIM_GET_COUNTER(&htim2) + (Encoder_Overflow_Count * 65535);

    /* ת��ת�� = ��λʱ���ڵļ���ֵ / �������ֱܷ��� * ʱ��ϵ��  */
    // 30-���ٱ�
    actual_speed = ((float)(Capture_Count - Last_Count) / (11 * 4) / 30) / (GET_BASIC_TIM_PERIOD() / 1000.0 / 60.0);

    /* ��¼��ǰ�ܼ���ֵ������һʱ�̼���ʹ�� */
    Last_Count = Capture_Count;

    cont_val = PID_realize(actual_speed); // ���� PID ����

    if (cont_val > 0) // �жϵ������
    {
      set_motor_fangxiang(MOTOR_FWD);
    }
    else
    {
      cont_val = -cont_val;
      set_motor_fangxiang(MOTOR_REV);
    }

    cont_val = (cont_val > 3600) ? 3600 : cont_val; // �ٶ����޴���

    set_motor_speed(cont_val); // ���� PWM ռ�ձ�
  }
}
