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
#include "UltraConfig.h"
#include "UltrasonicWave.h"
#include "debug.h"
#include "bsp_key.h"
#include "mp3.h"

#define DirectionFlag '#'
#define WalkingStickFlag '!'

extern int time;
extern int flag_FALLING;
extern int flag_volume;      
int ReceiveFromWalk[15];      //���ڶԹ������ݵ�ת��
int time_wait=0;
unsigned int Task_Delay[NumOfTask]={0};
char Status=0;     																				//���ڱ�ʶ�������ݵ�״̬
int IndexWalkingStick=0;																	//���ڱ�ʶ��ǰ���յ�������λ
extern void TimingDelay_Decrement(void);
extern void TimeStamp_Increment(void);
extern void gyro_data_ready_cb(void);
extern int UltrasonicWave_Distance_Walk[AVER_NUM_WALK];   //���Ȳɼ�����
extern int8_t GET_WALK_FLAG ;       											//���չ������ݱ�־

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

/// IO ���ж�
void EXTI_INT_FUNCTION (void)
{
//	MPU_DEBUG("intterrupt");
	if(EXTI_GetITStatus(EXTI_LINE) != RESET) //ȷ���Ƿ������EXTI Line�ж�
	{

		  /* Handle new gyro*/
		gyro_data_ready_cb();
		EXTI_ClearITPendingBit(EXTI_LINE);     //����жϱ�־λ
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
	//	printf("�յ�����");   //����ʹ�ã�����������
	//		printf("%d",ch);
	//printf("%c",ch);                  //printf�����Ķ����ַ�
			if(ch==DirectionFlag||ch==WalkingStickFlag)    //�жϵ�ǰ�Ľ���״̬��DirectionFlag��ʾ���յ����Ƿ�λ��Ϣ����WalkingStickFlag��ʾ�յ����ǳ�������������
			{
				Status=ch;
				memset(UltrasonicWave_Distance_Walk, 0, sizeof(UltrasonicWave_Distance_Walk));   //��UltrasonicWave_Distance_Walk[]��ʼ��
				IndexWalkingStick=0;                                                             //������ʼ��
				return;
			}
			if(Status==DirectionFlag)											//��������Ϊ��λ��Ϣ
			{
					p_debug("�յ��ķ�λΪ��%c\r\n",ch);
					PlayDirection(ch);
					Status=0;
			}
			else if(Status==WalkingStickFlag)							//��������Ϊ������Ϣ
			{
				ReceiveFromWalk[IndexWalkingStick]=ch-'0';  //�����յ������ݶ����ַ��ͣ���־�����������λ�������ݣ����Խ������͵�ת����
				IndexWalkingStick++;
				if(IndexWalkingStick == AVER_NUM_WALK*3)    //��15�����ݵ�λȫ���ռ���ʱ���������ݽ���
				{
						for(i=0;i<15;)                         
						{
								UltrasonicWave_Distance_Walk[i/3]=ReceiveFromWalk[i]*100+ReceiveFromWalk[i+1]*10+ReceiveFromWalk[i+2];
				//				p_debug("%d %d %d\r\n ,",ReceiveFromWalk[i],ReceiveFromWalk[i+1],ReceiveFromWalk[i+2]);
//								p_debug("%d ,",UltrasonicWave_Distance_Walk[i/3]);    //�������Ƿ���ȷ
								i=i+3;
						}											
						GET_WALK_FLAG=1;                          //�Ѿ���ȡ�˹�����Ϣ��־	
				}
			}
	} 
}


void USART3_IRQHandler(void)  
{  
	uint8_t ch;
	
     if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)   //����������ɺ�flag_volume��0��������һ�ε���������
        {       
          ch = USART_ReceiveData(USART3);
	//				printf("�յ�%c",ch);
					if(ch == 'K')           //��ʱ��������ָ���һ���᷵��ok�����Ե��ӵ�OKʱ�����ñ�־����ֹ�γ�����
					{
						flag_volume=1;

					}
					if(ch == 'S')
					{
						flag_volume=0;

					}
   
        }   
} 


//�ɼ�������ģ������
void TIM2_IRQHandler(void)
{
	extern int8_t  MEASURE_FLAG;   // 1 �۾��ɼ����ݣ� 0 �ȴ����Ȳɼ�����
	
	static int portNum = 0;      //ѡ����ͨ��	
	if ( TIM_GetITStatus( TIM2, TIM_IT_Update) != RESET ) 
	{			
//		p_debug("tim2\r\n");
		
		if( MEASURE_FLAG)
		{
			UltrasonicWave(portNum);    //�ɼ�һ��ģ������
			portNum++;
			if( portNum == AVER_NUM_GLASS +1 )   //�۾���ģ�����ݲɼ����
			{
				portNum = 0;
/************************/				
#ifdef ONLY_GLASS                //�۾���������
				HasObstacle();		 //�ж��ϰ��� 
			}
		}
#else		                    //�۾��ӹ���		
/************************/		
			GetWalkingStickRequire();//$$$$$$$$$$����ȷ��Ͳ������
				MEASURE_FLAG = 0;	 
				time_wait=0;		            //time_wait�Ǹ�ȫ�ֱ�����������ʱ������ʼ�ȴ���������ʱ��0
			}
		}
		else if( GET_WALK_FLAG )       //������յ����������������ʾ����������ȴ�������3�����·���
		{
			HasObstacle();               //�ж��ϰ���λ�ò���ʾ			
			GET_WALK_FLAG = 0;
			MEASURE_FLAG = 1;               //��ʼ��һ�ֲ��
		}
		if(time_wait>3000)								//�������������û�յ����ȷ�������Ϣ��������һ������ѯ
			MEASURE_FLAG = 1;               //��ʼ��һ�ֲ��
#endif			
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  		 
	}	
}



void TIM3_IRQHandler(void)
{
	// ��Ҫ��������źŵ����ڴ��ڶ�ʱ�������ʱʱ����ʱ���ͻ���������������ж�
	if ( TIM_GetITStatus ( TIM3, TIM_IT_Update) != RESET )               
	{		
		TIM_ICUserValueStructure[0].Capture_CcrValue += GENERAL_TIM_PERIOD+1;
		TIM_ICUserValueStructure[1].Capture_CcrValue += GENERAL_TIM_PERIOD+1;
		TIM_ClearITPendingBit ( TIM3, TIM_FLAG_Update ); 		
	}

//��һ��������ģ�������ز����ж�
	if ( TIM_GetITStatus (TIM3, TIM_IT_CC1 ) != RESET && !TIM_ICUserValueStructure[0].Capture_FinishFlag )
	{  			
		if ( TIM_ICUserValueStructure[0].Capture_StartFlag == 0 )
		{
			TIM_ICUserValueStructure[0].Capture_CcrValue = TIM_GetCapture1(TIM3);
			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Falling);			
			TIM_ICUserValueStructure[0].Capture_StartFlag = 1;		
			TIM_ICUserValueStructure[0].Capture_CCx = 1;			
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			p_debug("second\r\n");
			// ��ȡ����ȽϼĴ�����ֵ���һ�β�׽ֵ�Ĳ�ֵ
			TIM_ICUserValueStructure[0].Capture_CcrValue = TIM_GetCapture1(TIM3) - TIM_ICUserValueStructure[0].Capture_CcrValue ;
			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Rising);
			TIM_ICUserValueStructure[0].Capture_StartFlag = 0;
			      // ������ɱ�־��1			
			TIM_ICUserValueStructure[0].Capture_FinishFlag = 1;
		}
		TIM_ClearITPendingBit (TIM3,TIM_IT_CC1);	    
	}	
//�ڶ���������ģ�������ز����ж�
	if ( TIM_GetITStatus (TIM3, TIM_IT_CC2 ) != RESET && !TIM_ICUserValueStructure[1].Capture_FinishFlag)
	{	
		p_debug("cc2\r\n");
		// ��һ�β���
		if ( TIM_ICUserValueStructure[1].Capture_StartFlag == 0 )
		{
			p_debug("once\r\n");
			TIM_ICUserValueStructure[1].Capture_CcrValue = TIM_GetCapture2 (TIM3);
			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
			TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling);
      // ��ʼ�����׼��1			
			TIM_ICUserValueStructure[1].Capture_StartFlag = 1;
			TIM_ICUserValueStructure[1].Capture_CCx = 2;
		}
		// �½��ز����ж�
		else // �ڶ��β���
		{
			p_debug("second\r\n");
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ��񵽵ĸߵ�ƽ��ʱ���ֵ
			TIM_ICUserValueStructure[1].Capture_CcrValue = TIM_GetCapture2 (TIM3) - TIM_ICUserValueStructure[1].Capture_CcrValue ;
			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ��ÿ����µ�һ�ֲ���
			TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising);
      // ��ʼ�����־��0		
			TIM_ICUserValueStructure[1].Capture_StartFlag = 0;
            TIM_ICUserValueStructure[1].Capture_FinishFlag = 1;
			p_debug("tim3, cc2\r\n");			
		}
		TIM_ClearITPendingBit (TIM3,TIM_IT_CC2);	    
	}		
}

/*******************************************************
*
* Function name ��TIM6_IRQHandler
* Description   ����������ģ��ļ�ʱ
* Parameter     ����
* Return        ����
*
**********************************************************/
void  TIM6_IRQHandler (void)
{
	if ( TIM_GetITStatus( TIM6, TIM_IT_Update) != RESET ) 
	{	
		time_wait++;
		if(time_wait==10000)   //��ֹ�������
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
