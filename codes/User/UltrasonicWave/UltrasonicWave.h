#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H

#include "stm32f10x.h"
#include "bsp_delay.h"
#include "bsp_usart1.h"
#include "bsp_GeneralTim.h"






#define	TRIG_PORT1     			  GPIOC		//TRIG       
#define	TRIG_PIN1      			  GPIO_Pin_0   //TRIG       

#define	TRIG_PORT2     			  GPIOC		//TRIG       
#define	TRIG_PIN2      			  GPIO_Pin_1   //TRIG     



#define RCC_APB2Periph_GPIOx  RCC_APB2Periph_GPIOC

#define AVER_NUM        2              //����������

extern int8_t  IT_TAG;

int getRange(void);
int minusDistance(int distance);
int addDistance(int distance);
void UltrasonicWave_Configuration(void);               //�Գ�����ģ���ʼ��
void UltrasonicWave(int* num);
void dealTIM_ICUserValueStructureData(TIM_ICUserValueTypeDef TIM_ICUserValueStructurex, int i);
#endif /* __UltrasonicWave_H */

