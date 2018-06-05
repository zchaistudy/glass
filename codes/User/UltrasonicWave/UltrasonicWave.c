/******************** (C) 1209 Lab **************************
 * 文件名  : UltrasonicWave.c
 * 描述    ：超声波测距模块，UltrasonicWave_Configuration（）函数
             初始化超声模块，UltrasonicWave_StartMeasure（）函数
			 启动测距，并将测得的数据通过串口1打印出来         
 * 实验平台：Mini STM32开发板  STM32F103RBT6
 * 硬件连接：------------------
 *          | PC8  - TRIG      |
 *          | PC7  - ECHO      |
 *           ------------------
 * 库版本  ：ST3.5.0
 *
 * 作者    ：Lee 
*********************************************************************************/

#include "UltrasonicWave.h"




static void UltrasonicWave_StartMeasure(GPIO_TypeDef *  port, int32_t pin);              

float UltrasonicWave_Distance[AVER_NUM];      //计算出的距离    
static int16_t MAX_DISTACE = 50;        //最大距离
int8_t  IT_TAG = 0;          //读取标志，为1时表示以读取到数据

/*
 * 函数名：UltrasonicWave_Configuration
 * 描述  ：超声波模块的初始化
 * 输入  ：无
 * 输出  ：无	
 */
void UltrasonicWave_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE);
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		     //设为推挽输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN1;					
	GPIO_Init(TRIG_PORT1, &GPIO_InitStructure);	
	GPIO_InitStructure.GPIO_Pin = TRIG_PIN2;					
	GPIO_Init(TRIG_PORT2, &GPIO_InitStructure);		
}



/*
 * 函数名：dealTIM_ICUserValueStructureData
 * 描述  ：将捕捉到的时间数据转化为距离
 * 输入  ：TIM_ICUserValueStructurex ：TIM_ICUserValueStructure1-6
			i：对应超声波序号
 * 输出  ：无	
 */
void dealTIM_ICUserValueStructureData(TIM_ICUserValueTypeDef TIM_ICUserValueStructurex, int i)
{

	uint32_t time;
	double ftime;
	// 计算高电平时间的计数器的值
	time = TIM_ICUserValueStructurex.Capture_CcrValue+1;
	// 打印高电平脉宽时间
	ftime = ((double)time)/TIM_PscCLK;
	UltrasonicWave_Distance[i] = ftime * 340 / 2  * 100;
//	printf( "\r\n time %d\r\n",time );	
//	printf( "\r\n ftime %lf\r\n",ftime );
	printf( "\r\n%d : distance %f\r\n",i, UltrasonicWave_Distance[i]);
//	printf( "\r\n：%d.%d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK );	
}



/*
 * 函数名：UltrasonicWave_StartMeasure
 * 描述  ：开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
 * 输入  ：port = TRIG_PORTX ,pin = TRIG_PINX
 * 输出  ：无	
 */
void UltrasonicWave_StartMeasure(GPIO_TypeDef *  port, int32_t pin)
{
  GPIO_SetBits(port,pin); 		  //送>10US的高电平RIG_PORT,TRIG_PIN这两个在define中有?
  delayUs(11);		                      //延时20US
  GPIO_ResetBits(port,pin);

}




/*
 * 函数名：addDistance
 * 描述  ：增加检测距离，最大不超过250厘米
 * 输入  ：num 检测范围增加num厘米
 * 输出  ：修改后的检测范围
 */
int addDistance(int distance)
{
	MAX_DISTACE += distance;
	if( MAX_DISTACE > 250 )
		MAX_DISTACE = 250;
	return MAX_DISTACE;
}


/*
 * 函数名：addDistance
 * 描述  ：减小检测距离，最大不超过250厘米
 * 输入  ：distance 检测范围减小distanc厘米
 * 输出  ：修改后的检测范围
 */
int minusDistance(int distance)
{
	MAX_DISTACE -= distance;
	if( MAX_DISTACE <30 )
		MAX_DISTACE = 30;
	return MAX_DISTACE;
}

/*
 * 函数名：getDistance
 * 描述  ：获得当前检测范围
 * 输入  ：无
 * 输出  ：当前的检测范围
 */
int getDistance()
{
	return MAX_DISTACE;
}


/****************************************************************************
* 名    称：void UltrasonicWave(void *arg)
* 功    能：超声波测距线程
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void UltrasonicWave(int* num)
{
	static int8_t tag = 0;	
    if( TIM_ICUserValueStructure.Capture_FinishFlag == 1 )  
	{
		TIM_ICUserValueStructure.Capture_FinishFlag = 0;
	    dealTIM_ICUserValueStructureData(TIM_ICUserValueStructure, tag);
	}	
	tag = (tag +1) % AVER_NUM;
	switch(tag)          //开始测距，发送一个>10us的脉冲，
	{
		case 0: UltrasonicWave_StartMeasure(TRIG_PORT1,TRIG_PIN1); break;
		case 1: UltrasonicWave_StartMeasure(TRIG_PORT2,TRIG_PIN2); break;
	}

	
}


/******************* (C) 1209 Lab *****END OF FILE************/
