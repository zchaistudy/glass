
#include "./key/bsp_key.h"  
#include "./usart/bsp_usart1.h"
#include "./systick/bsp_SysTick.h"
#include "mp3.h"
#include "UltrasonicWave.h"

extern int flag_FALLING;
key_four key4 = {0, {0, 0, 0}, 3, 4, 0, 0};

static void delay(int i)
{
	for(;i>0;i--);
}



#if BREAK_EXTI_OPEN

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


/*
	四个中断的printf()函数的中文，注意是中文语句，都需要语音输出提示
*/


void KEY1_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
//		EXTI_n(KEY1);
		printf("\n按下安全键!");
		if(0 == flag_FALLING)
		{
				USART_SendData(USART1, '2');		//发送一般求助信息
				while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
					continue;		
				
				printf("\t盲人不是摔倒状态，只是发送一般性的求助信息");
		}
		else
			flag_FALLING=0;	//盲人安全
    //清除中断标志位
		delay(iCOUNT);
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
			printf("\t当前处于音量调节中,当前音量等级为：%d", key4.key_rank[MODE_VOLUME]);
			//播放：当前处于音量调节模式
		}
		else if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
		{
			printf("\t当前处于频率调节中，当前频率等级为：%d", key4.key_rank[MODE_FREQUENCY]);			
			//播放：当前处于频率调节模式
		}
		else if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
		{
			printf("\t当前处于距离调节中，当前距离等级为：%d", key4.key_rank[MODE_DISTANCE]);
			//播放：当前处于距离调节模式
		}
		else
		{
			printf("\nKEY2_IRQHandler error!");
		}
		
		//清除中断标志位
		delay(iCOUNT);
//		EXTI_n_Open(KEY2);
		EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);     
	}  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
#define MODE_VOLUME     0
#define MODE_FREQUENCY 	1
#define MODE_DISTANCE   2
*/


void KEY3_IRQHandler(void)	//下一个模式
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY3_INT_EXTI_LINE) != RESET) 
	{
//		EXTI_n(KEY3);
		printf("\n按下进入下一个模式键!");
		key4.current_mode=(++key4.current_mode)%key4.max_mode;
		if(MODE_VOLUME == key4.current_mode)
		{
			printf("\t当前模式调整为：%s", "音量模式");
		}
		if(MODE_FREQUENCY == key4.current_mode)
		{
			printf("\t当前模式调整为：%s", "频率模式");
		}
		if(MODE_DISTANCE == key4.current_mode)
		{
			printf("\t当前模式调整为：%s", "距离模式");
		}
		//清除中断标志位
		delay(iCOUNT);
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
				printf("\t当前音量调节为等级：%d，达到最大等级不可再调大", key4.key_rank[MODE_VOLUME]);				

			}
			else
			{
				//音量增加函数调用
				AddVolume();
				key4.key_rank[MODE_VOLUME]=(++key4.key_rank[MODE_VOLUME])%key4.max_rank;					
				printf("\t当前音量调节为等级：%d", key4.key_rank[MODE_VOLUME]);
			}

		}
		else if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
		{
			if(key4.max_rank == key4.key_rank[MODE_FREQUENCY]+1)
			{
				printf("\t当前频率调节为等级：%d，达到最大等级不可再调大", key4.key_rank[MODE_FREQUENCY]);			
			}
			else
			{
				//频率增加函数调用
				AddRate();
				key4.key_rank[MODE_FREQUENCY]=(++key4.key_rank[MODE_FREQUENCY])%key4.max_rank;					
				printf("\t当前频率调节为等级：%d", key4.key_rank[MODE_FREQUENCY]);
			}

		}
		else if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
		{
			if(key4.max_rank == key4.key_rank[MODE_DISTANCE]+1)
			{			
				printf("\t当前距离调节为等级：%d，达到最大等级不可再调大", key4.key_rank[MODE_DISTANCE]);
			}
			else
			{
				//距离增加函数调用
				addDistance();
				key4.key_rank[MODE_DISTANCE]=(++key4.key_rank[MODE_DISTANCE])%key4.max_rank;					
				printf("\t当前距离调节为等级：%d", key4.key_rank[MODE_DISTANCE]);
			}
		}
		else
		{
			printf("KEY4_IRQHandler error!");
		}
		
		//清除中断标志位
		delay(iCOUNT);
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
				printf("\t当前音量调节为等级：%d，达到最小等级不可再调小", key4.key_rank[MODE_VOLUME]);
			}
			else
			{
				//音量减少函数调用
				SubVolume();
				key4.key_rank[MODE_VOLUME]=(--key4.key_rank[MODE_VOLUME])%key4.max_rank;					
				printf("\t当前音量调节为等级：%d", key4.key_rank[MODE_VOLUME]);
			}				
		}
		else if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
		{
			if(key4.min_rank == key4.key_rank[MODE_FREQUENCY])
			{
				printf("\t当前频率调节为等级：%d，达到最小等级不可再调小", key4.key_rank[MODE_FREQUENCY]);
			}
			else
			{
				//频率减少函数调用
				SubRate();
				key4.key_rank[MODE_FREQUENCY]=(--key4.key_rank[MODE_FREQUENCY])%key4.max_rank;					
				printf("\t当前频率调节为等级：%d", key4.key_rank[MODE_FREQUENCY]);
			}
		}
		else if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
		{
			if(key4.min_rank == key4.key_rank[MODE_DISTANCE])
			{
				printf("\t当前距离调节为等级：%d，达到最小等级不可再调小", key4.key_rank[MODE_DISTANCE]);			
			}
			else
			{
				//距离减少函数调用
				minusDistance();
				key4.key_rank[MODE_DISTANCE]=(--key4.key_rank[MODE_DISTANCE])%key4.max_rank;					
				printf("\t当前距离调节为等级：%d", key4.key_rank[MODE_DISTANCE]);
			}
		}
		else
		{
			printf("KEY5_IRQHandler error!");
		}
		
		//清除中断标志位
		delay(iCOUNT);
		EXTI_ClearITPendingBit(KEY5_INT_EXTI_LINE);     
//		EXTI_n_Open(KEY5);
	}  
}




#else

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
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//设置引脚输出最大速率为50MHz

	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 					//K1
	// 设置按键的引脚为浮空输入
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//使用结构体初始化按键
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 					//K2
	//设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//使用结构体初始化按键
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
	
		//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN; 					//K3
	//设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//使用结构体初始化按键
	GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);	

	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN; 					//K4
	//设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//使用结构体初始化按键
	GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);	

	//选择按键的引脚
	GPIO_InitStructure.GPIO_Pin = KEY5_GPIO_PIN; 					//K5
	//设置按键的引脚为浮空输入
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//使用结构体初始化按键
	GPIO_Init(KEY5_GPIO_PORT, &GPIO_InitStructure);	
}


//按下时输出高电平，松开时输出低电平
 /*
 * 函数名：Key_Scan
 * 描述  ：检测是否有按键按下
 * 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
 *		     GPIO_Pin：待读取的端口位 	
 * 输出  ：KEY_OFF(没按下按键)、KEY_ON（按下按键）
 */
static uint8_t Key_Scan_down(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/* 检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_DOWN )  
	{	 
			delay(10);	
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_DOWN )  
			{
					while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_DOWN)	//等待按键放开
							continue; 
					
					return TRUE;	 
			}
			else
			{
					return FALSE;
			}
	}
	else
	{
			return FALSE;
	}
}


static uint8_t Key_Scan_down2(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/* 检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_UP )  
	{	 
			delay(iTick);	
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_UP )  
			{
					while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_UP)	//等待按键放开
							continue; 
					
					return TRUE;	 
			}
			else
			{
					return FALSE;
			}
	}
	else
	{
			return FALSE;
	}
}



void KeyPolling(void)
{
//		if( Key_Scan_down2(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == TRUE  )
//		{
//				printf("\n按下安全键!");
//			printf("LUNXUN\r\n");
//				if(0 == flag_FALLING)
//				{
//						USART_SendData(USART1, '2');		//发送一般求助信息
//						while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
//							continue;		
//						
//						printf("\t盲人不是摔倒状态，只是发送一般性的求助信息\r\n");
//				}
//				else
//					flag_FALLING=0;	//盲人安全

//		}		

//////////////////////////////////////////////////////////////////////////////////////////

		if( Key_Scan_down2(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == TRUE  )
		{
	
				printf("\n按下功能选择键!\r\n");
				if(MODE_VOLUME == key4.current_mode)	//当前处于音量调节模式
				{
					printf("\t当前处于音量调节中,当前音量等级为：%d\r\n", key4.key_rank[MODE_VOLUME]);
					//播放：当前处于音量调节模式
				}
				if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
				{
					printf("\t当前处于频率调节中，当前频率等级为：%d\r\n", key4.key_rank[MODE_FREQUENCY]);			
					//播放：当前处于频率调节模式
				}
				if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
				{
					printf("\t当前处于距离调节中，当前距离等级为：%d\r\n", key4.key_rank[MODE_DISTANCE]);
					//播放：当前处于距离调节模式
				}
				if(key4.current_mode != MODE_VOLUME && key4.current_mode != MODE_FREQUENCY && key4.current_mode != MODE_DISTANCE)
				{
					printf("\nKEY2 error! key4.current_mode = %d", key4.current_mode);
				}

		}		

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

		if( Key_Scan_down2(KEY3_GPIO_PORT,KEY3_GPIO_PIN) == TRUE  )
		{
				printf("\n按下进入下一个模式键!\r\n");
				key4.current_mode=(++key4.current_mode)%key4.max_mode;
				if(MODE_VOLUME == key4.current_mode)
				{
					printf("\t当前模式调整为：%s", "音量模式\r\n");
				}
				if(MODE_FREQUENCY == key4.current_mode)
				{
					printf("\t当前模式调整为：%s", "频率模式\r\n");
				}
				if(MODE_DISTANCE == key4.current_mode)
				{
					printf("\t当前模式调整为：%s", "距离模式\r\n");
				}

		}		

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

		if( Key_Scan_down2(KEY4_GPIO_PORT,KEY4_GPIO_PIN) == TRUE  )
		{
				printf("\n按下加号键!\r\n");
				
				if(MODE_VOLUME == key4.current_mode)	//当前处于音量调节模式
				{
						if(key4.max_rank == key4.key_rank[MODE_VOLUME]+1)
						{
							printf("\t当前音量调节为等级：%d，达到最大等级不可再调大\r\n", key4.key_rank[MODE_VOLUME]);				

						}
						else
						{
							//音量增加函数调用
							key4.key_rank[MODE_VOLUME]=(++key4.key_rank[MODE_VOLUME])%key4.max_rank;					
							printf("\t当前音量调节为等级：%d\r\n", key4.key_rank[MODE_VOLUME]);
						}

				}
				else if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
				{
						if(key4.max_rank == key4.key_rank[MODE_FREQUENCY]+1)
						{
							printf("\t当前频率调节为等级：%d，达到最大等级不可再调大\r\n", key4.key_rank[MODE_FREQUENCY]);			
						}
						else
						{
							//频率增加函数调用
							key4.key_rank[MODE_FREQUENCY]=(++key4.key_rank[MODE_FREQUENCY])%key4.max_rank;					
							printf("\t当前频率调节为等级：%d\r\n", key4.key_rank[MODE_FREQUENCY]);
						}

				}
				else if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
				{
						if(key4.max_rank == key4.key_rank[MODE_DISTANCE]+1)
						{			
							printf("\t当前距离调节为等级：%d，达到最大等级不可再调大\r\n", key4.key_rank[MODE_DISTANCE]);
						}
						else
						{
							//距离增加函数调用
							key4.key_rank[MODE_DISTANCE]=(++key4.key_rank[MODE_DISTANCE])%key4.max_rank;					
							printf("\t当前距离调节为等级：%d\r\n", key4.key_rank[MODE_DISTANCE]);
						}
				}
				else
				{
						printf("KEY4 error! key4.current_mode = %d\r\n", key4.current_mode);
				}

		}		

////////////////////////////////////////////////////////////////////////////////////////////////
		
		if( Key_Scan_down2(KEY5_GPIO_PORT,KEY5_GPIO_PIN) == TRUE  )
		{
				printf("\n按下减号键!\r\n");
		
				if(MODE_VOLUME == key4.current_mode)	//当前处于音量调节模式
				{
						if(key4.min_rank == key4.key_rank[MODE_VOLUME])
						{
							printf("\t当前音量调节为等级：%d，达到最小等级不可再调小\r\n", key4.key_rank[MODE_VOLUME]);
						}
						else
						{
							//音量减少函数调用
							key4.key_rank[MODE_VOLUME]=(--key4.key_rank[MODE_VOLUME])%key4.max_rank;					
							printf("\t当前音量调节为等级：%d\r\n", key4.key_rank[MODE_VOLUME]);
						}				
				}
				else if(MODE_FREQUENCY == key4.current_mode)	//当前处于频率调节模式
				{
						if(key4.min_rank == key4.key_rank[MODE_FREQUENCY])
						{
							printf("\t当前频率调节为等级：%d，达到最小等级不可再调小\r\n", key4.key_rank[MODE_FREQUENCY]);
						}
						else
						{
							//频率减少函数调用
							key4.key_rank[MODE_FREQUENCY]=(--key4.key_rank[MODE_FREQUENCY])%key4.max_rank;					
							printf("\t当前频率调节为等级：%d\r\n", key4.key_rank[MODE_FREQUENCY]);
						}
				}
				else if(MODE_DISTANCE == key4.current_mode)	//当前处于距离调节模式
				{
					if(key4.min_rank == key4.key_rank[MODE_DISTANCE])
					{
						printf("\t当前距离调节为等级：%d，达到最小等级不可再调小\r\n", key4.key_rank[MODE_DISTANCE]);			
					}
					else
					{
						//距离减少函数调用
						key4.key_rank[MODE_DISTANCE]=(--key4.key_rank[MODE_DISTANCE])%key4.max_rank;					
						printf("\t当前距离调节为等级：%d\r\n", key4.key_rank[MODE_DISTANCE]);
					}
				}
				else
				{
					printf("KEY5 error! key4.current_mode = %d\r\n", key4.current_mode);
				}
		
	}		

}

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
}

void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/*开启按键GPIO口的时钟*/
	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK,ENABLE);
												
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
}


void KEY1_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
//		EXTI_n(KEY1);
		printf("\n按下安全键! exit\r\n");
		if(0 == flag_FALLING)
		{
				USART_SendData(USART1, '2');		//发送一般求助信息
				while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
					continue;		
				
				printf("\t盲人不是摔倒状态，只是发送一般性的求助信息");
		}
		else
			flag_FALLING=0;	//盲人安全
    //清除中断标志位
		delay(iCOUNT);
//		EXTI_n_Open(KEY1);
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE); 
	}  
}


#endif



/*********************************************END OF FILE**********************/


