/**
  ******************************************************************************
  * @file    mp3.c
  * @author  chzh
  * @version V1.0
  * @date    2018-05-19
  * @brief   mp3模块函数
  ******************************************************************************
  */ 
#include "mp3.h"

 /**
  * @brief  初始化控制MP3的IO
  * @param  无
  * @retval 无
  */
void MP3_GPIO_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启MP3相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( MP31_GPIO_CLK | MP32_GPIO_CLK | MP33_GPIO_CLK| MP34_GPIO_CLK, ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = MP31_GPIO_PIN;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(MP31_GPIO_PORT, &GPIO_InitStructure);	
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = MP32_GPIO_PIN;

		/*调用库函数，初始化GPIO*/
		GPIO_Init(MP32_GPIO_PORT, &GPIO_InitStructure);
		
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = MP33_GPIO_PIN;

		/*调用库函数，初始化GPIOF*/
		GPIO_Init(MP33_GPIO_PORT, &GPIO_InitStructure);
		
				/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = MP34_GPIO_PIN;

		/*调用库函数，初始化GPIOF*/
		GPIO_Init(MP34_GPIO_PORT, &GPIO_InitStructure);

		/* 关闭所有MP3灯	*/
		GPIO_SetBits(MP31_GPIO_PORT, MP31_GPIO_PIN);
		
		/* 关闭所有MP3灯	*/
		GPIO_SetBits(MP32_GPIO_PORT, MP32_GPIO_PIN);	 
    
    /* 关闭所有MP3灯	*/
		GPIO_SetBits(MP33_GPIO_PORT, MP33_GPIO_PIN);
		
		    /* 关闭所有MP3灯	*/
		GPIO_SetBits(MP34_GPIO_PORT, MP34_GPIO_PIN);
}

void delay(int x) //延时X毫秒
{
	int y,z;
	for(z=x;z>0;z--)
	for(y=110;y>0;y--);
}

/*
	1、0111
	2、1011
	3、0011
	4、1101
	5、0101
	6、1001
	7、0001
	8、1110
	9、0110
*/



int MP3Run(int num)
{	
	switch(num)
	{
		case 1:  //北方
			MP31_ON;
			delay(500);
			MP31_OFF;
		break;
		case 2:  //东北方
			MP32_ON;
			delay(500);
			MP32_OFF;
		break;
		case 3:	//东方
			MP31_ON;
			MP32_ON;
			delay(500);
			MP31_OFF;
			MP32_OFF;
		break;
		case 4:	//东南方
			MP33_ON;
			delay(500);
			MP33_OFF;
		break;
		case 5:	//南方
			MP31_ON;
			MP33_ON;
			delay(500);
			MP31_OFF;
		MP33_OFF;
		break;
		case 6:	//西北方
			MP33_ON;
			MP32_ON;
			delay(500);
			MP33_OFF;
			MP32_OFF;
		break;
		case 7:	//西方
			MP31_ON;
			MP32_ON;
			MP33_ON;
			delay(500);
			MP31_OFF;
			MP32_OFF;
			MP33_OFF
		break;
		case 8:	//西南方
			MP34_ON;
			delay(500);
			MP34_OFF;	
		break;
		case 9:	//前方有障碍物
			MP31_ON;
			MP34_ON;
			delay(500);
			MP31_OFF;
			MP34_OFF;
		break;
		
	}
	//1--北方
//	MP31_ON;
//	delay(500);
//	MP31_OFF;
	
	//2---东北方
//	MP32_ON;
//	delay(500);
//	MP32_OFF;
	
	//3--东方
//	MP31_ON;
//	MP32_ON;
//	delay(500);
//	MP31_OFF;
//	MP32_OFF;
//	
//	//4----东南方
//	MP33_ON;
//	delay(500);
//	MP33_OFF;
//	
//	//5--南方
//	MP31_ON;
//	MP33_ON;
//	delay(500);
//	MP31_OFF;
//	MP33_OFF;
//	
//	//6--西北方
//	MP33_ON;
//	MP32_ON;
//	delay(500);
//	MP33_OFF;
//	MP32_OFF;
//	
//	//7---西方
//		MP31_ON;
//	MP32_ON;
//	MP33_ON;
//	delay(500);
//	MP31_OFF;
//	MP32_OFF;
//	MP33_OFF
//	
//	//8--西南方
//	MP34_ON;
//	delay(500);
//	MP34_OFF;	
//	
//	//9--前方有障碍物
//	MP31_ON;
//	MP34_ON;
//	delay(500);
//	MP31_OFF;
//	MP34_OFF;
	


    
}

