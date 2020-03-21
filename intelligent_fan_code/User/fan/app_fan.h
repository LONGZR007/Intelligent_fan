#ifndef __APP_FAN_H_
#define	__APP_FAN_H_

#include "stm32f1xx.h"


//���Ŷ���
/*******************************************************/
//LED��
#define FAN_PIN                  GPIO_PIN_0
#define FAN_GPIO_PORT            GPIOB                     
#define FAN_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

/************************************************************/

/* ���Ʒ��ȿ��صĺ� */
#define FAN_OPEN()     HAL_GPIO_WritePin(FAN_GPIO_PORT, FAN_PIN, GPIO_PIN_SET)
#define FAN_CLOSE()    HAL_GPIO_WritePin(FAN_GPIO_PORT, FAN_PIN, GPIO_PIN_RESET)

extern uint32_t Countdown;    // ����ʱ
extern uint32_t Countdown_Init;      // ����ʱ��ʼֵ
extern uint8_t flag;

void fan_gpio_config(void);

#endif /* __APP_FAN_H_ */
