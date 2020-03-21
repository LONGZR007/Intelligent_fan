/****************************************************************************************************
 * 描述：板级初始化实现
 *
 * 作者：龙泽润
 *
 * 版本：v1.0.0    日期：2018-04-15
 *                                                                              大连智海科技有限公司
****************************************************************************************************/

#include "./fan/app_fan.h"
#include "./ir/drv_remote.h"
#include "./oled/drv_oled.h"
#include "device.h"

uint32_t Countdown_Init = 0;    // 倒计时开始值
uint32_t Countdown = 0;         // 倒计时实际值
uint8_t flag = 0;

 /**
  * @brief  初始化控制LED的IO
  * @param  无
  * @retval 无
  */
void fan_gpio_config(void)
{		
		
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef  GPIO_InitStruct;

    /*开启LED相关的GPIO外设时钟*/
    FAN_GPIO_CLK_ENABLE();
  
    /*选择要控制的GPIO引脚*/															   
    GPIO_InitStruct.Pin = FAN_PIN;	

    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  

    /*设置引脚为上拉模式*/
    GPIO_InitStruct.Pull  = GPIO_PULLUP;

    /*设置引脚速率为高速 */   
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
    HAL_GPIO_Init(FAN_GPIO_PORT, &GPIO_InitStruct);
}

/****************************************************************************************************
 * 描述：
 *
 * 参数：无
 *
 * 返回：无
****************************************************************************************************/
	 
void IR_Deal_Task(void)
{
	static u8 Sign_1 = 0;    // 按键 1 标志
	static u8 Sign_2 = 0;    // 按键 2 标志
	static u8 Sign_3 = 0;    // 按键 3 标志
	static u8 Sign_4 = 0;    // 按键 4 标志
	static u8 Sign_5 = 0;    // 按键 5 标志
	static u8 Sign_6 = 0;    // 按键 6 标志
	static u8 Sign_7 = 0;    // 按键 7 标志
	static u8 Sign_8 = 0;    // 按键 8 标志
	static u8 Sign_9 = 0;    // 按键 9 标志
	static u8 Sign_0 = 0;    // 按键 0 标志
	static u8 Sign_P = 0;    // 按键 # 标志
	static u8 Sign_OK = 0;   // 按键 OK 标志
	static u8 Sign_A = 0;    // 按键 * 标志
	static u8 Sign_L = 0;    // 按键 < 标志
	static u8 Sign_R = 0;    // 按键 > 标志
	static u8 Sign_UP = 0;   // 按键 ↑ 标志
    static u32 xC = 0;
	static u32 Val[10];
	int32_t yC = 0;
	u32 iC = 0;
	
	if (K_1 == 1)           // 按键 S1 按下
	{
		Sign_1 = 1;          // 按键按下标志
	}
	else if (Sign_1 == 1)    // 按键 S1 短按弹起
	{
		Sign_1 = 0;          // 复位按键
		
		/* 单次按下 */

		Val[xC++] = 1;
	}

	if (K_2 == 1)           // 按键 S1 按下
	{
		Sign_2 = 1;          // 按键按下标志
	}
	else if (Sign_2 == 1)    // 按键 S1 短按弹起
	{
		Sign_2 = 0;          // 复位按键
		
		/* 单次按下 */

		Val[xC++] = 2;
	}
	
	if (K_3 == 1)           // 按键 S1 按下
	{
		Sign_3 = 1;          // 按键按下标志
	}
	else if (Sign_3 == 1)    // 按键 S1 短按弹起
	{
		Sign_3 = 0;          // 复位按键
		
		/* 单次按下 */

		Val[xC++] = 3;
	}
	
	if (K_4 == 1)           // 按键  按下
	{
		Sign_4 = 1;          // 按键按下标志
	}
	else if (Sign_4 == 1)    // 按键  短按弹起
	{
		Sign_4 = 0;          // 复位按键
		
		/* 单次按下 */

		Val[xC++] = 4;
	}
	
	if (K_5 == 1)           // 按键  按下
	{
		Sign_5 = 1;          // 按键按下标志
	}
	else if (Sign_5 == 1)    // 按键  短按弹起
	{
		Sign_5 = 0;          // 复位按键
		
		/* 单次按下 */

		Val[xC++] = 5;
	}
	
	if (K_6 == 1)           // 按键  按下
	{
		Sign_6 = 1;          // 按键按下标志
	}
	else if (Sign_6 == 1)    // 按键  短按弹起
	{
		Sign_6 = 0;          // 复位按键
		
		/* 单次按下 */

		Val[xC++] = 6;
	}
	
	if (K_7 == 1)           // 按键  按下
	{
		Sign_7 = 1;          // 按键按下标志
	}
	else if (Sign_7 == 1)    // 按键  短按弹起
	{
		Sign_7 = 0;          // 复位按键
		
		/* 单次按下 */

		Val[xC++] = 7;
	}
	
	if (K_8 == 1)           // 按键  按下
	{
		Sign_8 = 1;          // 按键按下标志
	}
	else if (Sign_8 == 1)    // 按键  短按弹起
	{
		Sign_8 = 0;          // 复位按键
		
		/* 单次按下 */

		Val[xC++] = 8;
	}

	if (K_9 == 1)           // 按键  按下
	{
		Sign_9 = 1;          // 按键按下标志
	}
	else if (Sign_9 == 1)    // 按键  短按弹起
	{
		Sign_9 = 0;          // 复位按键
		
		/* 单次按下 */

		Val[xC++] = 9;
	}
	
	if (K_0 == 1)           // 按键  按下
	{
		Sign_0 = 1;          // 按键按下标志
	}
	else if (Sign_0 == 1)    // 按键  短按弹起
	{
		Sign_0 = 0;          // 复位按键
		
		/* 单次按下 */

		Val[xC++] = 0;
	}
	
	if (K_P == 1)           // 按键  按下
	{
		Sign_P = 1;          // 按键按下标志
	}
	else if (Sign_P == 1)    // 按键  短按弹起
	{
		Sign_P = 0;          // 复位按键
		
		/* 单次按下 */

		if (xC>0)
		{
			xC --;
		}
	}
	
	if (K_OK == 1)           // 按键 OK 按下
	{
		Sign_OK = 1;          // 按键按下标志
	}
	else if (Sign_OK == 1)    // 按键 OK 短按弹起
	{
		Sign_OK = 0;          // 复位按键
		
		/* 单次按下 */

		iC = 1;
		for(yC=xC-1; yC>=0; yC--)
		{
			Countdown += Val[yC] * iC;    // 计算分钟数
			iC *= 10;
			printf("Countdown=%d, Val[yC]=%d, yC=%d\n\r", Countdown, Val[yC], yC);
		}
		 
		xC = 0;
		Countdown *= 60;    // 转换成秒
		if (Countdown > 0)
		{
			Countdown_Init = Countdown;
			flag = 1;
			
			FAN_OPEN();     // 开风扇
			
			func_countdown_upload();         // 向云端推送数据
			func_countdown_left_upload();    // 向云端推送数据
		}
	}
	
	/**/
	
	if (K_A == 1)           // 按键 * 按下
	{
		Sign_A = 1;          // 按键按下标志
	}
	else if (Sign_A == 1)    // 按键 * 短按弹起
	{
		Sign_A = 0;          // 复位按键
		
		/* 单次按下 */
		device_smartconfig(SMARTCONFIG_AUTO);    // 配网
		
		OLED_ShowString(0, 3, "net config", 16, 1);
	}
	
	if (K_L == 1)           // 按键 < 按下
	{
		Sign_L = 1;          // 按键按下标志
	}
	else if (Sign_L == 1)    // 按键 < 短按弹起
	{
		Sign_L = 0;          // 复位按键
		
		/* 单次按下 */

		FAN_OPEN();
		func_switch_upload();    // 向云端推送数据
		func_windspeed_upload();    // 向云端推送数据
		 
		Countdown = 0;    // 不计时
		Countdown_Init = 0;
		func_countdown_upload();         // 向云端推送数据
		xC = 0;
	}
	
	if (K_R == 1)           // 按键 > 按下
	{
		Sign_R = 1;          // 按键按下标志
	}
	else if (Sign_R == 1)    // 按键 > 短按弹起
	{
		Sign_R = 0;          // 复位按键
		
		/* 单次按下 */

		FAN_CLOSE();
		func_switch_upload();    // 向云端推送数据
		func_windspeed_upload();    // 向云端推送数据
		 
		Countdown = 0;    // 不计时
		Countdown_Init = 0;
		func_countdown_upload();         // 向云端推送数据
		xC = 0;
	}
	
	if (K_UP == 1)           // 按键 > 按下
	{
		Sign_UP = 1;          // 按键按下标志
	}
	else if (Sign_UP == 1)    // 按键 > 短按弹起
	{
		Sign_UP = 0;          // 复位按键
		
		/* 单次按下 */

		HAL_NVIC_SystemReset();
	}
	
	if (xC > 8)
	{
		xC = 8;
	}
	
	/* 显示 */
	OLED_ShowString(0, 0, "         ", 16, 1);
	iC = 0;
	for(yC=0; yC<xC; yC++)
	{
		OLED_ShowNum(iC, 0, Val[yC], 1, 16, 1);	   // 显示SYMBOL
		iC += 8;
	}
}



/* End Of File */
