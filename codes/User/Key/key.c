//按键逻辑实现
#include "key.h"
#include "./key/bsp_key.h"  
#include "./usart/bsp_usart1.h"
#include "mp3.h"
#include "UltrasonicWave.h"

extern int flag_volume;  
extern int flag_FALLING;
extern  int MODE_FLAG;       //1 语音 0 频率
static struct key KeyConfig;
extern int flag_volume;
 /*
 * 函数名：Key_Scan
 * 描述  ：检测是否有按键按下
 * 输入  ：GPIOx：x 可以是 A，B，C，D或者 E
 *		     GPIO_Pin：待读取的端口位 	
 * 输出  ：0(没按下按键)、1（按下按键）
 */
static uint8_t Key_Scan_down(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/* 检测是否有按键按下 */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_DOWN )  
	{	 
			delay_key(10);	
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_DOWN )  
			{
					while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_DOWN);	//等待按键放开
				//			printf("等待按键释放\r\n"); 
					
					return 1;	 
			}
			else
			{
					return 0;
			}
	}
	else
	{
			return 0;
	}
}



/*
* @brief 参数设置,选择指定参数进行调节
* @param  no
* @return no
*/
static void setFunction()
{
	flag_volume=0;
	printf("\n按下功能设置键!\r\n");
	 KeyConfig.set_parameter = (KeyConfig.set_parameter+1) % SET_MAX;    //选择下一个设置参数
	
	switch(KeyConfig.set_parameter)
	{
		case SET_CLOSE:	    		
						printf("\t退出设置\r\n");
						USART3_Send_String(ModeExit,sizeof(ModeExit)); 
						break;
		case SET_VOLUME:	
						printf("\t当前处于音量调节中,当前音量等级为：%d\r\n", KeyConfig.set_volume);
						USART3_Send_String(AdjustVolume,sizeof(AdjustVolume));  //播放：当前处于音量调节模式			
						break;   
		case SET_FREQUENCY:
						printf("\t当前处于频率调节中，当前频率等级为：%d\r\n", KeyConfig.set_frequency);			
						USART3_Send_String(AdjustRate,sizeof(AdjustRate));     //播放：当前处于频率调节模式
						break;    
		case SET_DISTANCE: 	
						printf("\t当前处于距离调节中，当前距离等级为：%d\r\n", KeyConfig.set_distance);
						USART3_Send_String(ModeDistance,sizeof(ModeDistance));  //播放：当前处于距离调节模式
						break;   
		case SET_SHAKE:
						printf("\t当前处于震动调节中,当前震动等级为：%d\r\n", KeyConfig.set_shake);
			//			USART3_Send_String(AdjustVolume,sizeof(AdjustVolume));  //播放：当前处于震动调节模式			
						break; 
		default :printf("\nsetparamter error! KeyConfig.current_mode = %d", KeyConfig.set_parameter);
	}			
}


/*
* @brief 切换提示模式
* @param  no
* @return no
*/
void switchMode()
{
	flag_volume=0;
	printf("\n按下进入下一个模式键!\r\n");
	KeyConfig.current_mode= (++KeyConfig.current_mode) % MODE_MAX;
	if(MODE_VOLUME == KeyConfig.current_mode)
	{
		MODE_FLAG=1;
		printf("\t当前模式调整为：%s", "音量模式\r\n");
		USART3_Send_String(ModeVolume,sizeof(ModeVolume));
		//播放
		AdjustVibrationFrequencyGlasses(0);   //将震动重设为0
	}
	else if(MODE_FREQUENCY == KeyConfig.current_mode)
	{
		MODE_FLAG=0;
		printf("\t当前模式调整为：%s", "频率模式\r\n");
		USART3_Send_String(ModeRate,sizeof(ModeRate));
		//播放
	}
	else if(MODE_SHAKE == KeyConfig.current_mode)
	{
		printf("\t当前模式调整为：%s", "震动模式\r\n");
		MODE_FLAG = 2;
	}	
}	

/*
* @brief 调高音量
* @param  no
* @return no
*/
static void volumeUp()
{
	if(MAX_RANK_VOLUME <= KeyConfig.set_volume)
	{
		printf("\t当前音量调节为等级：%d，达到最大等级不可再调大\r\n", KeyConfig.set_volume);				
		AddVolume();         //音量增加函数调用
	}
	else
	{
		AddVolume();         //音量增加函数调用
		++KeyConfig.set_volume;					
		printf("\t当前音量调节为等级：%d\r\n", KeyConfig.set_volume);
	}	
}	

/*
* @brief 调高频率
* @param  no
* @return no
*/
static void rateUp()
{
	if(MAX_RANK_FREQUENCY == KeyConfig.set_frequency)
	{
		printf("\t当前频率调节为等级：%d，达到最大等级不可再调大\r\n", KeyConfig.set_frequency);			
	}
	else
	{
		AddRate();//频率增加函数调用
		++KeyConfig.set_frequency;					
		printf("\t当前频率调节为等级：%d\r\n", KeyConfig.set_frequency);
	}		
}

/*
* @brief 增大监测距离
* @param  no
* @return no
*/
void distanceUp()
{
	if(MAX_RANK_DISTANCE == KeyConfig.set_distance)
	{			
		printf("\t当前距离调节为等级：%d，达到最大等级不可再调大\r\n", KeyConfig.set_distance);
	}
	else
	{
		addDistance();             //距离增加函数调用
		++KeyConfig.set_distance;					
		printf("\t当前距离调节为等级：%d\r\n", KeyConfig.set_distance);
	}	
}
/*
* @brief 增大震动等级
* @param  no
* @return no
*/
void shakeUp()
{
	if(MAX_RANK_SHAKE == KeyConfig.set_shake)
	{			
		printf("\t当前震动调节为等级：%d，达到最大等级不可再调大\r\n", KeyConfig.set_shake);
	}
	else
	{
		AddShake();             //距离增加函数调用
		++KeyConfig.set_shake;					
		printf("\t当前震动调节为等级：%d\r\n", KeyConfig.set_shake);
	}	
}

/*
* @brief 降低音量等级
* @param  no
* @return no
*/
static void volumeDown()
{
	if(MIN_RANK_VOLUME == KeyConfig.set_volume)
	{
		printf("\t当前音量调节为等级：%d，达到最小等级不可再调小\r\n", KeyConfig.set_volume);
		SubVolume();//音量减少函数调用
	}
	else
	{
		SubVolume();//音量减少函数调用
		--KeyConfig.set_volume;					
		printf("\t当前音量调节为等级：%d\r\n", KeyConfig.set_volume);
	}				
}
	
/*
* @brief 降低频率等级
* @param  no
* @return no
*/
static void rateDown()
{
	if(MIN_RANK_FREQUENCY == KeyConfig.set_frequency)
	{
		printf("\t当前频率调节为等级：%d，达到最小等级不可再调小\r\n", KeyConfig.set_frequency);
	}
	else
	{
		SubRate();//频率减少函数调用
		--KeyConfig.set_frequency;					
		printf("\t当前频率调节为等级：%d\r\n", KeyConfig.set_frequency);
	}	
}	

/*
* @brief 减小监测距离等级
* @param  no
* @return no
*/
static void distanceDown() 
{
	if( MIN_RANK_DISTANCE == KeyConfig.set_distance)
	{
		printf("\t当前距离调节为等级：%d，达到最小等级不可再调小\r\n", KeyConfig.set_distance);			
	}
	else
	{
		minusDistance();       //距离减少函数调用
		--KeyConfig.set_distance;					
		printf("\t当前距离调节为等级：%d\r\n", KeyConfig.set_distance);
	}
}

/*
* @brief 降低震动等级
* @param  no
* @return no
*/
static void shakeDown()
{
	if(MIN_RANK_SHAKE == KeyConfig.set_shake)
	{
		printf("\t当前频率调节为等级：%d，达到最小等级不可再调小\r\n", KeyConfig.set_shake);
	}
	else
	{
		SubShake();//震动减少函数调用
		--KeyConfig.set_shake;					
		printf("\t当前频率调节为等级：%d\r\n", KeyConfig.set_shake);
	}	
}	

/*
* @brief 监测按键按下
* @param  no
* @return no
*/
void KeyPolling(void)
{	

	if( Key_Scan_down(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == 1  )
	{
	   setFunction();    //功能设置      
	}	
	
	else if( Key_Scan_down(KEY3_GPIO_PORT,KEY3_GPIO_PIN) == 1  )
	{
       switchMode();    //模式切换
	}		

	else if( Key_Scan_down(KEY4_GPIO_PORT,KEY4_GPIO_PIN) == 1 )
	{
	    flag_volume=0;
		printf("\n按下加号键!\r\n");
		switch(KeyConfig.set_parameter)
		{
			case SET_CLOSE:	    printf("请选择设置模式\r\n"); break;
			case SET_VOLUME:	volumeUp();   				  break;    //调高音量
			case SET_FREQUENCY: rateUp();  			     	  break;    //调高频率
			case SET_DISTANCE: 	distanceUp();				  break;    //增大监测距离
			case SET_SHAKE: 	shakeUp();				      break;    //增大震动等级
			default :printf("KEY5 error! KeyConfig.set_parameter = %d\r\n", KeyConfig.set_parameter);
		}			
	}		
	else if( Key_Scan_down(KEY5_GPIO_PORT,KEY5_GPIO_PIN) == 1 )
	{
		flag_volume=0;
			printf("\n按下减号键!\r\n");
		switch(KeyConfig.set_parameter)
		{
			case SET_CLOSE:	    printf("请选择设置模式\r\n"); break;
			case SET_VOLUME:	volumeDown();  				  break;    //下调音量
			case SET_FREQUENCY: rateDown();  				  break;    //下调频率
			case SET_DISTANCE: 	distanceDown();				  break;    //减小监测距离
			case SET_SHAKE: 	shakeDown();				  break;    //减小震动等级
			default :printf("KEY5 error! KeyConfig.set_parameter = %d\r\n", KeyConfig.set_parameter);
		}			
    }		

}




















