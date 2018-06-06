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

LinkQueue q;
extern int flag_FALLING;	//盲人摔倒标志 =1表示摔倒， =0表示正常
int flag_SendText=0;	//判断是否发紧急信息（通过蓝牙）标志 =1表示发紧急信息， =0表示不发紧急信息


//#define BLIND_SAFE 1
//#define BLIND_UNSAFE 0
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
//	Key_GPIO_Config();	//轮询
	EXTI_Key_Config();	//中断
}

/**
  * @brief  判断是否需要通过蓝牙发送危险信息
  * @param  无  
  * @retval 无
  */
static void blind_falled()
{
	if(1==flag_FALLING)	//盲人摔倒
	{
			mdelay(1000);
			if(1==flag_FALLING){		//如果安全键还是没有被按下，那么flag_FALLING标志位还是为1，可以发送蓝牙信息
				
				USART_SendData(USART1, '1');		//发送危险信息
				while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
					continue;		
				
				printf("\n摔倒一段时间后安全键未被按下，已发送信息1给拐杖蓝牙模块");
				flag_FALLING=0;
			}
			
			else	//盲人无摔倒
			{
				printf("盲人摔倒后安全键已经被按下");		
			}
	}
	else
		return;
}

/**
  * @brief  按键功能模块
  * @param  无  
  * @retval 无
  */
static void key_module()
{
	int key_n;
	key_n=KeyPolling();	//按键模块
	
	switch(key_n)
	{
		case KEY_MODE:	//模式选择
			
			break;
		case KEY_BACK:	//返回上一个模式
			
			break;
		case KEY_ADD:	//加号
			
			break;
		case KEY_SUB:	//减号

			break;
		case KEY_SAFE:	//安全

			break;
		default:
			break;
	}
	
}


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	float Angle[4];
	
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
//		key_module();		//按键模块

		Deal_Data();
		
//		mdelay(50);
	}
}
/*********************************************END OF FILE**********************/
