/****************************************************************************************************
 * ������OLED Ӳ������������
 *
 * ���ߣ�������
 *
 * �汾��v1.0.0    ���ڣ�2018-04-25
 *                                                                              �����Ǻ��Ƽ����޹�˾
****************************************************************************************************/

#include "stm32f1xx.h"

#ifndef STM32F10X_OLED_H
#define STM32F10X_OLED_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
/* д���Ͷ��� */
	 
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
	 
#define OLED_CMD  0	   // д����
#define OLED_DATA 1	   // д����

/* ��ʱ */
	 
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	
	 
/****************************************************************************************************
 * ��������ʼ��SSD1306����	
 *
 * ��������
 *
 * ���أ���
****************************************************************************************************/

void OLED_Init(void);
	 
/****************************************************************************************************
 * ����������д��������
 *
 * ������Command = ����
 *
 * ���أ���
****************************************************************************************************/

void Write_IIC_Command(u8 Command);

/****************************************************************************************************
 * ������������������
 *
 * ������Data = ����
 *
 * ���أ���
****************************************************************************************************/

void Write_IIC_Data(u8 Data, u8 aga);

/****************************************************************************************************
 * �������������� / ����
 *
 * ������dat = ����
 * ������cmd = ���͵������ݻ�������
 *
 * ���أ���
****************************************************************************************************/

void OLED_WR_Byte(u8 dat,u8 cmd,u8 aga);

/****************************************************************************************************
 * ��������ʾһ��С��
 *
 * ������x, y:�������
 * ������num����ֵ
 * ������vir:С�������Чλ
 * ������size2�������С
 *
 * ���أ���
****************************************************************************************************/

void OLED_ShowDec(u8 x,u8 y,float num,u8 vir,u8 size2,u8 aga);

/****************************************************************************************************
 * ��������ʾһ���������Բ�ָ������
 *
 * ������x, y:�������
 * ������num����ֵ
 * ������size2�������С
 *
 * ���أ���
****************************************************************************************************/

void OLED_ShowNum_NL(u8 x,u8 y,u32 num, u8 size2,u8 aga);

/* δ�����ĺ������� */

void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size,u8 aga);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size,u8 aga);	 
void OLED_Set_Pos(u8 x, u8 y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no,u8 aga);
void OLED_DrawBMP(u8 x0, u8 y0,u8 x1, u8 y1,u8 BMP[]);
void fill_picture(unsigned char fill_Data);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2,u8 aga);

/* ѡ�� I2C �Ŀ��� */

#define SE_I2C1    0    // I2C ʹ��ѡ��, = 1 ʹ�� I2C1; = 0 ʹ�� SI2C

#ifdef __cplusplus
}
#endif

#endif /* STM32F10X_OLED_H */

/* End Of File */