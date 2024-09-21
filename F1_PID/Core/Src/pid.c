#include "pid.h"
#include "control.h"

PID pid[4];

/**
 * @brief  PID参数初始化
 *	@note 	无
 * @retval 无
 */
void PID_init(float p[], float i[], float d[])
{

  /* 初始化参数 */
  for (int a = 0; a < 4; a++)
  {
    pid[a].target_val = 0; // 目标值
    pid[a].output_val = 0; // 输出值
    pid[a].err = 0;        // 偏差值
    pid[a].err_last = 0;   // 上一个偏差值
    pid[a].Kp = p[a];      // 比例系数
    pid[a].Ki = i[a];      // 积分系数
    pid[a].Kd = d[a];      // 微分系数
    pid[a].integral = 0;   // 积分值
  }
}

/**
 * @brief  设置比例、积分、微分系数
 * @param  p：比例系数 P
 * @param  i：积分系数 i
 * @param  d：微分系数 d
 *	@note 	无
 * @retval 无
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
 * @brief  PID算法实现
 * @param  index: PID控制器索引
 * @param  actual_val: 当前实际值
 * @note   无
 * @retval 通过PID计算后的输出
 */
float PID_realize(int index)
{

  if (index < 0 || index >= 4)
    return 0; // 确保索引有效
  int actual_val;
  actual_val = Get_Speed(index); // 获取当前实际值

  pid[index].actual_val = actual_val; // 获取当前实际值
  /* 计算目标值与实际值的误差 */
  pid[index].err = pid[index].target_val - actual_val;
  /* 误差累积 */
  pid[index].integral += pid[index].err;
  // 将积分项的限制改为对称的上限和下限
  if (pid[index].integral > 100)
  {
    pid[index].integral = 100;
  }
  if (pid[index].integral < -150)
  {
    pid[index].integral = -150;
  }
  /* PID算法实现 */
  pid[index].output_val = pid[index].Kp * pid[index].err +
                          pid[index].Ki * pid[index].integral +
                          pid[index].Kd * (pid[index].err - pid[index].err_last);
  /* 误差传递 */
  pid[index].err_last = pid[index].err;
  /* 返回计算后的输出值 */
  return pid[index].output_val;
}
