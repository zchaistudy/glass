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
#include"queue.h"
extern LinkQueue q;

extern int flag_FALLING;
//char Receive[10];
//int Num;

unsigned int Task_Delay[NumOfTask]={0};
extern void TimingDelay_Decrement(void);
extern void TimeStamp_Increment(void);
extern void gyro_data_ready_cb(void);


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
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
//	    ch = USART1->DR;
//		  printf("sjdfj\r\n");
//			int i;
			ch = USART_ReceiveData(USART1);
		  en_Queue(&q, ch);
//			if(ch=='s'||Receive[0]=='s')
//				Receive[Num]=ch;
//			else
//				return;
//			Num++;
//			if(Receive[Num-1]=='e')
//			{
//				Num=0;
//				printf( " ch = %s\r\n", Receive );    //将接受到的数据直接返回打
//				for(i=0;i<10;i++)
//					Receive[i]=' ';
//			}
	} 
	 
}

void USART2_IRQHandler(void)  
{  
	uint8_t ch;
     if(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET)  
        {       
                    //USART_SendData(USART2, USART_ReceiveData(USART2));   
          ch = USART_ReceiveData(USART2);
					printf( "%c", ch );    //将接受到的数据直接返回打印
        }   
} 


void KEY1_IRQHandler(void)
{
  //确保是否产生了EXTI Line中断
	if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET) 
	{
		printf("\nsafe!\n");
		flag_FALLING=0;
    //清除中断标志位
		EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);     
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


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
