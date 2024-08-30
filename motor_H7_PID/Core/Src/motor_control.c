#include "motor_control.h"
#include "tim.h"
#include "pid.h"
#include "stm32H7xx_it.h"

static motor_dir motor_fangxian = MOTOR_FWD; // 记录方向
static uint16_t duty = 0;                    // 记录占空比
// static uint8_t motor_en = 0;                 // 电机使能

/**
 * @brief  设置电机速度
 * @param  v: 速度（占空比）
 * @retval 无
 */
void set_motor_speed(uint16_t v)
{
  v = (v > 3600) ? 3600 : v; // 上限处理

  duty = v;

  if (motor_fangxian == MOTOR_FWD)
  {
    SET_FWD_COMPAER(duty); // 设置速度
  }
  else
  {
    SET_REV_COMPAER(duty); // 设置速度
  }
}

/**
 * @brief  设置电机方向
 * @param  无
 * @retval 无
 */
void set_motor_fangxiang(motor_dir dir)
{
  motor_fangxian = dir;

  if (motor_fangxian == MOTOR_FWD)
  {
    SET_FWD_COMPAER(duty); // 设置速度
    SET_REV_COMPAER(0);    // 设置速度
  }
  else
  {
    SET_FWD_COMPAER(0);    // 设置速度
    SET_REV_COMPAER(duty); // 设置速度
  }
}

/**
 * @brief  电机位置式 PID 控制实现(定时调用)
 * @param  无
 * @retval 无
 */
void motor_pid_control(void)
{
  if (1) // 电机在使能状态下才进行控制处理
  {
    float cont_val = 0;                    // 当前控制值
    static __IO int32_t Capture_Count = 0; // 当前时刻总计数值
    static __IO int32_t Last_Count = 0;    // 上一时刻总计数值
    int32_t actual_speed = 0;              // 实际测得速度

    /* 当前时刻总计数值 = 计数器值 + 计数溢出次数 * ENCODER_TIM_PERIOD  */
    Capture_Count = __HAL_TIM_GET_COUNTER(&htim2) + (Encoder_Overflow_Count * 65535);

    /* 转轴转速 = 单位时间内的计数值 / 编码器总分辨率 * 时间系数  */
    // 30-减速比
    actual_speed = ((float)(Capture_Count - Last_Count) / (11 * 4) / 30) / (GET_BASIC_TIM_PERIOD() / 1000.0 / 60.0);

    /* 记录当前总计数值，供下一时刻计算使用 */
    Last_Count = Capture_Count;

    cont_val = PID_realize(actual_speed); // 进行 PID 计算

    if (cont_val > 0) // 判断电机方向
    {
      set_motor_fangxiang(MOTOR_FWD);
    }
    else
    {
      cont_val = -cont_val;
      set_motor_fangxiang(MOTOR_REV);
    }

    cont_val = (cont_val > 3600) ? 3600 : cont_val; // 速度上限处理

    set_motor_speed(cont_val); // 设置 PWM 占空比
  }
}
