#ifndef __KEY_H
#define	__KEY_H


#include "stm32f10x.h"


//按键
#define MODE_VOLUME     0
#define MODE_FREQUENCY 	1
#define MODE_DISTANCE   2

#define NO_RANK			0
#define LOW_RANK		1
#define MID_RANK		2
#define UP_RANK			3

typedef struct key{
	int current_mode;
	int key_rank[3];
	int max_mode;//3:0 1 2
	int max_rank;//4:0 1 2 3
	int min_mode;//0
	int min_rank;//0
}key_four;


#define zhongduan 0





//  引脚定义
#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY1_GPIO_PORT    GPIOA			   
#define    KEY1_GPIO_PIN		 GPIO_Pin_0

#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEY2_GPIO_PORT    GPIOB		   
#define    KEY2_GPIO_PIN		  GPIO_Pin_1

#define    KEY3_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEY3_GPIO_PORT    GPIOB			   
#define    KEY3_GPIO_PIN		 GPIO_Pin_2

#define    KEY4_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    KEY4_GPIO_PORT    GPIOD		   
#define    KEY4_GPIO_PIN		 GPIO_Pin_3

#define    KEY5_GPIO_CLK     RCC_APB2Periph_GPIOD
#define    KEY5_GPIO_PORT    GPIOD			   
#define    KEY5_GPIO_PIN		 GPIO_Pin_4


//引脚定义
#define KEY1_INT_GPIO_PORT         GPIOA
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_0
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource0
#define KEY1_INT_EXTI_LINE         EXTI_Line0
#define KEY1_INT_EXTI_IRQ          EXTI0_IRQn

#define KEY1_IRQHandler            EXTI0_IRQHandler



#define KEY2_INT_GPIO_PORT         GPIOB
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_1
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource1
#define KEY2_INT_EXTI_LINE         EXTI_Line1
#define KEY2_INT_EXTI_IRQ          EXTI1_IRQn

#define KEY2_IRQHandler            EXTI1_IRQHandler


#define KEY3_INT_GPIO_PORT         GPIOB
#define KEY3_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEY3_INT_GPIO_PIN          GPIO_Pin_2
#define KEY3_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY3_INT_EXTI_PINSOURCE    GPIO_PinSource2
#define KEY3_INT_EXTI_LINE         EXTI_Line2
#define KEY3_INT_EXTI_IRQ          EXTI2_IRQn


#define KEY3_IRQHandler            EXTI2_IRQHandler


#define KEY4_INT_GPIO_PORT         GPIOD
#define KEY4_INT_GPIO_CLK          (RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO)
#define KEY4_INT_GPIO_PIN          GPIO_Pin_3
#define KEY4_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOD
#define KEY4_INT_EXTI_PINSOURCE    GPIO_PinSource3
#define KEY4_INT_EXTI_LINE         EXTI_Line3
#define KEY4_INT_EXTI_IRQ          EXTI3_IRQn

#define KEY4_IRQHandler            EXTI3_IRQHandler

#define KEY5_INT_GPIO_PORT         GPIOD
#define KEY5_INT_GPIO_CLK          (RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO)
#define KEY5_INT_GPIO_PIN          GPIO_Pin_4
#define KEY5_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOD
#define KEY5_INT_EXTI_PINSOURCE    GPIO_PinSource4
#define KEY5_INT_EXTI_LINE         EXTI_Line4
#define KEY5_INT_EXTI_IRQ          EXTI4_IRQn

#define KEY5_IRQHandler            EXTI4_IRQHandler



#define KEY_NO 			0		//没有按键
#define KEY_MODE		1		//功能键 PB1
#define KEY_BACK		2		//返回键 PB2
#define KEY_ADD			3		//加号键 PD3
#define KEY_SUB			4		//减号键 PD4
#define KEY_SAFE		5		//安全键 PA0




 /** 按键按下标置宏
	*  按键按下为高电平，设置 KEY_ON=1， KEY_OFF=0
	*  若按键按下为低电平，把宏设置成KEY_ON=0 ，KEY_OFF=1 即可
	*/
#define KEY_ON	1
#define KEY_OFF	0

//#define KEY_ON	0
//#define KEY_OFF	1

void EXTI_Key_Config(void);
void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
#if zhongduan
#else
void KeyPolling(void);
#endif

#endif /* __KEY_H */

