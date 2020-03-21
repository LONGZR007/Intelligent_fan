/**
  ******************************************************************************
  * @file    bsp_ESP8266_usart.c
  * @author  fire
  * @version V1.0
  * @date    2016-xx-xx
  * @brief   ʹ�ô���1���ض���c��printf������usart�˿ڣ��жϽ���ģʽ
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� STM32 F103 ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./usart_esp8266/bsp_esp8266_usart.h"

UART_HandleTypeDef UartHandle;


/**
  * @brief UART MSP ��ʼ�� 
  * @param huart: UART handle
  * @retval ��
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
	/* ����Tx����Ϊ���ù���  */
	GPIO_InitStruct.Pin = ESP8266_USART_TX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed =  GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ESP8266_USART_TX_GPIO_PORT, &GPIO_InitStruct);

	/* ����Rx����Ϊ���ù��� */
	GPIO_InitStruct.Pin = ESP8266_USART_RX_PIN;
	GPIO_InitStruct.Mode=GPIO_MODE_AF_INPUT;	//ģʽҪ����Ϊ��������ģʽ��	
	HAL_GPIO_Init(ESP8266_USART_RX_GPIO_PORT, &GPIO_InitStruct); 
}

 /**
  * @brief  ESP8266_USART GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
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

	HAL_NVIC_SetPriority(ESP8266_USART_IRQ ,0,1);	//��ռ���ȼ�0�������ȼ�1
	HAL_NVIC_EnableIRQ(ESP8266_USART_IRQ );		    //ʹ��USART1�ж�ͨ�� 

	/*ʹ�ܴ��ڽ��ն� */
	__HAL_UART_ENABLE_IT(&UartHandle,UART_IT_RXNE);  
}

void usart_send_data(uint8_t *str, uint16_t len)
{
	HAL_UART_Transmit(&UartHandle, (uint8_t *)(str) ,len, 1000);
}

/*********************************************END OF FILE**********************/
