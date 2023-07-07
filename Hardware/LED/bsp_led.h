#ifndef _BSP_LED_H_
#define _BSP_LED_H_
#include "gd32f4xx.h"

#define LED1_GPIO GPIOE
#define LED1_PIN GPIO_PIN_3
#define LED1_RCU_CLK RCU_GPIOE

#define LED2_GPIO GPIOD
#define LED2_PIN GPIO_PIN_7
#define LED2_RCU_CLK RCU_GPIOD

#define LED3_GPIO GPIOG
#define LED3_PIN GPIO_PIN_3
#define LED3_RCU_CLK RCU_GPIOG

#define LED4_GPIO GPIOA
#define LED4_PIN GPIO_PIN_5
#define LED4_RCU_CLK RCU_GPIOA

#define GPIOA_OCTL_ADDR (GPIOA + 0x14)
#define GPIOD_OCTL_ADDR (GPIOD + 0x14)
#define GPIOE_OCTL_ADDR (GPIOE + 0x14)
#define GPIOG_OCTL_ADDR (GPIOG + 0x14)

#define ON 1
#define OFF 0

//SRAM 位带区:    0X2000 0000~0X200F 0000
//SRAM 位带别名区:0X2200 0000~0X23FF FFFF

//外设 位带区:    0X4000 0000~0X400F FFFF
//外设 位带别名区:0X4200 0000~0X43FF FFFF

// 把“位带地址+位序号”转换成别名地址的宏
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x02000000+((addr & 0x000FFFFF)<<5)+(bitnum<<2)) 

// 把一个地址转换成一个指针
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))

// 把位带别名区地址转换成指针
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

typedef enum 
{
	led1_on,
	led2_on,
	led3_on,
	led4_on
} Ledx_ON;

#define PEout(n) BIT_ADDR(GPIOE_OCTL_ADDR, n)
#define PAout(n) BIT_ADDR(GPIOA_OCTL_ADDR, n)
#define PDout(n) BIT_ADDR(GPIOD_OCTL_ADDR, n)
#define PGout(n) BIT_ADDR(GPIOG_OCTL_ADDR, n)

#define LED1(a)	if (a)	\
					PEout(3) = 1;\
					else		\
					PEout(3) = 0

#define LED2(a)	if (a)	\
					PDout(7) = 1;\
					else		\
					PDout(7) = 0
#define LED3(a)	if (a)	\
					PGout(3) = 1;\
					else		\
					PGout(3) = 0
#define LED4(a)	if (a)	\
					PAout(5) = 1;\
					else		\
					PAout(5) = 0   

#define LED1_TOOGLE gpio_bit_toggle(LED1_GPIO,LED1_PIN)
#define LED2_TOOGLE gpio_bit_toggle(LED2_GPIO,LED2_PIN)
#define LED3_TOOGLE gpio_bit_toggle(LED3_GPIO,LED3_PIN)
#define LED4_TOOGLE gpio_bit_toggle(LED4_GPIO,LED4_PIN)

void LED_Init(void);                                  
#endif /*_BSP_LED_H_*/


