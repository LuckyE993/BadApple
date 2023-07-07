#ifndef _BSP_USART_H_
#define _BSP_USART_H_
#include "gd32f4xx.h"
#include "stdio.h"
#define USART_RECEIVE_LENGTH 4096

/*
    ����ʹ��DMA�ж�
    ����ʹ��USART�ж�
*/
#define USART_DMA_INT_Control 1

void USART_Config(void);

int fputc(int ch, FILE *f);
int fgetc(FILE *f);
#endif /*_BSP_USART_H_*/

