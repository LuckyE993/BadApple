 /********************************************************************************
   * 测试硬件：立创·梁山派开发板GD32F470ZGT6    使用主频200Mhz    晶振25Mhz
   * 版 本 号: V1.0
   * 修改作者: LC
   * 修改日期: 2023年06月12日
   * 功能介绍:      
   ******************************************************************************
   * 梁山派软硬件资料与相关扩展板软硬件资料官网全部开源  
   * 开发板官网：www.lckfb.com   
   * 技术支持常驻论坛，任何技术问题欢迎随时交流学习  
   * 立创论坛：club.szlcsc.com   
   * 其余模块移植手册：【立创·梁山派开发板】模块移植手册
   * 关注bilibili账号：【立创开发板】，掌握我们的最新动态！
   * 不靠卖板赚钱，以培养中国工程师为己任
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
//-----------------测试LED端口定义---------------- 

//#define LED_ON GPIO_ResetBits(GPIOC,GPIO_Pin_12)
//#define LED_OFF GPIO_SetBits(GPIOC,GPIO_Pin_12)

#define USE_SOFTWARE 0      //是否使用软件SPI  0使用硬件SPI  1使用软件SPI 

//-----------------OLED端口移植---------------- 
//VCC - 3.3V
//D0 - PB13 SPI1_SCK
//D1 - PB15 SPI1_MOSI
//RES - PD0（可以接入复位）
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

//-----------------OLED端口定义---------------- 

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


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void OLED_ClearPoint(u8 x,u8 y);//画点
void OLED_ColorTurn(u8 i);//反显
void OLED_DisplayTurn(u8 i);//屏幕翻转
void OLED_WR_Byte(u8 dat,u8 mode);//串行写入数据
void OLED_DisPlay_On(void);//显示开
void OLED_DisPlay_Off(void);//显示关
void OLED_Refresh(void);//更新屏幕内容显示
void OLED_Clear(void);//清屏
void OLED_DrawPoint(u8 x,u8 y,u8 t);//画点
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);//画线
void OLED_DrawCircle(u8 x,u8 y,u8 r);//画圆
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);//显示单个字符
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);//显示字符串
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);//显示数字
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);//显示中文
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode);//字符串左移
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);//显示图片
void OLED_Init(void);//OLED初始化

#endif







