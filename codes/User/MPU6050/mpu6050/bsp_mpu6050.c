#include "bsp_mpu6050.h"
#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "stm32f10x_it.h"
#include "./systick/bsp_SysTick.h"
#include "./i2c/bsp_i2c.h"
#include "./exti/bsp_exti.h" 
#include "./MPU6050/mpu6050/mpu6050.h"
#include "bsp_key.h"


extern int flag_FALLING;	//盲人摔倒标志 =1表示摔倒， =0表示正常

/**
  * @brief  判断是否需要通过蓝牙发送危险信息
  * @param  无  
  * @retval 无
  */
void blind_falled()
{
	if(1==flag_FALLING)	//盲人摔倒
	{
			mdelay(10000);	//延时1秒
			if(1==flag_FALLING){		//如果安全键还是没有被按下，那么flag_FALLING标志位还是为1，可以发送蓝牙信息
				
				USART_SendData(USART1, '1');		//发送危险信息
				while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
					continue;		
				
				printf("\n摔倒一段时间后安全键未被按下，已发送信息1给拐杖蓝牙模块");
				flag_FALLING=0;
				
			}
			
			else	//盲人无摔倒
			{
				printf("盲人摔倒后安全键已经被按下，无安全隐患");		
			}
	}
	else
		return;
}

/**
  * @brief  四个按键功能模块
  * @param  无  
  * @retval 无
  */
void key_module()
{
//	int key_n;
//	key_n=KeyPolling();	//按键模块
//	
//	switch(key_n)
//	{
//		case KEY_MODE:	//模式选择
//			
//			break;
//		case KEY_BACK:	//返回上一个模式
//			
//			break;
//		case KEY_ADD:	//加号
//			
//			break;
//		case KEY_SUB:	//减号

//			break;
//		case KEY_SAFE:	//安全

//			break;
//		default:
//			break;
//	}
	
}
