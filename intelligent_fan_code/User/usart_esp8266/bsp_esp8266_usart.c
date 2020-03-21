/**
  ******************************************************************************
  * @file    bsp_ESP8266_usart.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   使用串口1，重定向c库printf函数到usart端口，中断接收模式
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32 F103 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart_esp8266/bsp_esp8266_usart.h"

UART_HandleTypeDef UartHandle;


/**
  * @brief UART MSP 初始化 
  * @param huart: UART handle
  * @retval 无
  */
void ESP8266_UART_MspInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	ESP8266_USART_CLK_ENABLE();

	ESP8266_USART_RX_GPIO_CLK_ENABLE();
	ESP8266_USART_TX_GPIO_CLK_ENABLE();
	ESP8266_RESET_GPIO_CLK_ENABLE();

	GPIO_InitStruct.Pin = ESP8266_RESET_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed =  GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ESP8266_RESET_GPIO_PORT, &GPIO_InitStruct);
	
	HAL_GPIO_WritePin(ESP8266_RESET_GPIO_PORT, ESP8266_RESET_PIN, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(ESP8266_RESET_GPIO_PORT, ESP8266_RESET_PIN, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = ESP8266_P0_PIN;
	HAL_GPIO_Init(ESP8266_P0_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(ESP8266_P0_GPIO_PORT, ESP8266_P0_PIN, GPIO_PIN_SET);

	/**USART1 GPIO Configuration    
	PA9     ------> USART1_TX
	PA10    ------> USART1_RX 
	*/
	/* 配置Tx引脚为复用功能  */
	GPIO_InitStruct.Pin = ESP8266_USART_TX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed =  GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ESP8266_USART_TX_GPIO_PORT, &GPIO_InitStruct);

	/* 配置Rx引脚为复用功能 */
	GPIO_InitStruct.Pin = ESP8266_USART_RX_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_AF_INPUT;	//模式要设置为复用输入模式！	
	HAL_GPIO_Init(ESP8266_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
}

 /**
  * @brief  ESP8266_USART GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */  
void ESP8266_USART_Config(void)
{ 
	ESP8266_UART_MspInit();

	UartHandle.Instance          = ESP8266_USART;

	UartHandle.Init.BaudRate     = ESP8266_USART_BAUDRATE;
	UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits     = UART_STOPBITS_1;
	UartHandle.Init.Parity       = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode         = UART_MODE_TX_RX;

	HAL_UART_Init(&UartHandle);

	HAL_NVIC_SetPriority(ESP8266_USART_IRQ ,0,1);	//抢占优先级0，子优先级1
	HAL_NVIC_EnableIRQ(ESP8266_USART_IRQ );		    //使能USART1中断通道 

	/*使能串口接收断 */
	__HAL_UART_ENABLE_IT(&UartHandle,UART_IT_RXNE);  
}

void usart_send_data(uint8_t *str, uint16_t len)
{
	HAL_UART_Transmit(&UartHandle, (uint8_t *)(str) ,len, 1000);
}

/*********************************************END OF FILE**********************/
