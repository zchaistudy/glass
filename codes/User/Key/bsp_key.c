
#include "./key/bsp_key.h"  
#include "./usart/bsp_usart1.h"
#include "./systick/bsp_SysTick.h"
#include "mp3.h"
#include "UltrasonicWave.h"
extern int flag_volume;  
extern int flag_FALLING;
extern  int MODE_FLAG;       //1 语音 0 频率
key_four key4 = {0, {1, 1, 1}, MAX_MODE, 4, 0, 0,0};

static void delay(int i)
{
	for(;i>0;i--);
}


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
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 

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


#if 1         //按下时输出高电平，松开时输出低电平
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
					while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_DOWN);	//等待按键放开
				//			printf("等待按键释放\r\n"); 
					
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

#else
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
#endif


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

		if( Key_Scan_down(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == TRUE  )
		{
									flag_volume=0;
				printf("\n按下功能设置键!\r\n");
			     key4.set_parameter = (key4.set_parameter+1) % SET_MAX;    //选择下一个模式
               if(SET_CLOSE == key4.set_parameter)	//当前处于不可调节模式
				{
					printf("\t退出设置\r\n");
					USART3_Send_String(ModeExit,sizeof(ModeExit));
					//播放：退出设置
				}		
				else if(SET_VOLUME == key4.set_parameter)	//当前处于音量调节模式
				{
					printf("\t当前处于音量调节中,当前音量等级为：%d\r\n", key4.key_rank[MODE_VOLUME]);
					USART3_Send_String(AdjustVolume,sizeof(AdjustVolume));
					//播放：当前处于音量调节模式
				}				
				else if(SET_FREQUENCY == key4.set_parameter)	//当前处于频率调节模式
				{
					printf("\t当前处于频率调节中，当前频率等级为：%d\r\n", key4.key_rank[MODE_FREQUENCY]);			
					USART3_Send_String(AdjustRate,sizeof(AdjustRate));
					//播放：当前处于频率调节模式
				}
				else if(SET_DISTANCE == key4.set_parameter)	//当前处于距离调节模式
				{
					printf("\t当前处于距离调节中，当前距离等级为：%d\r\n", key4.key_rank[MODE_DISTANCE]);
					USART3_Send_String(ModeDistance,sizeof(ModeDistance));
					//播放：当前处于距离调节模式
				}
				else 
				{
					printf("\nKEY2 error! key4.current_mode = %d", key4.current_mode);
				}

		}		

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

		if( Key_Scan_down(KEY3_GPIO_PORT,KEY3_GPIO_PIN) == TRUE  )
		{
			flag_volume=0;
				printf("\n按下进入下一个模式键!\r\n");
				key4.current_mode=(++key4.current_mode)%key4.max_mode;
				if(MODE_VOLUME == key4.current_mode)
				{
					MODE_FLAG=1;
					printf("\t当前模式调整为：%s", "音量模式\r\n");
					USART3_Send_String(ModeVolume,sizeof(ModeVolume));
					//播放
				}
				else if(MODE_FREQUENCY == key4.current_mode)
				{
					MODE_FLAG=0;
					printf("\t当前模式调整为：%s", "频率模式\r\n");
					USART3_Send_String(ModeRate,sizeof(ModeRate));
					//播放
				}
				else if(MODE_SNAKE == key4.current_mode)
				{
					printf("\t当前模式调整为：%s", "震动模式\r\n");
					MODE_FLAG = 2;
				}
		}		

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

		if( Key_Scan_down(KEY4_GPIO_PORT,KEY4_GPIO_PIN) == TRUE )
		{
			flag_volume=0;
				printf("\n按下加号键!\r\n");
				if( SET_CLOSE == key4.set_parameter )
				{
					printf("请选择设置模式\r\n");
				}
				 else if(SET_VOLUME == key4.set_parameter)	//当前处于音量调节模式
				{
						if(MAX_RANK_VOLUME <= key4.key_rank[MODE_VOLUME])
						{
							printf("\t当前音量调节为等级：%d，达到最大等级不可再调大\r\n", key4.key_rank[MODE_VOLUME]);				
								AddVolume();         //音量增加函数调用
						}
						else
						{
							AddVolume();         //音量增加函数调用
							++key4.key_rank[MODE_VOLUME];					
							printf("\t当前音量调节为等级：%d\r\n", key4.key_rank[MODE_VOLUME]);
						}

				}
				else if(SET_FREQUENCY == key4.set_parameter)	//当前处于频率调节模式
				{
						if(MAX_RANK_FREQUENCY == key4.key_rank[MODE_FREQUENCY])
						{
							printf("\t当前频率调节为等级：%d，达到最大等级不可再调大\r\n", key4.key_rank[MODE_FREQUENCY]);			
						}
						else
						{
							AddRate();//频率增加函数调用
							++key4.key_rank[MODE_FREQUENCY];					
							printf("\t当前频率调节为等级：%d\r\n", key4.key_rank[MODE_FREQUENCY]);
						}

				}
				else if(SET_DISTANCE == key4.set_parameter)	//当前处于距离调节模式
				{
						if(MAX_RANK_DISTANCE == key4.key_rank[MODE_DISTANCE])
						{			
							printf("\t当前距离调节为等级：%d，达到最大等级不可再调大\r\n", key4.key_rank[MODE_DISTANCE]);
						}
						else
						{
							addDistance();             //距离增加函数调用
							++key4.key_rank[MODE_DISTANCE];					
							printf("\t当前距离调节为等级：%d\r\n", key4.key_rank[MODE_DISTANCE]);
						}
				}
				else
				{
						printf("KEY4 error! key4.set_parameter = %d\r\n", key4.set_parameter);
				}

		}		

////////////////////////////////////////////////////////////////////////////////////////////////
		
		if( Key_Scan_down(KEY5_GPIO_PORT,KEY5_GPIO_PIN) == TRUE )
		{
			flag_volume=0;
				printf("\n按下减号键!\r\n");
				if( SET_CLOSE == key4.set_parameter )
				{
					printf("请选择设置模式\r\n");
				}		
				else if(SET_VOLUME == key4.set_parameter)	//当前处于音量调节模式
				{
						if(key4.min_rank == key4.key_rank[MODE_VOLUME])
						{
							printf("\t当前音量调节为等级：%d，达到最小等级不可再调小\r\n", key4.key_rank[MODE_VOLUME]);
							SubVolume();//音量减少函数调用
						}
						else
						{
							SubVolume();//音量减少函数调用
							--key4.key_rank[MODE_VOLUME];					
							printf("\t当前音量调节为等级：%d\r\n", key4.key_rank[MODE_VOLUME]);
						}				
				}
				else if(SET_FREQUENCY == key4.set_parameter)	//当前处于频率调节模式
				{
						if(key4.min_rank == key4.key_rank[MODE_FREQUENCY])
						{
							printf("\t当前频率调节为等级：%d，达到最小等级不可再调小\r\n", key4.key_rank[MODE_FREQUENCY]);
						}
						else
						{
							SubRate();//频率减少函数调用
							--key4.key_rank[MODE_FREQUENCY];					
							printf("\t当前频率调节为等级：%d\r\n", key4.key_rank[MODE_FREQUENCY]);
						}
				}
				else if(SET_DISTANCE == key4.set_parameter)	//当前处于距离调节模式
				{
					if(key4.min_rank == key4.key_rank[MODE_DISTANCE])
					{
						printf("\t当前距离调节为等级：%d，达到最小等级不可再调小\r\n", key4.key_rank[MODE_DISTANCE]);			
					}
					else
					{
						minusDistance();       //距离减少函数调用
						--key4.key_rank[MODE_DISTANCE];					
						printf("\t当前距离调节为等级：%d\r\n", key4.key_rank[MODE_DISTANCE]);
					}
				}
				else
				{
					printf("KEY5 error! key4.set_parameter = %d\r\n", key4.set_parameter);
				}
		
	}		

}

static void NVIC_Configuration2(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
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
	/* 沿中断 */
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
		delay(10);		
		if(GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_DOWN )  //消抖
		{
			if(0 == flag_FALLING)
			{
					USART_SendData(USART1, '2');		//发送一般求助信息
					while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
						continue;		
					flag_volume=0;
					USART3_Send_String(Alarm,sizeof(Alarm));
					printf("\t 播放：求救信息已发出r\n");
			}
			else
			{
					flag_FALLING=0;	//盲人安全
					flag_volume=0;
					USART3_Send_String(QuitAlarm,sizeof(QuitAlarm));
					printf("\t 已退出自动报警模式，请注意安全r\n");
			}

		//清除中断标志位
			delay(iCOUNT);			
		}			
//		EXTI_n_Open(KEY1);
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE); 
	}  
}






/*********************************************END OF FILE**********************/


