#ifndef __ESP8266_USART_H
#define	__ESP8266_USART_H

#include "stm32f1xx.h"
#include <stdio.h>

//串口波特率
#define ESP8266_USART_BAUDRATE                    9600

//引脚定义
/*******************************************************/
#define ESP8266_USART                             USART3
#define ESP8266_USART_CLK_ENABLE()                __HAL_RCC_USART3_CLK_ENABLE();

#define ESP8266_USART_RX_GPIO_PORT                GPIOB
#define ESP8266_USART_RX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define ESP8266_USART_RX_PIN                      GPIO_PIN_11

#define ESP8266_USART_TX_GPIO_PORT                GPIOB
#define ESP8266_USART_TX_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()
#define ESP8266_USART_TX_PIN                      GPIO_PIN_10

#define ESP8266_USART_IRQHandler                  USART3_IRQHandler
#define ESP8266_USART_IRQ                 		  USART3_IRQn

#define ESP8266_RESET_GPIO_PORT                   GPIOB
#define ESP8266_RESET_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define ESP8266_RESET_PIN                         GPIO_PIN_13

#define ESP8266_P0_GPIO_PORT                   GPIOB
#define ESP8266_P0_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define ESP8266_P0_PIN                         GPIO_PIN_14

#define ESP8266_P15_GPIO_PORT                   GPIOB
#define ESP8266_P15_GPIO_CLK_ENABLE()           __HAL_RCC_GPIOB_CLK_ENABLE()
#define ESP8266_P15_PIN                         GPIO_PIN_14
/************************************************************/

extern UART_HandleTypeDef UartHandle;

void usart_send_data(uint8_t *str, uint16_t len);
void ESP8266_USART_Config(void);
extern UART_HandleTypeDef UartHandle;
#endif /* __USART1_H */
