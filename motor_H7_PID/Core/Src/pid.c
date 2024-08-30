#include "pid.h"

PID pid;

/**
 * @brief  PID参数初始化
 *	@note 	无
 * @retval 无
 */
void PID_param_init()
{
  /* 初始化参数 */
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
 * @brief  设置目标值
 * @param  val		目标值
 *	@note 	无
 * @retval 无
 */
void setpid_target(float temp_val)
{
  pid.target_val = temp_val; // 设置当前的目标值
}

/**
 * @brief  获取目标值
 * @param  无
 *	@note 	无
 * @retval 目标值
 */
float getpid_target(void)
{
  return pid.target_val; // 设置当前的目标值
}

/**
 * @brief  设置比例、积分、微分系数
 * @param  p：比例系数 P
 * @param  i：积分系数 i
 * @param  d：微分系数 d
 *	@note 	无
 * @retval 无
 */
void set_p_i_d(float p, float i, float d)
{
  pid.Kp = p; // 设置比例系数 P
  pid.Ki = i; // 设置积分系数 I
  pid.Kd = d; // 设置微分系数 D
}

/**
 * @brief  PID算法实现
 * @param  actual_val:实际值
 *	@note 	无
 * @retval 通过PID计算后的输出
 */
float PID_realize(float actual_val)
{
  /*计算目标值与实际值的误差*/
  pid.err = pid.target_val - actual_val;
  /*误差累积*/
  pid.integral += pid.err;
  /*PID算法实现*/
  pid.actual_val = pid.Kp * pid.err + pid.Ki * pid.integral + pid.Kd * (pid.err - pid.err_last);
  /*误差传递*/
  pid.err_last = pid.err;
  /*返回当前实际值*/
  return pid.actual_val;
}
