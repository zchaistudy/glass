#ifndef __KEY_H
#define	__KEY_H
#include "stm32f10x.h"

//短一点的，按键模块平时输出低电平
//按下时输出高电平

//长一点的，相反，松开输出高电平
//按下输出低电平

#define KEY_DOWN 1	//按键按下表示高电平
#define KEY_UP 0		//按键松开表示低电平

#define iCOUNT 10000	//中断的延时
#define iTick 10	//轮询的延时

#define BREAK_EXTI_OPEN 0	//1为打开中断的控制方式，0为打开轮询的控制方式


//调试
#define DEBUG_ON_OFF_CYM 0
#define p_debug_cym(restrict,args...)  do{if(DEBUG_ON_OFF_CYM){printf(restrict,##args);}}while(0)
#define p_err_cym(restrict,args...)  do{if(DEBUG_ON_OFF_CYM){printf("\nfile:%s\nfunction:%s\t\tline:%d:\n",__FILE__,__func__,__LINE__);printf(restrict,##args);}}while(0)


//--------------------------------------------------------------------------------
//按键的三种模式
#define MODE_VOLUME     0	//音量模式
#define MODE_FREQUENCY 	1 //频率模式
#define MODE_SHAKE   2 //震动模式
#define MODE_DISTANCE 3  //距离模式

//按键的四种状态等级
#define NO_RANK			0	//0等级
#define LOW_RANK		1	//低等级
#define MID_RANK		2 //中等级
#define UP_RANK			3 //高等级

#define MAX_MODE 3      //两种模式 音频模式  语音模式

//设置参数
#define SET_CLOSE       0//关闭设置参数
#define SET_VOLUME      1//音量设置参数
#define SET_FREQUENCY 	2 //频率设置参数
#define SET_DISTANCE    3 //距离设置参数
#define SET_SHAKE       4 //震动设置参数
#define SET_MAX         5  //5种设置参数


//参数最大值
#define MAX_RANK_VOLUME       15     //最大音量等级
#define MAX_RANK_FREQUENCY    3      //最大频率等级
#define MAX_RANK_DISTANCE     6      //最大距离等级
#define MAX_RANK_SHAKE        3      //最大震动等级

#define MIN_RANK        1      //最大距离等级

typedef struct key{
	int current_mode;//当前模式
	int key_rank[SET_MAX];//每个模式的等级
	int max_mode;//最大模式是2。0 1 2   MODE_VOLUME MODE_FREQUENCY MODE_DISTANCE
	int max_rank;//最大等级是4。0 1 2 3 NO_RANK LOW_RANK MID_RANK UP_RANK
	int min_mode;//最小模式是0
	int min_rank;//最小等级是0
	uint8_t set_parameter; //当前设置参数
}key_four;





#if BREAK_EXTI_OPEN

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


void EXTI_Key_Config(void);
	
#else

#define KEY1_INT_GPIO_PORT         GPIOA
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_0
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource0
#define KEY1_INT_EXTI_LINE         EXTI_Line0
#define KEY1_INT_EXTI_IRQ          EXTI0_IRQn

#define KEY1_IRQHandler            EXTI0_IRQHandler


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

#define    KEY4_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEY4_GPIO_PORT    GPIOB
#define    KEY4_GPIO_PIN		 GPIO_Pin_0

#define    KEY5_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY5_GPIO_PORT    GPIOA
#define    KEY5_GPIO_PIN		 GPIO_Pin_1




#define TRUE 1
#define FALSE 0

void EXTI_Key_Config(void);

void Key_GPIO_Config(void);
void KeyPolling(void);
	

#endif




#endif /* __KEY_H */

