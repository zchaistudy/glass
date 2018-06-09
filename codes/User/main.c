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
#include "stm32f10x_it.h"
#include "./systick/bsp_SysTick.h"
#include "./i2c/bsp_i2c.h"
#include "./exti/bsp_exti.h" 
#include "./MPU6050/mpu6050/mpu6050.h"
#include "bsp_key.h"
#include "bsp_GeneralTim.h"
#include "UltrasonicWave.h"
#include "debug.h"
#include "bsp_mpu6050.h"

LinkQueue q;
extern key_four key4;


/**
  * @brief  初始化函数
  * @param  无  
  * @retval 无
  */
static void PeriphInit()
{
	//滴答时钟初始化
	SysTick_Init();
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	//MPU6050中断引脚
	EXTI_Pxy_Config();
	//I2C初始化
	I2C_Bus_Init();
	//MPU6050初始化
	MPU6050Config();
	//按键初始化
#if zhongduan
	EXTI_Key_Config();	//中断	
#else
	EXTI_Key_Config();	//中断	
	Key_GPIO_Config();	//轮询
#endif
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	float Angle[4];
	key4.current_mode=0;	//按键结构初始化
	key4.key_rank[0]=0,key4.key_rank[1]=0,key4.key_rank[2]=0;
	key4.max_mode=3;
	key4.max_rank=4;
	key4.min_mode=0;
	key4.min_rank=0;
	
	
	USART1_Config();	     			//初始化串口1用于蓝牙通讯
	NVIC_Configuration();				//设置优先级
	USART2_Initialise( 38400 );	//串口2用于调试
	USART3_Config();						//初始化串口3用于语音模块
	
	init_Queue(&q); 		
	
	UltrasonicWave_Configuration();
	GENERAL_TIM_Init();
	TIM2_Init();
	
	PeriphInit();	//外设初始化
	printf("\n系统初始化完毕......\n");

	for(;;)
	{
		MPU6050Triaxial(Angle);	//三轴检测
		blind_falled();		//盲人是否摔倒		
		
#if zhongduan
#else
		KeyPolling();
#endif
		
		Deal_Data();
		
//		mdelay(50);
	}
}

/*********************************************END OF FILE**********************/
