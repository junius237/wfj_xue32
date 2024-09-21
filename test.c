#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void set_p_i_d(int index, float p[], float i[], float d[]);
void PID_init(float p[], float i[], float d[]);
int Set_target_val(int motor_index, int target_val);
float PID_realize(int index);
typedef struct
{
  float target_val;
  float actual_val;
  float output_val;
  float err;
  float err_last,err_prev;
  float Kp, Ki, Kd;
  float integral;
} PID;

PID pid[4];

int main()
{
  float p[4] = {5, 5, 5, 5};
  float i[4] = {2, 2, 2, 2};
  float d[4] = {2, 2, 2, 2};

  PID_init(p, i, d);

  Set_target_val(0, 35);
  Set_target_val(1, 15);
  Set_target_val(2, 55);
  Set_target_val(3, 25);
  printf("Kp: %fKp: %fKp: %fKp: %f\n", pid[0].Kp, pid[1].Kp, pid[2].Kp, pid[3].Kp);
  printf("Ki: %fKi: %fKi: %fKi: %f\n", pid[0].Ki, pid[1].Ki, pid[2].Ki, pid[3].Ki);
  printf("Kd: %fKd: %fKd: %fKd: %f\n", pid[0].Kd, pid[1].Kd, pid[2].Kd, pid[3].Kd);
  printf("target_val: %f\n", pid[0].target_val);
  printf("target_val: %f\n", pid[1].target_val);
  printf("target_val: %f\n", pid[2].target_val);
  printf("target_val: %f\n", pid[3].target_val);
  printf("output_val: %f\n", PID_realize(0));
  printf("output_val: %f\n", PID_realize(1));
  printf("output_val: %f\n", PID_realize(2));
  printf("output_val: %f\n", PID_realize(3));

  return 0;
}

int Set_target_val(int motor_index, int target_val)
{
  if (motor_index < 0 || motor_index > 3)
    return 0;
  pid[motor_index].target_val = target_val;
  return target_val;
}

void PID_init(float p[], float i[], float d[])
{
  for (int a = 0; a < 4; a++)
  {
    pid[a].Kp = p[a];
    pid[a].Ki = i[a];
    pid[a].Kd = d[a];
    pid[a].integral = 0;
    pid[a].err_last = 0;
    pid[a].output_val = 0;
    pid[a].target_val = 0;
    pid[a].actual_val = 0;
    pid[a].err = 0;
  }
}

float PID_realize(int index)
{
  if (index < 0 || index >= 4)
    return 0; // 确保索引有效

  float actual_val=0;
 
  pid[index].actual_val = actual_val;

  // 计算当前误差
  pid[index].err = pid[index].target_val - actual_val;

  // 计算增量式 PID 的控制增量 Δoutput
  float delta_output = pid[index].Kp * (pid[index].err - pid[index].err_last) +
                       pid[index].Ki * pid[index].err +
                       pid[index].Kd * (pid[index].err - 2 * pid[index].err_last + pid[index].err_prev);

  // 更新输出值
  pid[index].output_val += delta_output;

  // 可选：限制输出值，防止饱和
  if (pid[index].output_val > 500)
    pid[index].output_val = 500;
  else if (pid[index].output_val < -500)
    pid[index].output_val = -500;

  // 更新误差历史
  pid[index].err_prev = pid[index].err_last;
  pid[index].err_last = pid[index].err;

  // 返回更新后的输出值
  return pid[index].output_val;
}