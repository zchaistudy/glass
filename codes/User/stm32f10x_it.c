/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_exti.h"
#include "stm32f10x_it.h"
#include "./exti/bsp_exti.h" 
#include "./i2c/bsp_i2c.h"
#include "bsp_key.h"
#include "./systick/bsp_SysTick.h"
#include <stdio.h>
#include "string.h"
#include "queue.h"
#include "UltraConfig.h"
#include "UltrasonicWave.h"
#include "debug.h"
#include "bsp_key.h"
#include "mp3.h"

#define DirectionFlag '#'
#define WalkingStickFlag '!'

extern LinkQueue q;
extern int time;
extern int flag_FALLING;
extern int flag_volume;      
int ReceiveFromWalk[15];      //用于对拐杖数据的转化
int time_wait=0;
unsigned int Task_Delay[NumOfTask]={0};
char Status=0;     																				//用于标识接收数据的状态
int IndexWalkingStick=0;																	//用于标识当前接收到的数据位
extern void TimingDelay_Decrement(void);
extern void TimeStamp_Increment(void);
extern void gyro_data_ready_cb(void);
extern int UltrasonicWave_Distance_Walk[AVER_NUM_WALK];   //拐杖采集数据
extern int8_t GET_WALK_FLAG ;       											//接收拐杖数据标志

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	unsigned char i;

	TimingDelay_Decrement();
	TimeStamp_Increment();
	
	for(i=0;i<NumOfTask;i++)
	{
		if(Task_Delay[i])
		{
			Task_Delay[i]--;
		}
	}
}

/// IO 线中断
void EXTI_INT_FUNCTION (void)
{
//	MPU_DEBUG("intterrupt");
	if(EXTI_GetITStatus(EXTI_LINE) != RESET) //确保是否产生了EXTI Line中断
	{

		  /* Handle new gyro*/
		gyro_data_ready_cb();
		EXTI_ClearITPendingBit(EXTI_LINE);     //清除中断标志位
	}  
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
void USART1_IRQHandler(void)
{
	uint8_t ch;
	int i;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
			ch = USART_ReceiveData(USART1);
	//	printf("收到数据");   //不能使用，否则会出问题
	//		printf("%d",ch);
	//printf("%c",ch);                  //printf出来的都是字符
			if(ch==DirectionFlag||ch==WalkingStickFlag)    //判断当前的接收状态
			{
				Status=ch;
				memset(UltrasonicWave_Distance_Walk, 0, sizeof(UltrasonicWave_Distance_Walk));   //对UltrasonicWave_Distance_Walk[]初始化
				IndexWalkingStick=0;                                                             //索引初始化
				return;
			}
			if(Status==DirectionFlag)											//接收数据为方位信息
			{
					PlayDirection(ch);
					printf("拐杖已经将报警行发送到服务端");
					Status=0;
			}
			else if(Status==WalkingStickFlag)							//接收数据为拐杖信息
			{
				ReceiveFromWalk[IndexWalkingStick]=ch-'0';
				IndexWalkingStick++;
				if(IndexWalkingStick == AVER_NUM_WALK*3)
				{
						for(i=0;i<15;)                      //直接使用for循环会出现位置错乱——逆序
						{
								UltrasonicWave_Distance_Walk[i/3]=ReceiveFromWalk[i]*100+ReceiveFromWalk[i+1]*10+ReceiveFromWalk[i+2];
				//				printf("%d %d %d\r\n ,",ReceiveFromWalk[i],ReceiveFromWalk[i+1],ReceiveFromWalk[i+2]);
								printf("%d ,",UltrasonicWave_Distance_Walk[i/3]);
								i=i+3;
						}											
						GET_WALK_FLAG=1;                          //已经获取了拐杖信息标志	
						printf("\r\n");
				}
			}
	} 
}

void USART2_IRQHandler(void)  
{  
	uint8_t ch;
     if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)  
        {       
                    //USART_SendData(USART2, USART_ReceiveData(USART2));   
          ch = USART_ReceiveData(USART2);
//					printf( "%c", ch );    //将接受到的数据直接返回打印
        }   
} 



//采集超声波模块数据
void TIM2_IRQHandler(void)
{
	extern int8_t  MEASURE_FLAG;   // 1 眼镜采集数据， 0 等待拐杖采集数据
	
	static int portNum = 0;      //选择测距通道	
//	printf("tim2\r\n");
	if ( TIM_GetITStatus( TIM2, TIM_IT_Update) != RESET ) 
	{			
		if( MEASURE_FLAG)
		{
			UltrasonicWave(portNum);    //采集一个模块数据
			portNum++;
			if( portNum == AVER_NUM_GLASS +1 )   //眼睛上模块数据采集完毕
			{
				portNum = 0;
/************************/				
#ifdef ONLY_GLASS                //眼镜单独测试
				HasObstacle();		 //判断障碍物 
			}
		}
#else		                    //眼镜加拐杖		
/************************/		
			GetWalkingStickRequire();//$$$$$$$$$$向拐杖发送测距请求
				MEASURE_FLAG = 0;	 
				time_wait=0;		
			}
		}
		else if( GET_WALK_FLAG )                  //接收到拐杖数据
		{
			HasObstacle();               //判断障碍物位置并提示			
			GET_WALK_FLAG = 0;
			MEASURE_FLAG = 1;               //开始下一轮测距
		}
		if(time_wait>3000)								//在三秒内如果还没收到拐杖反馈的信息，进入下一步的轮询
			MEASURE_FLAG = 1;               //开始下一轮测距
#endif			
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  		 
	}	
}

void TIM3_IRQHandler(void)
{
	// 当要被捕获的信号的周期大于定时器的最长定时时，定时器就会溢出，产生更新中断
	if ( TIM_GetITStatus ( TIM3, TIM_IT_Update) != RESET )               
	{	
//		TIM_ICUserValueStructure.Capture_FinishFlag = 1;	
		TIM_ICUserValueStructure[0].Capture_CcrValue += GENERAL_TIM_PERIOD+1;
		TIM_ICUserValueStructure[1].Capture_CcrValue += GENERAL_TIM_PERIOD+1;
		TIM_ClearITPendingBit ( TIM3, TIM_FLAG_Update ); 		
	}
//printf("tim3\r\n");
//第一个超声波模块上升沿捕获中断
	if ( TIM_GetITStatus (TIM3, TIM_IT_CC1 ) != RESET && !TIM_ICUserValueStructure[0].Capture_FinishFlag )
	{  	
         p_debug("cc1\r\n");		
		// 第一次捕获
		if ( TIM_ICUserValueStructure[0].Capture_StartFlag == 0 )
		{
			p_debug("once\r\n");
			// 计数器清0
		//	TIM_SetCounter ( TIM3, 0 );       
      			
			// 自动重装载寄存器更新标志清0
//			TIM_ICUserValueStructure[0].Capture_Period = 0;
      // 存捕获比较寄存器的值的变量的值清0			
	//		TIM_ICUserValueStructure[0].Capture_CcrValue = 0;
			//记录第一次捕获寄存器的值 
			TIM_ICUserValueStructure[0].Capture_CcrValue = TIM_GetCapture1(TIM3);
			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStructure[0].Capture_StartFlag = 1;		
			TIM_ICUserValueStructure[0].Capture_CCx = 1;			
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			p_debug("second\r\n");
			// 获取捕获比较寄存器的值与第一次捕捉值的差值，这个值就是捕获到的高电平的时间的值
			TIM_ICUserValueStructure[0].Capture_CcrValue = TIM_GetCapture1(TIM3) - TIM_ICUserValueStructure[0].Capture_CcrValue ;

			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			TIM_ICUserValueStructure[0].Capture_StartFlag = 0;
            if( TIM_ICUserValueStructure[0].Capture_CCx == 1)
			{
			      // 捕获完成标志置1			
				TIM_ICUserValueStructure[0].Capture_FinishFlag = 1;
				p_debug("finish\r\n");
			}
			else
			{
				p_err("捕捉通道不匹配\r\n");
			}	
			p_debug("tim3, cc1\r\n");
		}
		TIM_ClearITPendingBit (TIM3,TIM_IT_CC1);	    
	}	
//第二个超声波模块上升沿捕获中断
	if ( TIM_GetITStatus (TIM3, TIM_IT_CC2 ) != RESET && !TIM_ICUserValueStructure[1].Capture_FinishFlag)
	{	
		p_debug("cc2\r\n");
		// 第一次捕获
		if ( TIM_ICUserValueStructure[1].Capture_StartFlag == 0 )
		{
			p_debug("once\r\n");
			// 计数器清0
			//TIM_SetCounter ( TIM3, 0 );
			// 自动重装载寄存器更新标志清0
			//TIM_ICUserValueStructure[1].Capture_Period = 0;
      // 存捕获比较寄存器的值的变量的值清0			
//////			TIM_ICUserValueStructure[1].Capture_CcrValue = 0;
			TIM_ICUserValueStructure[1].Capture_CcrValue = TIM_GetCapture2 (TIM3);

			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
			TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStructure[1].Capture_StartFlag = 1;
			TIM_ICUserValueStructure[1].Capture_CCx = 2;
		}
		// 下降沿捕获中断
		else // 第二次捕获
		{
			p_debug("second\r\n");
			// 获取捕获比较寄存器的值，这个值就是捕获到的高电平的时间的值
			TIM_ICUserValueStructure[1].Capture_CcrValue = TIM_GetCapture2 (TIM3) - TIM_ICUserValueStructure[1].Capture_CcrValue ;
			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，好开启新的一轮捕获
			TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising);
      // 开始捕获标志清0		
			TIM_ICUserValueStructure[1].Capture_StartFlag = 0;
            if( TIM_ICUserValueStructure[1].Capture_CCx == 2)
			{
			      // 捕获完成标志置1			
				TIM_ICUserValueStructure[1].Capture_FinishFlag = 1;
				p_debug("finish\r\n");
			}
			else
			{
				p_err("捕捉通道不匹配\r\n");
			}	
			p_debug("tim3, cc2\r\n");			
		}
		TIM_ClearITPendingBit (TIM3,TIM_IT_CC2);	    
	}		
}


/*******************************************************
*
* Function name ：TIM6_IRQHandler
* Description   ：用于语音模块的计时
* Parameter     ：无
* Return        ：无
*
**********************************************************/
void  TIM6_IRQHandler (void)
{
	if ( TIM_GetITStatus( TIM6, TIM_IT_Update) != RESET ) 
	{	
		time++;
		time_wait++;
		if(time==3400)			//每隔两秒钟，flag_volume重置为0，开启播放语音模块的权限
		{
//			printf("2\r\n");
			flag_volume=0;
			time=0;
		}
		if(time_wait==10000)
			time_wait=0;
		TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);  		 
	}		 	
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

///////////////////////////////////////////////////////////////////////////////////


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
