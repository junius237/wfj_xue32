



本程序实现超声波测距串口1输出和OLED显示距离功能（使用十六进制模式，关于十六进制模式，可以参考说明手册）
单片机型号：STM32F103C8T6，使用内部晶振，总线频率8MHZ，串口1和串口2的波特率都为9600
-----------------------------------------------------------------
硬件接口：
测试板-------------0.96寸OLED-IIC接口(GND VCC SCL SDA)
PB6-----------------------SCL
PB7-----------------------SDA
---------------------------------
测试板-------------超声波接收模块（5V-可接3.3-5V电源，GND接地）
PA3(RX2)---------------TXD
PA2(TX2)---------------不接
-----------------------------------------------------------------
功能说明：
1、用户购买本店测试底板
用TYPE-C数据线把电脑和底板连接，电脑端打开串口助手，即可看到距离信息（距离计算请参考说明手册）
2、用户没有购买本店测试底板
串口1的PA9(TX1)引脚接到USB转串口模块的RXD口上，即可以通过串口看到距离信息
-----------------------------------------------------------------
只要液晶屏接口按照上述OLED接法，即可在液晶屏显示距离信息和相关测距信息，具体信息如下：
如果没有有效数据：显示
Data invalid 
----------
------------------------------
如果数据错误：显示
Data invalid 
Data Error
------------------------------
如果探测距离太近：显示
Data invalid 
Too close
------------------------------
如果数据有效：显示
Now Distance 
实时距离：XXXmm
-----------------------------------------------------------------
