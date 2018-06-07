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
#include "debug.h"
#include "mp3.h"

////////调试开关//////////////
#ifdef DEBUG_ON_OFF 
#undef  DEBUG_ON_OFF
#endif
#define DEBUG_ON_OFF 0       //1打开调试。0关闭
//////////////////////////////

static void UltrasonicWave_StartMeasure(GPIO_TypeDef *  port, int32_t pin);              

static int UltrasonicWave_Distance[AVER_NUM_GLASS];      //计算出的距离    
int UltrasonicWave_Distance_Walk[AVER_NUM_WALK] = { 500, 500, 500, 500, 500};   //拐杖采集数据
static int16_t MAX_DISTACE =150;        //最大距离
int8_t  MEASURE_FLAG = 0;   // 0 眼镜采集数据， 1 等待拐杖采集数据

int8_t GET_WALK_FLAG = 0;       //接收拐杖数据标志

static void Obstacle(int distance_glass[], int distance_walk[], int* distanceVoice, int* distanceRate );




/*
 * 函数名：dealTIM_ICUserValueStructureData
 * 描述  ：将捕捉到的时间数据转化为距离
 * 输入  ：TIM_ICUserValueStructurex ：TIM_ICUserValueStructure1-6
			i：对应超声波序号
 * 输出  ：无	
 */
static void dealTIM_ICUserValueStructureData(TIM_ICUserValueTypeDef TIM_ICUserValueStructurex)
{

//	uint32_t time;
	double ftime;
	int distanceVoice, distanceRate;
	int i;
	i = TIM_ICUserValueStructurex.Capture_CCx;
	// 计算高电平时间的计数器的值
//	time = TIM_ICUserValueStructurex.Capture_CcrValue+1;
	// 打印高电平脉宽时间
	ftime = ((double) TIM_ICUserValueStructurex.Capture_CcrValue+1)/TIM_PscCLK;
	UltrasonicWave_Distance[i-1] = ftime * 340 / 2  * 100;
	
	p_debug( "\r\n%d : distance %d\r\n",i, UltrasonicWave_Distance[i-1]);

	
	Obstacle(UltrasonicWave_Distance, UltrasonicWave_Distance_Walk,&distanceVoice, &distanceRate );      //分析障碍物信息

//	PlayRate(distanceRate);                    //调用频率模式
	PlayVoice(distanceVoice);                  //修改语音模式
}

/*
 * 函数名：addDistance
 * 描述  ：增加检测距离， 距离取值为300 200 100
 * 输入  ：num 检测范围增加num厘米
 * 输出  ：修改后的检测范围
 */
int addDistance()
{
	MAX_DISTACE += 100;
	if( MAX_DISTACE > 300 )           //最大检测范围为3oocm	
		MAX_DISTACE = 300;
	return MAX_DISTACE;
}


/*
 * 函数名：addDistance
 * 描述  ：减小检测距离，距离取值为300 200 100
 * 输入  ：distance 检测范围减小distanc厘米
 * 输出  ：修改后的检测范围
 */
int minusDistance()
{
	MAX_DISTACE -= 100;
	if( MAX_DISTACE < 100  )
		MAX_DISTACE = 100;		
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


//障碍物判断
//distance 采集的距离数据
//distanceVoice 语音模式下障碍物提示， 0 无障碍物 1 障碍物在脚下 2 障碍物在正前方 3障碍物在头部位置
//distanceRate 频率模式下障碍物提示， 0 无障碍物 1 障碍物在3~2m， 2 障碍物在2~1m ,3 障碍物<1m
static void Obstacle(int distance_glass[], int distance_walk[], int* distanceVoice, int* distanceRate )
{
	
	
	int i = 0; 
	int mindistace = 300 ;    //记录最近的障碍物距离
	static int8_t lateobstacle[4] = {0};      //记录最近几次测距障碍物状态，连续监测障碍物时+1，2未监测到障碍物时清零
	
	*distanceVoice = OBSTACLE_NO;
	*distanceRate = OBSTACLE_NO;
	
	
	for( ; i < AVER_NUM_GLASS; i++ )                  //眼镜部分数据障碍物判断
	{
		if( distance_glass[i] < MAX_DISTACE )           //判断头部是否有障碍物
		{
			lateobstacle[0]++;
			if( lateobstacle[0] > LATE_NUM )
			{
				lateobstacle[0] = LATE_NUM;
			}
		} 
		else
		{
			lateobstacle[0] = 0;
		}
		p_debug("              %d\r\n", distance_glass[i]);
	}
	
	if( distance_walk[0]  < MAX_DISTACE || distance_walk[1] < MAX_DISTACE )  
	{
		lateobstacle[1]++;
		if( lateobstacle[1] > LATE_NUM )
		{
			lateobstacle[1] = LATE_NUM;
		}		
	}
	else
	{
		lateobstacle[1] = 0;
	}	
	if( distance_walk[2]  < MAX_DISTACE || distance_walk[3] < MAX_DISTACE )  
	{
		lateobstacle[2]++;
		if( lateobstacle[2] > LATE_NUM )
		{
			lateobstacle[2] = LATE_NUM;
		}
	}
	else
	{
		lateobstacle[2] = 0;
	}	
	if( distance_walk[4]  < MAX_DISTACE  )  
	{
		lateobstacle[3]++;
		if( lateobstacle[3] > LATE_NUM )
		{
			lateobstacle[3] = LATE_NUM;
		}
	}
	else
	{
		lateobstacle[3] = 0;
	}	

//判断头部是否有障碍物
	if( lateobstacle[0] == LATE_NUM )
	{
		*distanceVoice = OBSTACLE_HEAD;
	}
//判断前面是否有障碍物
	if( lateobstacle[1] == LATE_NUM || lateobstacle[2] == LATE_NUM  )
	{
		*distanceVoice = OBSTACLE_AHEAD;
	}    	
//判断脚下是否有障碍物
	if( lateobstacle[3] == LATE_NUM )
	{
		*distanceVoice = OBSTACLE_FOOT;
	}
//频率模式下障碍物提示,取最近障碍物距离
	for( i = 0; i < AVER_NUM_GLASS; i++ )                
	{
		mindistace = mindistace > distance_glass[i] ? distance_glass[i] : mindistace;
	}	
	for( i = 0; i < AVER_NUM_WALK; i++ )
	{
		mindistace = mindistace > distance_walk[i] ? distance_walk[i] : mindistace;
	}
	*distanceRate = mindistace / 100;
    	
}




/*
 * 函数名：UltrasonicWave_StartMeasure
 * 描述  ：开始测距，发送一个>10us的脉冲，然后测量返回的高电平时间
 * 输入  ：port = TRIG_PORTX ,pin = TRIG_PINX
 * 输出  ：无	
 */
static void UltrasonicWave_StartMeasure(GPIO_TypeDef *  port, int32_t pin)
{
  GPIO_SetBits(port,pin); 		  //送>10US的高电平RIG_PORT,TRIG_PIN这两个在define中有?
  delayUs(11);		                      //延时20US
  GPIO_ResetBits(port,pin);

}


/****************************************************************************
* 名    称：void UltrasonicWave(void *arg)
* 功    能：超声波测距线程
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void UltrasonicWave(int portNum)
{
    if( TIM_ICUserValueStructure[0].Capture_FinishFlag == 1 )  
	{
	    dealTIM_ICUserValueStructureData(TIM_ICUserValueStructure[0]);
		TIM_ICUserValueStructure[0].Capture_FinishFlag = 0;
	}
    if( TIM_ICUserValueStructure[1].Capture_FinishFlag == 1 )  
	{
	    dealTIM_ICUserValueStructureData(TIM_ICUserValueStructure[1]);
		p_debug("test\r\n");
		TIM_ICUserValueStructure[1].Capture_FinishFlag = 0;
	}	
	switch(portNum)          //开始测距，发送一个>10us的脉冲，
	{
		case 0: UltrasonicWave_StartMeasure(TRIG_PORT1,TRIG_PIN1); break;
		case 1: UltrasonicWave_StartMeasure(TRIG_PORT2,TRIG_PIN2); break;
	}	
}




/******************* (C) 1209 Lab *****END OF FILE************/
