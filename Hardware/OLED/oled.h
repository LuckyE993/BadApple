 /********************************************************************************
   * ����Ӳ������������ɽ�ɿ�����GD32F470ZGT6    ʹ����Ƶ200Mhz    ����25Mhz
   * �� �� ��: V1.0
   * �޸�����: LC
   * �޸�����: 2023��06��12��
   * ���ܽ���:      
   ******************************************************************************
   * ��ɽ����Ӳ�������������չ����Ӳ�����Ϲ���ȫ����Դ  
   * �����������www.lckfb.com   
   * ����֧�ֳ�פ��̳���κμ������⻶ӭ��ʱ����ѧϰ  
   * ������̳��club.szlcsc.com   
   * ����ģ����ֲ�ֲ᣺����������ɽ�ɿ����塿ģ����ֲ�ֲ�
   * ��עbilibili�˺ţ������������塿���������ǵ����¶�̬��
   * ��������׬Ǯ���������й�����ʦΪ����
 *********************************************************************************/

#ifndef __OLED_H
#define __OLED_H 

#include "gd32f4xx.h"
#include "stdlib.h"	
#ifndef u8
#define u8 uint8_t
#endif

#ifndef u16
#define u16 uint16_t
#endif

#ifndef u32
#define u32 uint32_t
#endif
//-----------------����LED�˿ڶ���---------------- 

//#define LED_ON GPIO_ResetBits(GPIOC,GPIO_Pin_12)
//#define LED_OFF GPIO_SetBits(GPIOC,GPIO_Pin_12)

#define USE_SOFTWARE 0      //�Ƿ�ʹ�����SPI  0ʹ��Ӳ��SPI  1ʹ�����SPI 

//-----------------OLED�˿���ֲ---------------- 
//VCC - 3.3V
//D0 - PB13 SPI1_SCK
//D1 - PB15 SPI1_MOSI
//RES - PD0�����Խ��븴λ��
//DC  - PC6
//CS  - PB12        SPI1_NSS
#define RCU_LCD_D0     RCU_GPIOB//SCK
#define PORT_LCD_D0    GPIOB
#define GPIO_LCD_D0    GPIO_PIN_13

#define RCU_LCD_D1     RCU_GPIOB//MOSI
#define PORT_LCD_D1    GPIOB
#define GPIO_LCD_D1    GPIO_PIN_15

#define RCU_LCD_CS      RCU_GPIOB//NSS
#define PORT_LCD_CS     GPIOB
#define GPIO_LCD_CS     GPIO_PIN_12

#define RCU_LCD_DC      RCU_GPIOC //DC
#define PORT_LCD_DC     GPIOC
#define GPIO_LCD_DC     GPIO_PIN_6

#define RCU_LCD_RES     RCU_GPIOD//RES
#define PORT_LCD_RES    GPIOD
#define GPIO_LCD_RES    GPIO_PIN_0

#define RCU_SPI_HARDWARE RCU_SPI1
#define PORT_SPI         SPI1
#define LINE_AF_SPI      GPIO_AF_5

//-----------------OLED�˿ڶ���---------------- 

#define OLED_SCL_Clr() gpio_bit_write(PORT_LCD_D0, GPIO_LCD_D0, RESET)//SCL=SCLK
#define OLED_SCL_Set() gpio_bit_write(PORT_LCD_D0, GPIO_LCD_D0, SET)

#define OLED_SDA_Clr() gpio_bit_write(PORT_LCD_D1, GPIO_LCD_D1, RESET)//SDA=MOSI
#define OLED_SDA_Set() gpio_bit_write(PORT_LCD_D1, GPIO_LCD_D1, SET)

#define OLED_RES_Clr() gpio_bit_write(PORT_LCD_RES, GPIO_LCD_RES, RESET)//RES
#define OLED_RES_Set() gpio_bit_write(PORT_LCD_RES,GPIO_LCD_RES, SET)

#define OLED_DC_Clr()  gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, RESET)//DC
#define OLED_DC_Set()  gpio_bit_write(PORT_LCD_DC, GPIO_LCD_DC, SET)
                       
#define OLED_CS_Clr()  gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, RESET)//CS
#define OLED_CS_Set()  gpio_bit_write(PORT_LCD_CS, GPIO_LCD_CS, SET)


#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

void OLED_ClearPoint(u8 x,u8 y);//����
void OLED_ColorTurn(u8 i);//����
void OLED_DisplayTurn(u8 i);//��Ļ��ת
void OLED_WR_Byte(u8 dat,u8 mode);//����д������
void OLED_DisPlay_On(void);//��ʾ��
void OLED_DisPlay_Off(void);//��ʾ��
void OLED_Refresh(void);//������Ļ������ʾ
void OLED_Clear(void);//����
void OLED_DrawPoint(u8 x,u8 y,u8 t);//����
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);//����
void OLED_DrawCircle(u8 x,u8 y,u8 r);//��Բ
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);//��ʾ�����ַ�
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);//��ʾ�ַ���
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);//��ʾ����
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);//��ʾ����
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);//�ַ�������
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);//��ʾͼƬ
void OLED_Init(void);//OLED��ʼ��

#endif







