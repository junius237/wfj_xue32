��λ�������ʨѧԺ�Ƽ�����ʵ����
��Ŀ���ƣ����ܵ�����
��Ȩ���У������ʨѧԺ�Ƽ�����ʵ����,�����ʨѧԺ�Ƽ�����Э�ᣬE��I��Lab��STMicroelectronics
�߻��ˣ��ķ���
ִ�У��ķ���
���룺GB2312
��¡��Ŀ���룺git@github.com:junius237/wfj_xue32.git

������־��
2024
....
����С�����̣�С���߲���ֱ������
���̼��� PID �ٶȻ�����
���� P��I��D ��������
....

doule ���С���� 15 λ%.f15
float ���С���� 6 λ%.f6

9.21
9.22 ���� hcsr04.c
9.23 ���� bug��0 �ŵ���޷��������������̣���ͬ��������Զ�����
9.24bug δ���
10.2 ���̸��� smart_NEW,bug ���������ʹ�� tim1,2,4,5,ԭ�� PA0��1 �����õ� pin ���� PA5 �� PB3
printf ����ʹ�ø�ϵͳƵ���йش��� 40MHZ �޷�ʹ��
10.5 ���������Դ���
10.15.hc ���Գɹ� 9600
10.18 ���� delay.c ������ hc ����
10.20 ���� hc������С������ͷģʽ���������˶��ȣ����������ʹ�� H743 ���������ǣ�ʹ��ƫ���ǣ�����ʶ������
10.21 ���� sh3001��ium��i2cnew,���� i2c ����Ϊ i2cnew
10.22 ���� sh3001��ium ����
10.24 �����ǵ��Գɹ�||{
hi2c2.Instance = I2C2;
hi2c2.Init.Timing = 0x00909FCE;
hi2c2.Init.OwnAddress1 = 0;
hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
hi2c2.Init.OwnAddress2 = 0;
hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
�� cubemx ���õ� i2c ����ʹ�õ������ I2C������Ҫ���� I2C ����Ĵ�������Ŀʹ��Ӳ�� I2C����Ҫ����Ӳ�� I2C �ĳ�ʼ��
}
10.26 ���Զ�
