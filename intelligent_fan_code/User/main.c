/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����led
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� F103 STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx.h"
#include "./usart/bsp_debug_usart.h"
#include "./led/bsp_led.h"  
#include "c_types.h"
#include "func_ops.h"
#include "device.h"
#include "err.h"
#include "product_def.h"
#include "./ir/drv_remote.h"
#include "./oled/drv_oled.h"
#include "./rtc/bsp_rtc.h"
#include "./fan/app_fan.h"
#include "./usart_esp8266/bsp_esp8266_usart.h"

/*****************************************************************************/
/* Local Definitions ( Constant and Macro )                                  */
/*****************************************************************************/
#define DEVICE_SN   "love_swl_001"

/*****************************************************************************/
/* Structures, Enum and Typedefs                                             */
/*****************************************************************************/

/*****************************************************************************/
/* Local Function Prototype                                                  */
/*****************************************************************************/

/*****************************************************************************/
/* Local Variables                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Global Variables                                                          */
/*****************************************************************************/

/*****************************************************************************/
/* External Functions and Variables                                          */
/*****************************************************************************/

/*****************************************************************************/
/* Function Implementation                                                   */
/*****************************************************************************/
/**
 * �豸�¼��ص�����
 * @param evt
 */
int32 device_evt_cb(struct device_event_t *evt)
{
    printf("evt id is %d\n", evt->evt_id);
	
	OLED_ShowString(0, 3, "                 ", 16, 1);
	
	/* �豸���������ӡ */
	switch (evt->evt_id)
	{
		case DEVICE_EVT_ID_NET_CONNECTED:
		{
//			usart_printf(USART1, "Net Connected Successful\r\n");
			OLED_ShowString(0, 3, "net config Succes", 16, 1);
		break;
		} 
		
		case DEVICE_EVT_ID_NET_DISCONNECTED:
		{
			OLED_ShowString(0, 3, "net config fall", 16, 1);
		break;
		} 
		
		case DEVICE_EVT_ID_ONENET_CONNECTED:
		{
			OLED_ShowString(0, 3, "ONENET CONNECTED", 16, 1);
			LED1_ON;
		break;
		} 
		
		case DEVICE_EVT_ID_ONENET_DISCONNECTED:
		{
			OLED_ShowString(0, 3, "ONENET DISCONNECTED", 16, 1);
			LED1_OFF;
//			usart_printf(USART1, "OneNet Disonnected\r\n");
//			GPIO_ResetBits(GPIOA, GPIO_Pin_4); //Ϩ�� led�� ��ʾ�Ͽ�ƽ̨
		break;
		}
		
		case DEVICE_EVT_ID_MODULE_RESET:
		{
			OLED_ShowString(0, 3, "Module Reset", 16, 1);
//			usart_printf(USART1, "Module Reset\r\n");
//			system_reset_flag = 1;
		break;
		}
		
		case DEVICE_EVT_ID_SMARTCONFIG_TIMEOUT:
		{
			OLED_ShowString(0, 3, "Smartconfig Timeout", 16, 1);
//			usart_printf(USART1, "Smartconfig Timeout\r\n");
		break;
		}
		
		case DEVICE_EVT_ID_FACTORY_TEST_OVER:
		{
			OLED_ShowString(0, 3, "Test Over", 16, 1);
//			usart_printf(USART1, "Test Over\r\n");
		break;
		} 
		
		case DEVICE_EVT_ID_SYNC_TIME:
		{
			OLED_ShowString(0, 3, "Time Synchronization", 16, 1);
//			usart_printf(USART1, "Time Synchronization\r\n");
		break;
		} 
		
		default: break;
	}

    return 0;
}

/**
 * ͨ�Ŵ������ݷ��ͽӿ�
 * @param data
 * @param data_len
 * @return
 */
int32 device_data_send_cb(uint8 *data, uint16 data_len)
{
    /** ͨ����WiFiģ�����ӵ�ͨ�Ŵ��ڷ�������*/
	usart_send_data(data, data_len); //�û�ʵ�ָú���
	
    return 0;
}

void IR_Deal_Task(void);
uint8_t pack[6] = {0x5A, 0x5A, 0x06, 0x00, 0x01, 0x00};
//uint8_t pack[] = "AT\r\n";
/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	HAL_Init();
	
	/* ����ϵͳʱ��Ϊ72 MHz */ 
	SystemClock_Config();
	
	/*��ʼ��USART ����ģʽΪ 115200 8-N-1���жϽ���*/
	DEBUG_USART_Config();
	
	/* RTC ���� */
	Rtc_Handle.Instance = RTC; 
	Rtc_Handle.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
	HAL_RTC_Init(&Rtc_Handle);
	
	/* ��ʼ��RGB�ʵ� */ 
	LED_GPIO_Config(); 
	
	int32 ret = ERR_OK;
	
	/** �����豸��ʼ��*/
	#ifdef  GSM_MODE
	ret = device_init(NULL, device_evt_cb, device_data_send_cb);
	#else
	ret = device_init(DEVICE_SN, device_evt_cb, device_data_send_cb);
	#endif
	if (ERR_OK != ret)
		return -1;
	
	/* ESP8266 ��ʼ�� */
	ESP8266_USART_Config();

	/* ���ȳ�ʼ�� */
	fan_gpio_config();

	/* ����ӿڳ�ʼ�� */
	Remote_Init();
	
	/* OLED��ʼ�� */
	OLED_Init();
	
	printf("Intelligent fan\r\n"); 
    
    /** ��ʼ����device_data_recv()���մ�������*/
	
	for (uint8_t i=0; i<5; i++)
	{
		pack[5] += pack[i];
	}
	
	device_data_send_cb(pack, 6);
	
	/** �豸��Ϣ���������������ϴ�*/
	while(1)
	{	
		IR_Deal_Task();
		
		ret = device_step();
		
		if (Rtc_Handle.Instance->CRL & 0x1)      // ��ȡ���־
		{
			Rtc_Handle.Instance->CRL &= ~0x1;    // ������־
			
			/* ÿ������ */

			if (Countdown > 0 && flag == 1)
			{
				Countdown--;
				if (Countdown % 60 == 0)
				{
					func_countdown_left_upload();    // ���ƶ��ϱ�ʣ��ķ�����
				}
				
				OLED_ShowString(0, 5, "     ", 16, 1);
				OLED_ShowNum(0, 5, Countdown, 4, 16, 1);
			}
			else if (flag == 1)
			{
				flag = 0;
				Countdown_Init = 0;
				OLED_ShowString(0, 5, "     ", 16, 1);
				FAN_CLOSE();    // ����ʱ�����رշ���
			}
		}
		/** �����ⲿ�������û��߼������Ƿ���Ҫ����device_smartconfig()��������*/

		/** ����Ƿ���Ҫ�ϴ����ݡ������Ҫ����ö�Ӧ��upload����*/
		//func_xxx_upload();
	}	
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 72000000
  *            HCLK(Hz)                       = 72000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 2
  *            APB2 Prescaler                 = 1
  *            HSE Frequency(Hz)              = 8000000
  *            HSE PREDIV1                    = 1
  *            PLLMUL                         = 9
  *            Flash Latency(WS)              = 2
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSEPredivValue  = RCC_HSE_PREDIV_DIV1;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB2CLKDivider = RCC_HCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_2)!= HAL_OK)
  {
    /* Initialization Error */
    while(1); 
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
