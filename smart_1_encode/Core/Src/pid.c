#include "pid.h"
#include "control.h"

#define MAX_OUTPUT 500
#define MIN_OUTPUT -500

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
    pid[a].target_val = 50; // 目标值
    pid[a].output_val = 0;  // 输出值
    pid[a].err = 0;         // 偏差值
    pid[a].err_last = 0;    // 上一个偏差值
    pid[a].err_prev = 0;    // 上上一个偏差值
    pid[a].Kp = p[a];       // 比例系数
    pid[a].Ki = i[a];       // 积分系数
    pid[a].Kd = d[a];       // 微分系数
    pid[a].integral = 0;    // 积分值
  }
}

/**
 * @brief  PID算法实现(增量式 PID)
 * @param  index: PID控制器索引
 * @param  actual_val: 当前实际值
 * @note   无
 * @retval 通过PID计算后的输出
 */

float PID_realize(int index)
{
  if (index < 0 || index >= 4)
    return HAL_ERROR; // 确保索引有效

  int actual_val;
  actual_val = Get_Speed(index); // 获取当前实际值

  pid[index].actual_val = actual_val;

  // 计算当前误差
  pid[index].err = pid[index].target_val - actual_val;

  // 计算增量式 PID 的控制增量 Δoutput
  float delta_output = pid[index].Kp * (pid[index].err - pid[index].err_last) +
                       pid[index].Ki * pid[index].err +
                       pid[index].Kd * (pid[index].err - 2 * pid[index].err_last + pid[index].err_prev);

  // 更新输出值
  pid[index].output_val += delta_output;

  //限制输出值，防止饱和
  if (pid[index].output_val > MAX_OUTPUT)
    pid[index].output_val = MAX_OUTPUT;
  else if (pid[index].output_val < MIN_OUTPUT)
    pid[index].output_val = MIN_OUTPUT;

  // 更新误差历史
  pid[index].err_prev = pid[index].err_last;
  pid[index].err_last = pid[index].err;

  // 返回更新后的输出值
  return pid[index].output_val;
}
