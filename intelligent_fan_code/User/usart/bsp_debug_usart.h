#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f1xx.h"
#include <stdio.h>

//���ڲ�����
#define DEBUG_USART_BAUDRATE                    115200

//���Ŷ���
/*******************************************************/
#define DEBUG_USART                             USART2
#define DEBUG_USART_CLK_ENABLE()                __HAL_RCC_USART2_CLK_ENABLE();

#define DEBUG_USART_RX_GPIO_PORT                GPIOA
#define DEBUG_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_RX_PIN                      GPIO_PIN_3

#define DEBUG_USART_TX_GPIO_PORT                GPIOA
#define DEBUG_USART_TX_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USART_TX_PIN                      GPIO_PIN_2

#define DEBUG_USART_IRQHandler                  USART2_IRQHandler
#define DEBUG_USART_IRQ                 		USART2_IRQn
/************************************************************/

extern UART_HandleTypeDef UartHandle;

void usart_send_data(uint8_t *str, uint16_t len);
void Usart_SendString(uint8_t *str);
void DEBUG_USART_Config(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
extern UART_HandleTypeDef UartHandle;
#endif /* __USART1_H */