/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   串口中断接收测试
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 iSO STM32 开发板 
  * 论坛    :http://www.chuxue123.com
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
 
 
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include"queue.h"
LinkQueue q;
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
// int i;
	USART1_Config();	
	NVIC_Configuration();

	USART2_Initialise( 38400 );

	init_Queue(&q); 
	
	for(;;)
	{
		Deal_Data();
	}
}
/*********************************************END OF FILE**********************/
