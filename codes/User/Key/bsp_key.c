/**
  ******************************************************************************
  * @file    bsp_key.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   按键应用bsp（扫描模式）
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "./key/bsp_key.h"  
#include "./usart/bsp_usart1.h"
#include "./systick/bsp_SysTick.h"
#include "mp3.h"
#include "UltrasonicWave.h"


extern int flag_FALLING;
key_four key4;


#ifdef HHHHH
/**
  * @brief  配置按键用到的I/O口
  * @param  无
  * @retval 无
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*开启按键端口的时钟*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK|KEY3_GPIO_CLK|KEY4_GPIO_CLK|KEY5_GPIO_CLK,ENABLE);
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	// 设置按键的引脚为浮空输入
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 

//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//使用结构体初始化按键
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	//设置按键的引脚为浮空输入
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	//使用结构体初始化按键
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
	
		//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN; 
	//设置按键的引脚为浮空输入
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	//使用结构体初始化按键
	GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);	

	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN; 
	//设置按键的引脚为浮空输入
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	//使用结构体初始化按键
	GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);	

	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY5_GPIO_PIN; 
	//设置按键的引脚为浮空输入
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	//使用结构体初始化按键
	GPIO_Init(KEY5_GPIO_PORT, &GPIO_InitStructure);	
}

#define iTick 5000
static void delay(long i)
{
	for(;i>0;i--);
}

//delay(iTick);
//按下时输出高电平，松开时输出低电平
 /*
 * 函数名：Key_Scan
 * 描述  ：检测是否有按键按下
 * 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
 *		     GPIO_Pin：待读取的端口位 	
 * 输出  ：KEY_OFF(没按下按键)、KEY_ON（按下按键）
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/* 检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		delay(iTick);
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)
		{
			/* 等待按键释放 */
			while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON); 
				return 	KEY_ON;	 
		}
		else
			return KEY_OFF;			
	}
	else
		return KEY_OFF;
}

int KeyPolling(void)
{
	if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
	{
		printf("\n按下功能选择键");
		if(MODE_VOLUME == key4.current_mode)	//当前处于音量调节模式
		{
			printf("\n当前处于音量调节中,当前音量等级为：%d", key4.key_rank[MODE_VOLUME]);
			//播放：当前处于音量调节模式
		}
		else if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
		{
			printf("\n当前处于频率调节中，当前频率等级为：%d", key4.key_rank[MODE_FREQUENCY]);			
			//播放：当前处于频率调节模式
		}
		else if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
		{
			printf("\n当前处于距离调节中，当前距离等级为：%d", key4.key_rank[MODE_DISTANCE]);
			//播放：当前处于距离调节模式
		}
//		else
//		{
//			printf("\nKEY2 error!");
//		}

		return KEY_MODE;
	}		
	
	if( Key_Scan(KEY3_GPIO_PORT,KEY3_GPIO_PIN) == KEY_ON  )
	{
		printf("\n按下进入下一个模式键!");
		key4.current_mode=(++key4.current_mode)%key4.max_mode;

		return KEY_BACK;
	}		

	if( Key_Scan(KEY4_GPIO_PORT,KEY4_GPIO_PIN) == KEY_ON  )
	{
		printf("\n按下加号键");
		
		if(MODE_VOLUME == key4.current_mode)	//当前处于音量调节模式
		{
			if(key4.max_rank == key4.key_rank[MODE_VOLUME]+1)
			{
				printf("\n当前音量调节为等级：%d，达到最大等级不可再调大", key4.key_rank[MODE_VOLUME]);				

			}
			else
			{
				//音量增加函数调用
				key4.key_rank[MODE_VOLUME]=(++key4.key_rank[MODE_VOLUME])%key4.max_rank;					
				printf("\n当前音量调节为等级：%d", key4.key_rank[MODE_VOLUME]);
			}

		}
		else if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
		{
			if(key4.max_rank == key4.key_rank[MODE_FREQUENCY]+1)
			{
				printf("\n当前频率调节为等级：%d，达到最大等级不可再调大", key4.key_rank[MODE_FREQUENCY]);			
			}
			else
			{
				//频率增加函数调用
				key4.key_rank[MODE_FREQUENCY]=(++key4.key_rank[MODE_FREQUENCY])%key4.max_rank;					
				printf("\n当前频率调节为等级：%d", key4.key_rank[MODE_FREQUENCY]);
			}

		}
		else if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
		{
			if(key4.max_rank == key4.key_rank[MODE_DISTANCE]+1)
			{			
				printf("\n当前距离调节为等级：%d，达到最大等级不可再调大", key4.key_rank[MODE_DISTANCE]);
			}
			else
			{
				//距离增加函数调用
				key4.key_rank[MODE_DISTANCE]=(++key4.key_rank[MODE_DISTANCE])%key4.max_rank;					
				printf("\n当前距离调节为等级：%d", key4.key_rank[MODE_DISTANCE]);
			}
		}
//		else
//		{
//			printf("KEY4 error!");
//		}

		return KEY_ADD;
	}		

	if( Key_Scan(KEY5_GPIO_PORT,KEY5_GPIO_PIN) == KEY_ON  )
	{
		printf("\n按下减号键");
		
		if(MODE_VOLUME == key4.current_mode)	//当前处于音量调节模式
		{
			if(key4.min_rank == key4.key_rank[MODE_VOLUME])
			{
				printf("\n当前音量调节为等级：%d，达到最小等级不可再调小", key4.key_rank[MODE_VOLUME]);
			}
			else
			{
				//音量减少函数调用
				key4.key_rank[MODE_VOLUME]=(--key4.key_rank[MODE_VOLUME])%key4.max_rank;					
				printf("\n当前音量调节为等级：%d", key4.key_rank[MODE_VOLUME]);
			}				
		}
		else if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
		{
			if(key4.min_rank == key4.key_rank[MODE_FREQUENCY])
			{
				printf("\n当前频率调节为等级：%d，达到最小等级不可再调小", key4.key_rank[MODE_FREQUENCY]);
			}
			else
			{
				//频率减少函数调用
				key4.key_rank[MODE_FREQUENCY]=(--key4.key_rank[MODE_FREQUENCY])%key4.max_rank;					
				printf("\n当前频率调节为等级：%d", key4.key_rank[MODE_FREQUENCY]);
			}
		}
		else if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
		{
			if(key4.min_rank == key4.key_rank[MODE_DISTANCE])
			{
				printf("\n当前距离调节为等级：%d，达到最小等级不可再调小", key4.key_rank[MODE_DISTANCE]);			
			}
			else
			{
				//距离减少函数调用
				key4.key_rank[MODE_DISTANCE]=(--key4.key_rank[MODE_DISTANCE])%key4.max_rank;					
				printf("\n当前距离调节为等级：%d", key4.key_rank[MODE_DISTANCE]);
			}
		}
//		else
//		{
//			printf("KEY5 error!");
//		}
		
		return KEY_SUB;
	}		

	else
		return KEY_NO;
}

#endif

/**
  * @brief  按键功能模块
  * @param  无  
  * @retval 无
  */
//void key_module()
//{
//	int key_n;
//	key_n=KeyPolling();	//按键轮询
//	
//	switch(key_n)
//	{
///*******************************************************************************************************/
//		case KEY_MODE:	//模式选择
//			
//			if(MODE_VOLUME == key4.current_mode)	//当前处于音量调节模式
//			{
//				//播放：当前处于音量调节模式
//				break;
//			}
//			else if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
//			{
//				//播放：当前处于频率调节模式
//				break;
//			}
//			else if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
//			{
//				//播放：当前处于距离调节模式
//				break;
//			}
//			else
//				break;
///*******************************************************************************************************/
//			
//		case KEY_BACK:	//返回上一个模式/下一个模式
//			
//			key4.current_mode=(++key4.current_mode)%key4.max_mode;
//			break;
///*******************************************************************************************************/
//		
//		case KEY_ADD:	//加号

//			if(MODE_VOLUME == key4.current_mode)	//当前处于音量调节模式
//			{
//				if(key4.max_rank == key4.key_rank[MODE_VOLUME])
//				{
//				
//				}
//				else if(key4.min_rank == key4.key_rank[MODE_VOLUME])
//				{
//				
//				}
//				else
//				{
//					//音量增加函数调用
//					key4.key_rank[MODE_VOLUME]=(++key4.key_rank[MODE_VOLUME])%key4.max_rank;					
//				}

//			}
//			else if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
//			{
//				if(key4.max_rank == key4.key_rank[MODE_FREQUENCY])
//				{
//				
//				}
//				else if(key4.min_rank == key4.key_rank[MODE_FREQUENCY])
//				{
//				
//				}
//				else
//				{
//					//频率增加函数调用
//					key4.key_rank[MODE_FREQUENCY]=(++key4.key_rank[MODE_FREQUENCY])%key4.max_rank;					
//				}

//			}
//			else if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
//			{
//				if(key4.max_rank == key4.key_rank[MODE_DISTANCE])
//				{
//				
//				}
//				else if(key4.min_rank == key4.key_rank[MODE_DISTANCE])
//				{
//				
//				}
//				else
//				{
//					//距离增加函数调用
//					key4.key_rank[MODE_DISTANCE]=(++key4.key_rank[MODE_DISTANCE])%key4.max_rank;					
//				}
//			}
//			else
//				break;
///*******************************************************************************************************/
//			
//		case KEY_SUB:	//减号
//			
//			if(MODE_VOLUME == key4.current_mode)	//当前处于音量调节模式
//			{
//				if(key4.max_rank == key4.key_rank[MODE_VOLUME])
//				{
//				
//				}
//				else if(key4.min_rank == key4.key_rank[MODE_VOLUME])
//				{
//				
//				}
//				else
//				{
//					//音量减少函数调用
//					key4.key_rank[MODE_VOLUME]=(--key4.key_rank[MODE_VOLUME])%key4.max_rank;					
//				}				
//			}
//			else if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
//			{
//				if(key4.max_rank == key4.key_rank[MODE_FREQUENCY])
//				{
//				
//				}
//				else if(key4.min_rank == key4.key_rank[MODE_FREQUENCY])
//				{
//				
//				}
//				else
//				{
//					//频率减少函数调用
//					key4.key_rank[MODE_FREQUENCY]=(--key4.key_rank[MODE_FREQUENCY])%key4.max_rank;					
//				}
//			}
//			else if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
//			{
//				if(key4.max_rank == key4.key_rank[MODE_DISTANCE])
//				{
//				
//				}
//				else if(key4.min_rank == key4.key_rank[MODE_DISTANCE])
//				{
//				
//				}
//				else
//				{
//					//距离减少函数调用
//					key4.key_rank[MODE_DISTANCE]=(--key4.key_rank[MODE_DISTANCE])%key4.max_rank;					
//				}
//			}
//			else
//				break;
///*******************************************************************************************************/

//		case KEY_SAFE:	//安全
//			break;
///*******************************************************************************************************/
//		
//		default:
//			break;
//	}
//	
//}





 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration2(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 配置NVIC为优先级组1 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源：按键1 */
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	  /* 配置中断源：按键2，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);

	  /* 配置中断源：按键3，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel = KEY3_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);

  /* 配置中断源：按键4，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel = KEY4_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);

  /* 配置中断源：按键5，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel = KEY5_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);

}

 /**
  * @brief  配置 IO为EXTI中断口，并设置中断优先级
  * @param  无
  * @retval 无
  */
void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/*开启按键GPIO口的时钟*/
	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK|KEY2_INT_GPIO_CLK|KEY3_INT_GPIO_CLK|KEY4_INT_GPIO_CLK|KEY5_INT_GPIO_CLK,ENABLE);
												
	/* 配置 NVIC 中断*/
	NVIC_Configuration2();
	  
/*--------------------------KEY1配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
  /*--------------------------KEY2配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	  /*--------------------------KEY3配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY3_INT_GPIO_PIN;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEY3_INT_EXTI_PORTSOURCE, KEY3_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY3_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	  /*--------------------------KEY4配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY4_INT_GPIO_PIN;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY4_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEY4_INT_EXTI_PORTSOURCE, KEY4_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY4_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	
	  /*--------------------------KEY5配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY5_INT_GPIO_PIN;
  /* 配置为浮空输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY5_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEY5_INT_EXTI_PORTSOURCE, KEY5_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY5_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KEY1 1
#define KEY2 2
#define KEY3 3
#define KEY4 4
#define KEY5 5


static void EXTI_n(int key_n)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	if(KEY1==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
		/* 关闭中断 */	
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);
	}
	else if(KEY2==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
		/* 关闭中断 */	
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else if(KEY3==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY3_INT_EXTI_LINE;
		/* 关闭中断 */	
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else if(KEY4==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY4_INT_EXTI_LINE;
		/* 关闭中断 */	
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else if(KEY5==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY5_INT_EXTI_LINE;
		/* 关闭中断 */	
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else
		;
		
}

static void EXTI_n_Open(int key_n)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	if(KEY1==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
		/* 关闭中断 */	
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
	}
	else if(KEY2==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
		/* 关闭中断 */	
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else if(KEY3==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY3_INT_EXTI_LINE;
		/* 关闭中断 */	
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else if(KEY4==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY4_INT_EXTI_LINE;
		/* 关闭中断 */	
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else if(KEY5==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY5_INT_EXTI_LINE;
		/* 关闭中断 */	
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else
		;
		
}

#define iCOUNT 1
#define jCOUNT 100
static void delay(int i, int j)
{
	int yj=j;
	while(i--)
	{
		j=yj;
		while(j--);
	}
}
/*
	四个中断的printf()函数的中文，注意是中文语句，都需要语音输出提示
*/

#ifdef xuyaocxkaiqi

void KEY1_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
//		EXTI_n(KEY1);
		printf("\n按下安全键!");
		flag_FALLING=0;	//盲人安全
    //清除中断标志位
//		delay(iCOUNT, jCOUNT);
//		EXTI_n_Open(KEY1);
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE); 
	}  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void KEY2_IRQHandler(void)	//查询当前状态
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET) 
	{
//    EXTI_n(KEY2);
		printf("\n按下功能选择键");
		if(MODE_VOLUME == key4.current_mode)	//当前处于音量调节模式
		{
			printf("\n当前处于音量调节中,当前音量等级为：%d", key4.key_rank[MODE_VOLUME]);
			//播放：当前处于音量调节模式
		}
		else if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
		{
			printf("\n当前处于频率调节中，当前频率等级为：%d", key4.key_rank[MODE_FREQUENCY]);			
			//播放：当前处于频率调节模式
		}
		else if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
		{
			printf("\n当前处于距离调节中，当前距离等级为：%d", key4.key_rank[MODE_DISTANCE]);
			//播放：当前处于距离调节模式
		}
		else
		{
			printf("\nKEY2_IRQHandler error!");
		}
		
		//清除中断标志位
//		delay(iCOUNT, jCOUNT);
//		EXTI_n_Open(KEY2);
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void KEY3_IRQHandler(void)	//下一个模式
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY3_INT_EXTI_LINE) != RESET) 
	{
//		EXTI_n(KEY3);
		printf("\n按下进入下一个模式键!");
		key4.current_mode=(++key4.current_mode)%key4.max_mode;
		//清除中断标志位
//		delay(iCOUNT, jCOUNT);
		EXTI_ClearITPendingBit(KEY3_INT_EXTI_LINE);     
//		EXTI_n_Open(KEY3);
	}  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void KEY4_IRQHandler(void)	//加号键
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY4_INT_EXTI_LINE) != RESET) 
	{
//    EXTI_n(KEY4);
		printf("\n按下加号键");
		
		if(MODE_VOLUME == key4.current_mode)	//当前处于音量调节模式
		{
			if(key4.max_rank == key4.key_rank[MODE_VOLUME]+1)
			{
				printf("\n当前音量调节为等级：%d，达到最大等级不可再调大", key4.key_rank[MODE_VOLUME]);				

			}
			else
			{
				//音量增加函数调用
				AddVolume();
				key4.key_rank[MODE_VOLUME]=(++key4.key_rank[MODE_VOLUME])%key4.max_rank;					
				printf("\n当前音量调节为等级：%d", key4.key_rank[MODE_VOLUME]);
			}

		}
		else if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
		{
			if(key4.max_rank == key4.key_rank[MODE_FREQUENCY]+1)
			{
				printf("\n当前频率调节为等级：%d，达到最大等级不可再调大", key4.key_rank[MODE_FREQUENCY]);			
			}
			else
			{
				//频率增加函数调用
				AddRate();
				key4.key_rank[MODE_FREQUENCY]=(++key4.key_rank[MODE_FREQUENCY])%key4.max_rank;					
				printf("\n当前频率调节为等级：%d", key4.key_rank[MODE_FREQUENCY]);
			}

		}
		else if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
		{
			if(key4.max_rank == key4.key_rank[MODE_DISTANCE]+1)
			{			
				printf("\n当前距离调节为等级：%d，达到最大等级不可再调大", key4.key_rank[MODE_DISTANCE]);
			}
			else
			{
				//距离增加函数调用
				addDistance();
				key4.key_rank[MODE_DISTANCE]=(++key4.key_rank[MODE_DISTANCE])%key4.max_rank;					
				printf("\n当前距离调节为等级：%d", key4.key_rank[MODE_DISTANCE]);
			}
		}
		else
		{
			printf("KEY4_IRQHandler error!");
		}
		
		//清除中断标志位
//		delay(iCOUNT, jCOUNT);
		EXTI_ClearITPendingBit(KEY4_INT_EXTI_LINE);     
//		EXTI_n_Open(KEY4);
	}  
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void KEY5_IRQHandler(void)	//减号键
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY5_INT_EXTI_LINE) != RESET) 
	{
//    EXTI_n(KEY5);
		printf("\n按下减号键");
		
		if(MODE_VOLUME == key4.current_mode)	//当前处于音量调节模式
		{
			if(key4.min_rank == key4.key_rank[MODE_VOLUME])
			{
				printf("\n当前音量调节为等级：%d，达到最小等级不可再调小", key4.key_rank[MODE_VOLUME]);
			}
			else
			{
				//音量减少函数调用
				SubVolume();
				key4.key_rank[MODE_VOLUME]=(--key4.key_rank[MODE_VOLUME])%key4.max_rank;					
				printf("\n当前音量调节为等级：%d", key4.key_rank[MODE_VOLUME]);
			}				
		}
		else if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
		{
			if(key4.min_rank == key4.key_rank[MODE_FREQUENCY])
			{
				printf("\n当前频率调节为等级：%d，达到最小等级不可再调小", key4.key_rank[MODE_FREQUENCY]);
			}
			else
			{
				//频率减少函数调用
				SubRate();
				key4.key_rank[MODE_FREQUENCY]=(--key4.key_rank[MODE_FREQUENCY])%key4.max_rank;					
				printf("\n当前频率调节为等级：%d", key4.key_rank[MODE_FREQUENCY]);
			}
		}
		else if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
		{
			if(key4.min_rank == key4.key_rank[MODE_DISTANCE])
			{
				printf("\n当前距离调节为等级：%d，达到最小等级不可再调小", key4.key_rank[MODE_DISTANCE]);			
			}
			else
			{
				//距离减少函数调用
				minusDistance();
				key4.key_rank[MODE_DISTANCE]=(--key4.key_rank[MODE_DISTANCE])%key4.max_rank;					
				printf("\n当前距离调节为等级：%d", key4.key_rank[MODE_DISTANCE]);
			}
		}
		else
		{
			printf("KEY5_IRQHandler error!");
		}
		
		//清除中断标志位
//		delay(iCOUNT, jCOUNT);
		EXTI_ClearITPendingBit(KEY5_INT_EXTI_LINE);     
//		EXTI_n_Open(KEY5);
	}  
}

#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////


































///////////////////////////////////////////////////////////////////////////////




/*********************************************END OF FILE**********************/


