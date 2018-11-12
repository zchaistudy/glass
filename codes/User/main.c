/**
  ******************************************************************************
  * @file    main.c
  * @author  chzh
  * @version V1.0
  * @date    2018-10-12
  * @brief   ���ܱ����۾�
  ******************************************************************************
  */ 
 
 
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "mp3.h"
#include "stm32f10x_it.h"
#include "./systick/bsp_SysTick.h"
#include "./i2c/bsp_i2c.h"
#include "./exti/bsp_exti.h" 
#include "./MPU6050/mpu6050/mpu6050.h"
#include "bsp_key.h"
#include "UltraConfig.h"
#include "UltrasonicWave.h"
#include "debug.h"
#include "bsp_mpu6050.h"
#include "mpu6050.h"
#include "key.h"

extern int time;

//key4.current_mode=0;	//�����ṹ��ʼ��
//key4.key_rank[MODE_VOLUME]=0,key4.key_rank[MODE_FREQUENCY]=0,key4.key_rank[MODE_DISTANCE]=0;
//key4.max_mode=3;
//key4.max_rank=4;
//key4.min_mode=0;
//key4.min_rank=0;

void delaymain()
{
	int i = 1000, j;
	while( i-- )
	{
		j = 1000;
		while(j-- )
			;
	}
}



/**
  * @brief  ��ʼ������
  * @param  ��  
  * @retval ��
  */
static void PeriphInit()
{
	//�δ�ʱ�ӳ�ʼ��
	SysTick_Init();
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;    //ʹ�ܵδ�ʱ��
	//MPU6050�ж�����
	EXTI_Pxy_Config();
	//I2C��ʼ��
	I2C_Bus_Init();
	//MPU6050��ʼ��
	MPU6050Config();
	//������ʼ��
#if BREAK_EXTI_OPEN
	EXTI_Key_Config();	//�ж�
#else
	EXTI_Key_Config();
#endif
}

int main(void)
{	
	delaymain();
	USART_Config();	     			   //��ʼ������,����1��������ͨѶ������2���ڵ��ԡ���ʼ������3��������ģ��
	NVIC_Configuration();				//���ô������ȼ������ȼ�����ʹ��NVIC_PriorityGroup_2
    Key_Config();                       //��ʼ������
	UltrasonicWave_Init();          //��ʼ�����
	
	TIM6_Config();
	
	PeriphInit();              	//����ģ���ʼ��
	printf("\nϵͳ��ʼ�����......\n");

	for(;;)
	{
//printf("\runing......\n");		
		blind_falled();			
		//ä���Ƿ�ˤ��		
//#if BREAK_EXTI_OPEN
//#else
		KeyPolling();
//#endif

	}
}

/*********************************************END OF FILE**********************/
