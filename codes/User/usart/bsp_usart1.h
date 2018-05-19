#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>


void USART1_Config(void);
void NVIC_Configuration(void);
int fputc(int ch, FILE *f);
int fgetc(FILE *f);
//void NVIC_Configuration_2(void);
//void USART2_Config(void);
void Deal_Data(void);
void USART2_Initialise( u32 bound );
#endif /* __USART1_H */
