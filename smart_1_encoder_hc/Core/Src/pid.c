#include "pid.h"
#include "control.h"

#define MAX_OUTPUT 500  //积分输出限幅
#define MIN_OUTPUT -500
#define MIN_TARGET -100 // 电机目标值范围
#define MAX_TARGET 100
#define PWM_MAX_VALUE 1000 // PWM 输出最大值
#define PID_MAX_OUTPUT 500 // PID 输出最大绝对值
#define SCALE_FACTOR ((float)PWM_MAX_VALUE / (float)PID_MAX_OUTPUT) // 比列系数

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
    pid[a].output_val = 0;  // 输出值
    pid[a].actual_val = 0;  // 实际值
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
 * @brief 设置电机的目标值
 *
 * @param motor_index 电机索引，范围为0到3
 * @param target_val 目标值，范围为MIN_TARGET到MAX_TARGET
 * @return int 如果索引或目标值无效，返回HAL_ERROR；否则返回设置的目标值
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
 * @brief 实现 PID 控制算法并返回 PWM 输出值
 * 
 * @param index PID 控制器的索引，范围为 0 到 3
 * @return int 返回更新后的 PWM 输出值，如果索引无效则返回 HAL_ERROR
 * 
 * 该函数实现了增量式 PID 控制算法。首先确保索引有效，然后获取当前实际值并计算误差。
 * 根据 PID 控制算法计算控制增量 Δoutput，并更新输出值。输出值会被限制在预设的范围内，
 * 防止饱和。最后将输出值转换为 PWM 输出，并返回更新后的 PWM 输出值。
 * 
 * 注意：函数内部使用了全局变量 pid 和常量 MAX_OUTPUT, MIN_OUTPUT, SCALE_FACTOR, PWM_MAX_VALUE。
 */

int PID_realize(int index)
{
  if (index < 0 || index >= 4)
    return HAL_ERROR; // 确保索引有效

  int pwm_output[4];
  int actual_val[4];
  actual_val[index] = Get_Speed(index); // 获取当前实际值
  pid[index].actual_val = actual_val[index];

  // 计算当前误差
  pid[index].err = pid[index].target_val - actual_val[index];

  // 计算增量式 PID 的控制增量 Δoutput
  float delta_output = pid[index].Kp * (pid[index].err - pid[index].err_last) +
                       pid[index].Ki * pid[index].err +
                       pid[index].Kd * (pid[index].err - 2 * pid[index].err_last + pid[index].err_prev);

  // 更新输出值
  pid[index].output_val += delta_output;

  // 限制输出值，防止饱和
  if (pid[index].output_val > MAX_OUTPUT)
    pid[index].output_val = MAX_OUTPUT;
  else if (pid[index].output_val < MIN_OUTPUT)
    pid[index].output_val = MIN_OUTPUT;

  // 将输出值转换为 PWM 输出
  pwm_output[index] = (int)(pid[index].output_val * SCALE_FACTOR);
  
  if (pwm_output[index] > PWM_MAX_VALUE)
    pwm_output[index] = PWM_MAX_VALUE;
  else if (pwm_output[index] < 0)
    pwm_output[index] = 0;
  // 更新误差
  pid[index].err_prev = pid[index].err_last;
  pid[index].err_last = pid[index].err;

  // 返回更新后的输出值
  return pwm_output[index];
}
