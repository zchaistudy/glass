/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  chzh
  * @version V1.0
  * @date    2018-10-12
  * @brief   usart相关函数
  ******************************************************************************
  */
  
#include "bsp_usart1.h"
char Receive[10];
int Num;
int flag=0;
extern int Shake;

/*
 * 函数名：USART_Config
 * 描述  ：串口函数的配置
 * 输入  ：无
 * 输出  ：无	
 */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1、USART2、USART3clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE);   
	
	/* USART1、USART2、USART3 GPIO config */
	/* Configure USART1、USART2、USART3 Tx (PA.09、PA.02、PB10.) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure USART1、USART2、USART3 Rx (PA.10、PA.03、PB.11) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*USART1、USART2、USART3 mode config */
	USART_InitStructure.USART_BaudRate = 38400;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Init(USART2, &USART_InitStructure);
	USART_InitStructure.USART_BaudRate = 9600;
	USART_Init(USART3, &USART_InitStructure);
	
	/* 使能串口1接收中断 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USART_Cmd(USART3, ENABLE);
}

/*
 * 函数名：NVIC_Configuration
 * 描述  ：配置USART1、USART3接收中断
 * 输入  ：无
 * 输出  ：无	
 */
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART2
 * 输入  ：无
 * 输出  ：无	
 */
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到USART2 */
		USART_SendData(USART2, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

/*
 * 函数名：fgetc
 * 描述  ：重定向c库函数scanf到USART2
 * 输入  ：无
 * 输出  ：无	
 */
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
		return (int)USART_ReceiveData(USART2);
}

/*
 * 函数名：GetWalkingStickRequire
 * 描述  ：获取拐杖数据请求
 * 输入  ：无
 * 输出  ：无	
 */

void GetWalkingStickRequire(void)
{
	USART_SendData(USART1,'#');          //字符'3'为获取信息信号
	//	printf("数据已发送\r\n");
}

void AdjustVibrationFrequencyGlasses(char degree)
{
	if(degree)
		degree=Shake+degree+'0';
	else
		degree = '0';
	USART_SendData(USART1,degree);          //字符'3'为获取信息信号
}

/*********************************************END OF FILE**********************/
