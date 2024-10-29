单位：天津天狮学院科技创新实验室
项目名称：智能导购车
版权所有：天津天狮学院科技创新实验室,天津天狮学院科技创新协会，E·I·Lab，STMicroelectronics
策划人：文芳建
执行：文芳建
编码：GB2312
克隆项目代码：git@github.com:junius237/wfj_xue32.git

更新日志：
2024
....
调试小车底盘，小车走不了直线问题
工程加入PID速度环控制
调试P，I，D三个参数
....

doule 输出小数后15位%.f15
float 输出小数后6位%.f6

9.21
9.22加入hcsr04.c
9.23发现bug，0号电机无法读数，更换工程，相同参数后可以读数：
9.24bug未解决
10.2工程改名smart_NEW,bug解决，重新使用tim1,2,4,5,原来PA0，1不能用的pin换成PA5和PB3
printf函数使用跟系统频率有关大于40MHZ无法使用
10.5加入跟随测试代码
10.15.hc调试成功
10.18加入delay.c ，测试hc代码
10.20测试hc，发现小车是无头模式，不规则运动等，解决方案：使用H743板载陀螺仪，使用偏航角，来标识正方向
10.21加入sh3001，ium，i2cnew,其中i2c更名为i2cnew
10.22调试sh3001，ium代码
10.24陀螺仪调试成功||{
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00909FCE;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
用cubemx配置的i2c代码使用的是软件I2C，不需要配置I2C外设寄存器。项目使用硬件I2C，需要进行硬件I2C的初始化
}
10.26sh3001解算成功，但是hc接收不到数据，







