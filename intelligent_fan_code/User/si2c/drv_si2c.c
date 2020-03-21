/****************************************************************************************************
 * ������SI2C �ӿ�Ӳ��������ʵ��
 *
 * ���ߣ�������
 *
 * �汾��v1.0.0    ���ڣ�2018-05-02
 *                                                                              �����Ǻ��Ƽ����޹�˾
****************************************************************************************************/

#include "./si2c/drv_si2c.h"
#include "./oled/drv_oled.h"

/****************************************************************************************************
 * ������SI2C1 ���߳�ʼ��
 *
 * ��������
 *
 * ���أ���
****************************************************************************************************/

void SI2C_Init(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef  GPIO_InitStruct;

    /*����LED��ص�GPIO����ʱ��*/
    SCL_GPIO_CLK_ENABLE();
    SDA_GPIO_CLK_ENABLE();
  
    /*ѡ��Ҫ���Ƶ�GPIO����*/															   
    GPIO_InitStruct.Pin = SCL_PIN;	

    /*�������ŵ��������Ϊ�������*/
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;  

    /*��������Ϊ����ģʽ*/
    GPIO_InitStruct.Pull  = GPIO_NOPULL;

    /*������������Ϊ���� */   
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
    HAL_GPIO_Init(SCL_GPIO_PORT, &GPIO_InitStruct);	
	
	GPIO_InitStruct.Pin = SDA_PIN;
	HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);
}

/****************************************************************************************************
 * ���������� SDA Ϊ���
 *
 * ��������
 *
 * ���أ���
****************************************************************************************************/

void SI2C_SDA_OUT(void)
{
//	GPIO_Config(GPIOA, SI2C_SDA, GPIO_MODE_GP_PP, GPIO_SPEED_50M);    // SDA �ܽ�, ͨ���������, �ٶ� 50 MHz
}

/****************************************************************************************************
 * ���������� SDA Ϊ����
 *
 * ��������
 *
 * ���أ���
****************************************************************************************************/

void SI2C_SDA_IN(void)
{
//	GPIO_Config(GPIOA, SI2C_SDA, GPIO_MODE_IPU, 0);    // SDA �ܽ�, ��������
}

void SI2C_SCL_SET(void)  	
{	
	HAL_GPIO_WritePin(SCL_GPIO_PORT, SCL_PIN, GPIO_PIN_SET);   // SCL �����λ
}

void SI2C_SCL_RST(void)    
{
	HAL_GPIO_WritePin(SCL_GPIO_PORT, SCL_PIN, GPIO_PIN_RESET); // SCL �����λ
}

void SI2C_SDA_SET(void)    
{
	HAL_GPIO_WritePin(SCL_GPIO_PORT, SDA_PIN, GPIO_PIN_SET); // SDA �����λ
} 

void SI2C_SDA_RST(void)    
{
	HAL_GPIO_WritePin(SCL_GPIO_PORT, SDA_PIN, GPIO_PIN_RESET);    // SDA �����λ
}

u8 SI2C_SDA_GET(void)    
{
	return HAL_GPIO_ReadPin(SCL_GPIO_PORT, SDA_PIN);    // SDA �����ȡ
}

/****************************************************************************************************
 * ������SI2C ������ʼ�ź�
 *
 * ��������
 *
 * ���أ���
****************************************************************************************************/

void SI2C_Start(void)
{
	SI2C_SDA_OUT();    // SDA ����Ϊ���
	SI2C_SCL_SET();    // ����ʱ����
	SI2C_SDA_SET();    // SDA Ϊ��
	SI2C_SDA_RST();    // SDA Ϊ��
	SI2C_SCL_RST();    // ����ʱ���ߣ��ȴ����ݷ���
}

/****************************************************************************************************
 * ������SI2C ����ֹͣ�ź�
 *
 * ��������
 *
 * ���أ���
****************************************************************************************************/

void SI2C_Stop(void)
{
	SI2C_SDA_OUT();    // SDA ����Ϊ���
	
	SI2C_SCL_SET();    // ����ʱ����
	SI2C_SDA_RST();    // SDA Ϊ��
	SI2C_SDA_SET();    // SDA Ϊ��
//    SI2C_SCL_RST();    // ����ʱ����

}

/****************************************************************************************************
 * �������ȴ�Ӧ���ź�
 *
 * ��������
 *
 * ���أ�1�����ճɹ�    0������ʧ��
****************************************************************************************************/

u8 SI2C_Wait_ACK(void)
{
//	u8 ACKErrTime = 0;    // ѭ������
	
//	SI2C_SDA_IN();        // SDA ����Ϊ����  
	
	SI2C_SCL_SET();       // ����ʱ����

//	while(SI2C_SDA_GET() == 1)
	{

//		if(ACKErrTime++ > 250)    // ��ʱδ���յ�Ӧ��
//		{
////			SI2C_Stop();          // ����ֹͣ�ź�
//			
//			return 0;             // ���� 0 ����Ӧ��ʧ��
//		}
	}
	
	SI2C_SCL_RST();               // ����ʱ����

	return 1;                     // ���� 1 ����Ӧ��ɹ�
} 

/****************************************************************************************************
 * ����������Ӧ���ź�
 *
 * ��������
 *
 * ���أ���
****************************************************************************************************/

void SI2C_ACK(void)
{	
	SI2C_SCL_RST();    // ����ʱ����
	
	SI2C_SDA_OUT();    // SDA ����Ϊ���
	
	SI2C_SDA_RST();    // ��λ SDA
	Delay_us(2);       // ��ʱ 2us
	SI2C_SCL_SET();    // ����ʱ����
	Delay_us(2);       // ��ʱ 2us
	SI2C_SCL_RST();    // ����ʱ����
}

/****************************************************************************************************
 * ������������Ӧ���ź�
 *
 * ��������
 *
 * ���أ���
****************************************************************************************************/

void SI2C_NACK(void)
{
	SI2C_SCL_RST();    // ����ʱ����
	
	SI2C_SDA_OUT();    // SDA ����Ϊ���
	
	SI2C_SDA_SET();    // SDA �ø�
	Delay_us(2);       // ��ʱ 2us
	SI2C_SCL_SET();    // ����ʱ����
	Delay_us(2);       // ��ʱ 2us
	SI2C_SCL_RST();    // ����ʱ����
}

/****************************************************************************************************
 * ������SI2C ����һ���ֽ�
 *
 * ������txd�����͵�����
 *
 * ���أ���
****************************************************************************************************/

void SI2C_Send_Byte(u8 txd)
{                        
    u8 Count;          // ѭ������
	
	SI2C_SDA_OUT();    // SDA ����Ϊ���	    
	SI2C_SCL_RST();    // ����ʱ���߿�ʼ���ݴ���
	
    for(Count=8; Count>0; Count--)
    {
		if (txd & (1 << (Count - 1)))
		{
			SI2C_SDA_SET();    // SDA �ø�
		}
		else
		{
			SI2C_SDA_RST();    // SDA �õ�
		}
		SI2C_SCL_SET();        // ����ʱ����
		SI2C_SCL_RST();        // ����ʱ����
    }	 
}

/****************************************************************************************************
 * ������SI2C ����һ���ֽ�(����)
 *
 * ������txd�����͵�����
 *
 * ���أ���
****************************************************************************************************/

void SI2C_Send_Against_Byte(u8 txd)
{                        
    u8 Count;          // ѭ������
	
	SI2C_SDA_OUT();    // SDA ����Ϊ���	    
	SI2C_SCL_RST();    // ����ʱ���߿�ʼ���ݴ���
	
    for(Count=8; Count>0; Count--)
    {
		if (txd & (1 << (Count - 1)))
		{
			SI2C_SDA_RST();    // SDA �õ�
		}
		else
		{
			SI2C_SDA_SET();    // SDA �ø�
		}
		SI2C_SCL_SET();        // ����ʱ����
		SI2C_SCL_RST();        // ����ʱ����
    }	 
}

/****************************************************************************************************
 * ������SI2C ����һ���ֽ�
 *
 * ������ack = 1������ACK
 * ������ack = 0������NACK 
 *
 * ���أ�SI2C ���յ�������
****************************************************************************************************/

u8 SI2C_Read_Byte(u8 ack)
{
	u8 Count;        // ѭ������
	u8 Receive = 0;    // ������յ�����
	
	SI2C_SDA_IN();     // SDA ����Ϊ����
	
    for(Count=0;Count<8;Count++ )
	{
		SI2C_SCL_RST();        // ����ʱ����
		Delay_us(2);           // ��ʱ 2us
		SI2C_SCL_SET();        // ����ʱ����
        Receive <<= 1;         // ��������������һλ
		
        if(SI2C_SDA_GET() == 1)
		{
			Receive++;         // ����λΪ 1
		}
		
		Delay_us(1);           // ��ʱ 1 us
    }			
	
    if (!ack)
        SI2C_NACK();           // ����NACK
    else
        SI2C_ACK();            // ����ACK
	
    return Receive;            // ���ؽ��յ�����
}

/****************************************************************************************************
 * ������΢����ʱ����
 *
 * ������time��ʱ��
 *
 * ���أ���
****************************************************************************************************/

void Delay_us(u16 time)
{    
   u16 i = 0;  
   while(time--)
   {
      i = 1;          // �Լ�����
      while(i--);
   }
}

/* End Of File */