#include "hcsr04.h"


double hc_sr04_get_distance(void)

{
    int count;
    double Distance;
    /* ��ʱ��13��ͨ��1��ģʽ��㣬��Ҫ���ڼ�����û��ʹ�ñ��ز��� */
    HC_Start(); // ����������ģ��

    HAL_TIM_Base_Start(&htim13); // ������ʱ��

    // �Գ���������˿ڲ���
    while (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_9) == GPIO_PIN_RESET)
        ; // �ȴ������ƽ����

    __HAL_TIM_SetCounter(&htim14, 0);

    // �Գ���������˿ڲ���
    while (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_9) == GPIO_PIN_SET)
        ; // �ȴ������ƽ���

    count = __HAL_TIM_GetCounter(&htim14);

    HAL_TIM_Base_Stop(&htim14);

    Distance = count * 340 / 2 * 0.000001 * 100;

    return Distance;
}

void HC_Start(void)
{

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET); // ����

    TIM13_Delay_us(15);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET); // ����
}
