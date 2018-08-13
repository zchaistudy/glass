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
#include "UltraConfig.h"
#include "UltrasonicWave.h"
#include "debug.h"
#include "bsp_mpu6050.h"

LinkQueue q;
extern key_four key4;
extern int time;

//key4.current_mode=0;	//按键结构初始化
//key4.key_rank[MODE_VOLUME]=0,key4.key_rank[MODE_FREQUENCY]=0,key4.key_rank[MODE_DISTANCE]=0;
//key4.max_mode=3;
//key4.max_rank=4;
//key4.min_mode=0;
//key4.min_rank=0;


/**
  * @brief  初始化函数
  * @param  无  
  * @retval 无
  */
static void PeriphInit()
{
	//滴答时钟初始化
	SysTick_Init();
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;    //使能滴答定时器
	//MPU6050中断引脚
	EXTI_Pxy_Config();
	//I2C初始化
	I2C_Bus_Init();
	//MPU6050初始化
	MPU6050Config();
	//按键初始化
#if BREAK_EXTI_OPEN
	EXTI_Key_Config();	//中断
#else
	EXTI_Key_Config();
	Key_GPIO_Config();	//轮询
#endif
}

int main(void)
{	
	float Angle[4];
	
	USART1_Config();	     			//初始化串口1用于蓝牙通讯
	NVIC_Configuration();				//设置优先级
	USART2_Initialise( 38400 );	//串口2用于调试
	USART3_Config();						//初始化串口3用于语音模块
	
//	init_Queue(&q); 		
	
	TIM6_TIM_Mode_Config();			//初始化定时器6
	TIM6_TIM_NVIC_Config();			//配置定时器6的优先级
	
 	UltrasonicWave_Configuration();
	GENERAL_TIM_Init();
	TIM2_Init();
	
	PeriphInit();              	//报警模块以及按钮的初始化
	printf("\n系统初始化完毕......\n");

	for(;;)
	{	
		MPU6050Triaxial(Angle);		//三轴检测
		blind_falled();						//盲人是否摔倒		
		SendHelp();
//#if BREAK_EXTI_OPEN
//#else
		KeyPolling();

//#endif
//		
//		Deal_Data();
	}
}

/*********************************************END OF FILE**********************/
