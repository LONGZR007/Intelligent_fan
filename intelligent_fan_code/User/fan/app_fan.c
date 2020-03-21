/****************************************************************************************************
 * �������弶��ʼ��ʵ��
 *
 * ���ߣ�������
 *
 * �汾��v1.0.0    ���ڣ�2018-04-15
 *                                                                              �����Ǻ��Ƽ����޹�˾
****************************************************************************************************/

#include "./fan/app_fan.h"
#include "./ir/drv_remote.h"
#include "./oled/drv_oled.h"
#include "device.h"

uint32_t Countdown_Init = 0;    // ����ʱ��ʼֵ
uint32_t Countdown = 0;         // ����ʱʵ��ֵ
uint8_t flag = 0;

 /**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void fan_gpio_config(void)
{		
		
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef  GPIO_InitStruct;

    /*����LED��ص�GPIO����ʱ��*/
    FAN_GPIO_CLK_ENABLE();
  
    /*ѡ��Ҫ���Ƶ�GPIO����*/															   
    GPIO_InitStruct.Pin = FAN_PIN;	

    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  

    /*��������Ϊ����ģʽ*/
    GPIO_InitStruct.Pull  = GPIO_PULLUP;

    /*������������Ϊ���� */   
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
    HAL_GPIO_Init(FAN_GPIO_PORT, &GPIO_InitStruct);
}

/****************************************************************************************************
 * ������
 *
 * ��������
 *
 * ���أ���
****************************************************************************************************/
	 
void IR_Deal_Task(void)
{
	static u8 Sign_1 = 0;    // ���� 1 ��־
	static u8 Sign_2 = 0;    // ���� 2 ��־
	static u8 Sign_3 = 0;    // ���� 3 ��־
	static u8 Sign_4 = 0;    // ���� 4 ��־
	static u8 Sign_5 = 0;    // ���� 5 ��־
	static u8 Sign_6 = 0;    // ���� 6 ��־
	static u8 Sign_7 = 0;    // ���� 7 ��־
	static u8 Sign_8 = 0;    // ���� 8 ��־
	static u8 Sign_9 = 0;    // ���� 9 ��־
	static u8 Sign_0 = 0;    // ���� 0 ��־
	static u8 Sign_P = 0;    // ���� # ��־
	static u8 Sign_OK = 0;   // ���� OK ��־
	static u8 Sign_A = 0;    // ���� * ��־
	static u8 Sign_L = 0;    // ���� < ��־
	static u8 Sign_R = 0;    // ���� > ��־
	static u8 Sign_UP = 0;   // ���� �� ��־
    static u32 xC = 0;
	static u32 Val[10];
	int32_t yC = 0;
	u32 iC = 0;
	
	if (K_1 == 1)           // ���� S1 ����
	{
		Sign_1 = 1;          // �������±�־
	}
	else if (Sign_1 == 1)    // ���� S1 �̰�����
	{
		Sign_1 = 0;          // ��λ����
		
		/* ���ΰ��� */

		Val[xC++] = 1;
	}

	if (K_2 == 1)           // ���� S1 ����
	{
		Sign_2 = 1;          // �������±�־
	}
	else if (Sign_2 == 1)    // ���� S1 �̰�����
	{
		Sign_2 = 0;          // ��λ����
		
		/* ���ΰ��� */

		Val[xC++] = 2;
	}
	
	if (K_3 == 1)           // ���� S1 ����
	{
		Sign_3 = 1;          // �������±�־
	}
	else if (Sign_3 == 1)    // ���� S1 �̰�����
	{
		Sign_3 = 0;          // ��λ����
		
		/* ���ΰ��� */

		Val[xC++] = 3;
	}
	
	if (K_4 == 1)           // ����  ����
	{
		Sign_4 = 1;          // �������±�־
	}
	else if (Sign_4 == 1)    // ����  �̰�����
	{
		Sign_4 = 0;          // ��λ����
		
		/* ���ΰ��� */

		Val[xC++] = 4;
	}
	
	if (K_5 == 1)           // ����  ����
	{
		Sign_5 = 1;          // �������±�־
	}
	else if (Sign_5 == 1)    // ����  �̰�����
	{
		Sign_5 = 0;          // ��λ����
		
		/* ���ΰ��� */

		Val[xC++] = 5;
	}
	
	if (K_6 == 1)           // ����  ����
	{
		Sign_6 = 1;          // �������±�־
	}
	else if (Sign_6 == 1)    // ����  �̰�����
	{
		Sign_6 = 0;          // ��λ����
		
		/* ���ΰ��� */

		Val[xC++] = 6;
	}
	
	if (K_7 == 1)           // ����  ����
	{
		Sign_7 = 1;          // �������±�־
	}
	else if (Sign_7 == 1)    // ����  �̰�����
	{
		Sign_7 = 0;          // ��λ����
		
		/* ���ΰ��� */

		Val[xC++] = 7;
	}
	
	if (K_8 == 1)           // ����  ����
	{
		Sign_8 = 1;          // �������±�־
	}
	else if (Sign_8 == 1)    // ����  �̰�����
	{
		Sign_8 = 0;          // ��λ����
		
		/* ���ΰ��� */

		Val[xC++] = 8;
	}

	if (K_9 == 1)           // ����  ����
	{
		Sign_9 = 1;          // �������±�־
	}
	else if (Sign_9 == 1)    // ����  �̰�����
	{
		Sign_9 = 0;          // ��λ����
		
		/* ���ΰ��� */

		Val[xC++] = 9;
	}
	
	if (K_0 == 1)           // ����  ����
	{
		Sign_0 = 1;          // �������±�־
	}
	else if (Sign_0 == 1)    // ����  �̰�����
	{
		Sign_0 = 0;          // ��λ����
		
		/* ���ΰ��� */

		Val[xC++] = 0;
	}
	
	if (K_P == 1)           // ����  ����
	{
		Sign_P = 1;          // �������±�־
	}
	else if (Sign_P == 1)    // ����  �̰�����
	{
		Sign_P = 0;          // ��λ����
		
		/* ���ΰ��� */

		if (xC>0)
		{
			xC --;
		}
	}
	
	if (K_OK == 1)           // ���� OK ����
	{
		Sign_OK = 1;          // �������±�־
	}
	else if (Sign_OK == 1)    // ���� OK �̰�����
	{
		Sign_OK = 0;          // ��λ����
		
		/* ���ΰ��� */

		iC = 1;
		for(yC=xC-1; yC>=0; yC--)
		{
			Countdown += Val[yC] * iC;    // ���������
			iC *= 10;
			printf("Countdown=%d, Val[yC]=%d, yC=%d\n\r", Countdown, Val[yC], yC);
		}
		 
		xC = 0;
		Countdown *= 60;    // ת������
		if (Countdown > 0)
		{
			Countdown_Init = Countdown;
			flag = 1;
			
			FAN_OPEN();     // ������
			
			func_countdown_upload();         // ���ƶ���������
			func_countdown_left_upload();    // ���ƶ���������
		}
	}
	
	/**/
	
	if (K_A == 1)           // ���� * ����
	{
		Sign_A = 1;          // �������±�־
	}
	else if (Sign_A == 1)    // ���� * �̰�����
	{
		Sign_A = 0;          // ��λ����
		
		/* ���ΰ��� */
		device_smartconfig(SMARTCONFIG_AUTO);    // ����
		
		OLED_ShowString(0, 3, "net config", 16, 1);
	}
	
	if (K_L == 1)           // ���� < ����
	{
		Sign_L = 1;          // �������±�־
	}
	else if (Sign_L == 1)    // ���� < �̰�����
	{
		Sign_L = 0;          // ��λ����
		
		/* ���ΰ��� */

		FAN_OPEN();
		func_switch_upload();    // ���ƶ���������
		func_windspeed_upload();    // ���ƶ���������
		 
		Countdown = 0;    // ����ʱ
		Countdown_Init = 0;
		func_countdown_upload();         // ���ƶ���������
		xC = 0;
	}
	
	if (K_R == 1)           // ���� > ����
	{
		Sign_R = 1;          // �������±�־
	}
	else if (Sign_R == 1)    // ���� > �̰�����
	{
		Sign_R = 0;          // ��λ����
		
		/* ���ΰ��� */

		FAN_CLOSE();
		func_switch_upload();    // ���ƶ���������
		func_windspeed_upload();    // ���ƶ���������
		 
		Countdown = 0;    // ����ʱ
		Countdown_Init = 0;
		func_countdown_upload();         // ���ƶ���������
		xC = 0;
	}
	
	if (K_UP == 1)           // ���� > ����
	{
		Sign_UP = 1;          // �������±�־
	}
	else if (Sign_UP == 1)    // ���� > �̰�����
	{
		Sign_UP = 0;          // ��λ����
		
		/* ���ΰ��� */

		HAL_NVIC_SystemReset();
	}
	
	if (xC > 8)
	{
		xC = 8;
	}
	
	/* ��ʾ */
	OLED_ShowString(0, 0, "         ", 16, 1);
	iC = 0;
	for(yC=0; yC<xC; yC++)
	{
		OLED_ShowNum(iC, 0, Val[yC], 1, 16, 1);	   // ��ʾSYMBOL
		iC += 8;
	}
}



/* End Of File */
