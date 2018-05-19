#ifndef __MP3_H
#define	__MP3_H


#include "stm32f10x.h"


/* 定义音频模块连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的MP3引脚 */

#define MP31_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define MP31_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define MP31_GPIO_PIN		GPIO_Pin_8			        /* 连接到SCL时钟线的GPIO */


#define MP32_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define MP32_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define MP32_GPIO_PIN		GPIO_Pin_5			        /* 连接到SCL时钟线的GPIO */


#define MP33_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define MP33_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define MP33_GPIO_PIN		GPIO_Pin_6			        /* 连接到SCL时钟线的GPIO */

#define MP34_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define MP34_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define MP34_GPIO_PIN			GPIO_Pin_7			        /* 连接到SCL时钟线的GPIO */


/** the macro definition to trigger the MP3 on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平
#define digitalToggle(p,i) {p->ODR ^=i;} //输出反转状态


/* 定义控制IO的宏 */
#define MP31_TOGGLE		 digitalToggle(MP31_GPIO_PORT,MP31_GPIO_PIN)
#define MP31_OFF		   digitalHi(MP31_GPIO_PORT,MP31_GPIO_PIN)
#define MP31_ON			   digitalLo(MP31_GPIO_PORT,MP31_GPIO_PIN)

#define MP32_TOGGLE		 digitalToggle(MP32_GPIO_PORT,MP32_GPIO_PIN)
#define MP32_OFF		   digitalHi(MP32_GPIO_PORT,MP32_GPIO_PIN)
#define MP32_ON			   digitalLo(MP32_GPIO_PORT,MP32_GPIO_PIN)

#define MP33_TOGGLE		 digitalToggle(MP33_GPIO_PORT,MP33_GPIO_PIN)
#define MP33_OFF		   digitalHi(MP33_GPIO_PORT,MP33_GPIO_PIN)
#define MP33_ON			   digitalLo(MP33_GPIO_PORT,MP33_GPIO_PIN)

#define MP34_OFF		   digitalHi(MP34_GPIO_PORT,MP34_GPIO_PIN)
#define MP34_ON			   digitalLo(MP34_GPIO_PORT,MP34_GPIO_PIN)


void MP3_GPIO_Config(void);
int MP3Run(int num);
#endif /* __MP3_H */
