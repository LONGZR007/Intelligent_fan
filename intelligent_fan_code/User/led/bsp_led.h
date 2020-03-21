#ifndef __LED_H
#define	__LED_H

#include "stm32f1xx.h"

//���Ŷ���
/*******************************************************/
//LED��
#define LED1_PIN                  GPIO_PIN_1                
#define LED1_GPIO_PORT            GPIOB                     
#define LED1_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

/************************************************************/

/** ����LED������ĺ꣬
	* LED�͵�ƽ��������ON=0��OFF=1
	* ��LED�ߵ�ƽ�����Ѻ����ó�ON=1 ��OFF=0 ����
	*/
#define OFF GPIO_PIN_SET
#define ON  GPIO_PIN_RESET

/* ���κ꣬��������������һ��ʹ�� */
#define LED1(a)	HAL_GPIO_WritePin(LED1_GPIO_PORT,LED1_PIN,a)


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			  //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRR=(uint32_t)i << 16;}				//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬


/* �������IO�ĺ� */
#define LED1_TOGGLE		digitalToggle(LED1_GPIO_PORT,LED1_PIN)
#define LED1_OFF		digitalLo(LED1_GPIO_PORT,LED1_PIN)
#define LED1_ON			digitalHi(LED1_GPIO_PORT,LED1_PIN)

void LED_GPIO_Config(void);

#endif /* __LED_H */
