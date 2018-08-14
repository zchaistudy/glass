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
#include <stdio.h>
int Rate=0;
int time=0;
int flag_volume=0;


u8 Call[5]={0x7E,0x03,0x0E,0x0D,0xEF};//停止指令

u8 Next[5]={0x7E,0x03,0x03,0x00,0xEF};//下一曲指令
u8 Last[5]={0x7E,0x03,0x04,0x07,0xEF};//上一曲指令

u8 Add[5] ={0x7E,0x03,0x05,0x06,0xEF};					//音量+指令	
u8 Sub[5] ={0x7E,0x03,0x06,0x05,0xEF};         //音量减指令

u8 rate_1[5]=      {0x7E,0x03,0x01,0x02,0xEF};                //播放指令,循环播放上一条语音
u8 rate_2[7]=      {0x7E,0x05,0x41,0x00,0x0F,0x4B,0xEF};      //最慢
u8 rate_3[7]=      {0x7E,0x05,0x41,0x00,0x01,0x45,0xEF};      //慢频率
u8 rate_4[7]=      {0x7E,0x05,0x41,0x00,0x02,0x46,0xEF};      //中频率
u8 rate_5[7]=      {0x7E,0x05,0x41,0x00,0x03,0x47,0xEF};      //快频率

u8 CareFootLeft[7]=    {0x7E,0x05,0x41,0x00,0x04,0x40,0xEF};      //脚下
u8 CareHandLeft[7]=    {0x7E,0x05,0x41,0x00,0x05,0x41,0xEF};      //手腕 
u8 CareHeadLeft[7]=    {0x7E,0x05,0x41,0x00,0x06,0x42,0xEF};      //头部
u8 CareFootRight[7]=    {0x7E,0x05,0x41,0x00,0x18,0x5C,0xEF};      //脚下
u8 CareHandRight[7]=    {0x7E,0x05,0x41,0x00,0x19,0x5D,0xEF};      //手腕 
u8 CareHeadRight[7]=    {0x7E,0x05,0x41,0x00,0x1A,0x5E,0xEF};      //头部

u8 East[7]=        {0x7E,0x05,0x41,0x00,0x07,0x43,0xEF};      //东
u8 South[7]=       {0x7E,0x05,0x41,0x00,0x08,0x4C,0xEF};      //南
u8 West[7]=        {0x7E,0x05,0x41,0x00,0x09,0x4D,0xEF};      //西
u8 North[7]=       {0x7E,0x05,0x41,0x00,0x0A,0x4E,0xEF};      //北
u8 SouthEast[7]=   {0x7E,0x05,0x41,0x00,0x0B,0x4F,0xEF};      //东南
u8 SouthWest[7]=   {0x7E,0x05,0x41,0x00,0x0C,0x48,0xEF};      //西南
u8 NorthWest[7]=   {0x7E,0x05,0x41,0x00,0x0D,0x49,0xEF};      //西北
u8 NorthEast[7]=   {0x7E,0x05,0x41,0x00,0x0E,0x4A,0xEF};      //东北

u8 ModeVolume[7]=  {0x7E,0x05,0x41,0x00,0x15,0x51,0xEF};      //语音提示模式
u8 ModeRate[7]=    {0x7E,0x05,0x41,0x00,0x16,0x52,0xEF};      //频率提示模式
u8 AdjustVolume[7]={0x7E,0x05,0x41,0x00,0x10,0x54,0xEF};      //音量调节模式
u8 AdjustRate[7]=  {0x7E,0x05,0x41,0x00,0x11,0x55,0xEF};      //频率调节模式
u8 ModeDistance[7]={0x7E,0x05,0x41,0x00,0x12,0x56,0xEF};      //距离调节
u8 ModeExit[7]=    {0x7E,0x05,0x41,0x00,0x17,0x53,0xEF};      //退出设置

u8 Alarm[7]=       {0x7E,0x05,0x41,0x00,0x13,0x57,0xEF};      //求救信号已发出
u8 AutoAlarm[7]=   {0x7E,0x05,0x41,0x00,0x14,0x50,0xEF};      //是否需要自动报警
u8 SendSucceed[7]= {0x7E,0x05,0x41,0x00,0x1B,0x5F,0xEF};      //报警信息已发送
u8 QuitAlarm[7]=   {0x7E,0x05,0x41,0x00,0x1C,0x58,0xEF};      //退出自动报警

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
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART3, ENABLE);
}

/*
 * 函数名：USART3_Send_String
 * 描述  ：发送AT指令
 * 输入  ：无
 * 输出  ：无	
 */
void USART3_Send_String(u8 *p,u8 cnt)
{   
//		printf("flag_volume == %d\r\n",flag_volume);
		if(flag_volume==1)
			return;
		else
		{
			flag_volume=1;
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
}

void USART3_Send_String_Key(u8 *p,u8 cnt)
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

/*
 * 函数名：AddRate
 * 描述  ：增加频率
 * 输入  ：无
 * 输出  ：无	
 */
void AddRate()
{
	if(Rate<3)
		Rate++;
}

/*
 * 函数名：SubRate
 * 描述  ：减少频率
 * 输入  ：无
 * 输出  ：无	
 */
void SubRate()
{
	if(Rate>1)
		Rate--;
}


/*
 * 函数名：AddVolume
 * 描述  ：增加音量
 * 输入  ：无
 * 输出  ：无	
 */
void AddVolume()
{
	USART3_Send_String_Key(Add,sizeof(Add));  //增加音量
}

/*
 * 函数名：SubVolume
 * 描述  ：减少音量
 * 输入  ：无
 * 输出  ：无	
 */
void SubVolume()
{
		USART3_Send_String_Key(Sub,sizeof(Sub));  //减少音量
}
	
/*
 * 函数名：Weighting
 * 描述  ：对等级进行加权
 * 输入  ：无
 * 输出  ：无	
 */
int Weighting(int degree)
{
	if(degree==1)
		return Rate;
	else
		return degree+2;
}

/*
 * 函数名：PlayVoice
 * 描述  ：播放语音。
 * 输入  ：输入即将碰撞的部位
 * 输出  ：无	
 */
void PlayVoice(int position,int left_right)
{
			if(left_right)
			{
				switch(position)
				{
					case head:
						USART3_Send_String(CareHeadRight,sizeof(CareHeadRight));
						break;
					case hand:
						USART3_Send_String(CareHandRight,sizeof(CareHandRight));
						break;
					case foot:
						USART3_Send_String(CareFootRight,sizeof(CareFootRight));
						break;
				}
			}
			else
			{
				switch(position)
				{
					case head:
						USART3_Send_String(CareHeadLeft,sizeof(CareHeadLeft));
						break;
					case hand:
						USART3_Send_String(CareHandLeft,sizeof(CareHandLeft));
						break;
					case foot:
						USART3_Send_String(CareFootLeft,sizeof(CareFootLeft));
						break;
				}
			}
		
}


/*
 * 函数名：PlayRate
 * 描述  ：播放频率。
 * 输入  ：输入频率的等级
 * 输出  ：无	
 */
void PlayRate(int degree)
{
		if(flag_volume==1)
			return;
		else
		{
			flag_volume=1;
			time=0;
			degree=Rate+degree;
			switch(degree)
			{
				case first:
					USART3_Send_String(rate_5,sizeof(rate_5));				//频率1
					break;
				case second:
					USART3_Send_String(rate_4,sizeof(rate_4));				//频率2
					break;
				case third:
					USART3_Send_String(rate_3,sizeof(rate_3));				//频率3
					break;
				case forth:
					USART3_Send_String(rate_2,sizeof(rate_2));				//频率4
					break;
				case fifth:
					USART3_Send_String(rate_1,sizeof(rate_1));				//频率5
					break;
			}
		}
}

/*
 * 函数名：PlayDirection
 * 描述  ：播放方位的信息。
 * 输入  ：输入方位信息
 * 输出  ：无	
 */
void PlayDirection(char direction)
{
		flag_volume=0;
		printf("内  收到的方位为：%c\r\n",direction);
		switch(direction)
		{
				case InEast:
					USART3_Send_String(East,sizeof(East));
					break;
				case InSouth:
						USART3_Send_String(South,sizeof(South));
						break;
				case InWest:
						USART3_Send_String(West,sizeof(West));
						break;
				case InNorth:
						USART3_Send_String(North,sizeof(North));
						break;
				case InSouthEast:
						USART3_Send_String(SouthEast,sizeof(SouthEast));
						break;
				case InSouthWest:
						USART3_Send_String(SouthWest,sizeof(SouthWest));
						break;
				case InNorthWest:
						USART3_Send_String(NorthWest,sizeof(NorthWest));
						break;
				case InNorthEast:
						USART3_Send_String(NorthEast,sizeof(NorthEast));
						break;					
		}
}

// 中断优先级配置
void TIM6_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn ;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
 * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
 * 另外三个成员是通用定时器和高级定时器才有.
 *-----------------------------------------------------------------------------
 *typedef struct
 *{ TIM_Prescaler            都有
 *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
 *  TIM_Period               都有
 *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
 *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
 *}TIM_TimeBaseInitTypeDef; 
 *-----------------------------------------------------------------------------
 */


void TIM6_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启定时器时钟,即内部时钟CK_INT=72M
    TIM6_TIM_APBxClock_FUN(TIM6_TIM_CLK, ENABLE);
	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = TIM6_TIM_Period;	

	  // 时钟预分频数为
    TIM_TimeBaseStructure.TIM_Prescaler= TIM6_TIM_Prescaler;
	
	  // 初始化定时器
    TIM_TimeBaseInit(TIM6_TIM, &TIM_TimeBaseStructure);
		
		// 清除计数器中断标志位
    TIM_ClearFlag(TIM6_TIM, TIM_FLAG_Update);
	  
		// 开启计数器中断
    TIM_ITConfig(TIM6_TIM,TIM_IT_Update,ENABLE);
		
		// 使能计数器
    TIM_Cmd(TIM6_TIM, ENABLE);	
}

