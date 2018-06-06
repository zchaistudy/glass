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

u8 Play[5]={0x7E,0x03,0x01,0x02,0xEF};//播放指令
u8 Call[5]={0x7E,0x03,0x0E,0x0D,0xEF};//停止指令

u8 Next[5]={0x7E,0x03,0x03,0x00,0xEF};//下一曲指令
u8 Last[5]={0x7E,0x03,0x04,0x07,0xEF};//上一曲指令

u8 Add[5]={0x7E,0x03,0x05,0x06,0xEF};					//音量+指令	
u8 Sub[5]={0x7E,0x03,0x06,0x05,0xEF};         //音量减指令

u8 LF[7]={0x7E,0x05,0x41,0x00,0x01,0x45,0xEF};//慢频率
u8 MF[7]={0x7E,0x05,0x41,0x00,0x02,0x46,0xEF};//中频率
u8 HF[7]={0x7E,0x05,0x41,0x00,0x03,0x47,0xEF};//快频率
u8 CareFoot[7]={0x7E,0x05,0x41,0x00,0x04,0x40,0xEF};//脚下
u8 CareHand[7]={0x7E,0x05,0x41,0x00,0x05,0x41,0xEF};//手腕
u8 CareHead[7]={0x7E,0x05,0x41,0x00,0x06,0x42,0xEF};//头部
u8 East[7]={0x7E,0x05,0x41,0x00,0x07,0x43,0xEF};//东
u8 South[7]={0x7E,0x05,0x41,0x00,0x08,0x4C,0xEF};//南
u8 West[7]={0x7E,0x05,0x41,0x00,0x09,0x4D,0xEF};//西
u8 North[7]={0x7E,0x05,0x41,0x00,0x0A,0x4E,0xEF};//北
u8 SouthEast[7]={0x7E,0x05,0x41,0x00,0x0B,0x4F,0xEF};//东南
u8 SouthWest[7]={0x7E,0x05,0x41,0x00,0x0C,0x48,0xEF};//西南
u8 NorthWest[7]={0x7E,0x05,0x41,0x00,0x0D,0x49,0xEF};//西北
u8 NorthEast[7]={0x7E,0x05,0x41,0x00,0x0E,0x4A,0xEF};//东北

void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART3 clock */
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); //使能UART3所在GPIOB的时钟    
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);
	
	
	USART_Cmd(USART3, ENABLE);
}

void USART3_Send_String(u8 *p,u8 cnt)
{   
		 while(cnt>0)
		 {	 
		     USART_ClearFlag(USART3, USART_FLAG_TC);
		     USART_SendData(USART3,*p++);			 
		     while(USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
		     {
		   	     ;
		     }
				 cnt--;
		 }
}
