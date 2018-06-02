/**
  ******************************************************************************
  * @file    main.c
  * @author  chzh
  * @version V1.0
  * @date    2018-05-19
  * @brief   智能避障眼镜
  ******************************************************************************
  */ 
 
 
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "queue.h"
#include "mp3.h"
#include "bsp_GeneralTim.h"
#include "UltrasonicWave.h"
#include "debug.h"
LinkQueue q;
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	USART1_Config();	     			//初始化串口1用于蓝牙通讯
	NVIC_Configuration();				//设置优先级
	USART2_Initialise( 38400 );	//串口2用于调试
	UltrasonicWave_Configuration();
	GENERAL_TIM_Init();
	TIM2_Init();
	init_Queue(&q); 						
	MP3_GPIO_Config();
	MP3Run(1);									//用于输出语音
	printf("start\r\n");
	for(;;)
	{
		Deal_Data();
	}
}
/*********************************************END OF FILE**********************/
