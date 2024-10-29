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
工程加入 PID 速度环控制
调试 P，I，D 三个参数
....

doule 输出小数后 15 位%.f15
float 输出小数后 6 位%.f6

9.21
9.22 加入 hcsr04.c
9.23 发现 bug，0 号电机无法读数，更换工程，相同参数后可以读数：
9.24bug 未解决
10.2 工程改名 smart_NEW,bug 解决，重新使用 tim1,2,4,5,原来 PA0，1 不能用的 pin 换成 PA5 和 PB3
printf 函数使用跟系统频率有关大于 40MHZ 无法使用
10.5 加入跟随测试代码
10.15.hc 调试成功 9600
10.18 加入 delay.c ，测试 hc 代码
10.20 测试 hc，发现小车是无头模式，不规则运动等，解决方案：使用 H743 板载陀螺仪，使用偏航角，来标识正方向
10.21 加入 sh3001，ium，i2cnew,其中 i2c 更名为 i2cnew
10.22 调试 sh3001，ium 代码
10.24 陀螺仪调试成功||{
hi2c2.Instance = I2C2;
hi2c2.Init.Timing = 0x00909FCE;
hi2c2.Init.OwnAddress1 = 0;
hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
hi2c2.Init.OwnAddress2 = 0;
hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
用 cubemx 配置的 i2c 代码使用的是软件 I2C，不需要配置 I2C 外设寄存器。项目使用硬件 I2C，需要进行硬件 I2C 的初始化
}
10.26 可以动
