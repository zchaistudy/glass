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

//障碍物检测次数判断，若distance>MAX_DISTACE，则obstacleNum++，obstacleNum最大值为LATE_NUM
#define    HAS_OBSTACLE_NUM(distance,obstacleNum)  do\
			 {if(distance<MAX_DISTACE)\
			    {if(obstacleNum<LATE_NUM){obstacleNum++;}}\
			  else{obstacleNum=0;}}while(0)

			  
		
#define    OBSTACLE_LEFT_SIDE      1     //障碍物在左边
#define    OBSTACLE_RIGHT_SIDE     0     //障碍物在右边

static void UltrasonicWave_StartMeasure(GPIO_TypeDef *  port, int32_t pin);              
static int UltrasonicWave_Distance[AVER_NUM_GLASS];      //计算出的距离    
static int16_t MAX_DISTACE =150;        //最大距离
static int8_t lateobstacle[AVER_NUM_WALK+AVER_NUM_GLASS] = {0};      //记录最近几次测距障碍物状态，连续监测障碍物时+1，2未监测到障碍物时清零

int UltrasonicWave_Distance_Walk[AVER_NUM_WALK] = { 500, 500, 500, 500, 500};   //拐杖采集数据
int MODE_FLAG = 1;       //1 语音 0 频率
int8_t  MEASURE_FLAG = 1;   // 1 眼镜采集数据， 0 等待拐杖采集数据
int8_t GET_WALK_FLAG = 0;       //接收拐杖数据标志

extern int flag_FALLING;




static void ObstacleDelayUs( uint32_t t )
{

	int i;
    for( i = 0; i < t * 10; i++ )
 	{
		__NOP();
	}	
}

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
//	int distanceVoice, distanceRate;
	int i;
	i = TIM_ICUserValueStructurex.Capture_CCx;
	// 计算高电平时间的计数器的值
//	time = TIM_ICUserValueStructurex.Capture_CcrValue+1;
	// 打印高电平脉宽时间
	ftime = ((double) TIM_ICUserValueStructurex.Capture_CcrValue+1)/TIM_PscCLK;

	UltrasonicWave_Distance[i-1] = ftime * 340 / 2  * 100;
	
//
	p_debug( "\r\n%d : distance %d\r\n",i, UltrasonicWave_Distance[i-1]);

	
//	Obstacle(UltrasonicWave_Distance, UltrasonicWave_Distance_Walk,&distanceVoice, &distanceRate );      //分析障碍物信息

////	PlayRate(distanceRate);                    //调用频率模式
//	PlayVoice(distanceVoice);                  //修改语音模式
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


/**
*障碍物判断
*distance 采集的距离数据
*distanceVoice 语音模式下障碍物提示， 0 无障碍物 1 障碍物在脚下 2 障碍物在正前方 3障碍物在头部位置
*sides         语音模式下，OBSTACLE_LEFT_SIDE：障碍物在右边， OBSTACLE_RIGHT_SIDE:障碍物在左边
*distanceRate 频率模式下障碍物提示， 0 无障碍物 1 障碍物在3~2m， 2 障碍物在2~1m ,3 障碍物<1m
*/
static void Obstacle(int distance_glass[], int distance_walk[], int* distanceVoice,int *sides, int* distanceRate )
{
	
	
	int i = 0; 
	int mindistace = 300 ;    //记录最近的障碍物距离

	
	*distanceVoice = OBSTACLE_NO;
	*distanceRate = 0;
	*sides = 0;
	
//头部障碍物次数统计	
	HAS_OBSTACLE_NUM(distance_glass[0],lateobstacle[0]);	
	HAS_OBSTACLE_NUM(distance_glass[1],lateobstacle[1]);	
//拐杖障碍物次数统计
	HAS_OBSTACLE_NUM(distance_walk[0],lateobstacle[2]);	
	HAS_OBSTACLE_NUM(distance_walk[1],lateobstacle[3]);	
	HAS_OBSTACLE_NUM(distance_walk[2],lateobstacle[4]);	
	HAS_OBSTACLE_NUM(distance_walk[3],lateobstacle[5]);		
	HAS_OBSTACLE_NUM(distance_walk[4],lateobstacle[6]);	
	
	p_debug("\r\ndistance  ");
	for( i = 0; i < 5;i++ )
	p_debug("%d ",distance_walk[i]);
    p_debug("macx: %d", MAX_DISTACE);
 
//判断头部是否有障碍物
	if( lateobstacle[0] == LATE_NUM)
	{
		*distanceVoice = OBSTACLE_HEAD;
		*sides = OBSTACLE_RIGHT_SIDE;
	}
	if( lateobstacle[1] == LATE_NUM)
	{
		*distanceVoice = OBSTACLE_HEAD;
		*sides = OBSTACLE_LEFT_SIDE;
	}	
//判断前面是否有障碍物
	if( lateobstacle[2] == LATE_NUM  )
	{
		*distanceVoice = OBSTACLE_AHEAD;
		*sides = OBSTACLE_LEFT_SIDE;
	}    	
	if( lateobstacle[3] == LATE_NUM  )
	{
		*distanceVoice = OBSTACLE_AHEAD;
		*sides = OBSTACLE_RIGHT_SIDE;
	}  	
//判断脚下是否有障碍物
	else if( lateobstacle[4] == LATE_NUM )
	{
		*distanceVoice = OBSTACLE_FOOT;
		*sides = OBSTACLE_RIGHT_SIDE;
	}
	else if( lateobstacle[5] == LATE_NUM || lateobstacle[6] == LATE_NUM )
	{
		*distanceVoice = OBSTACLE_FOOT;
		*sides = OBSTACLE_LEFT_SIDE;
	}	
/***************频率模式***********************/	
//频率模式下障碍物提示,取最近障碍物距离
	for( i = 0; i < AVER_NUM_GLASS; i++ )                
	{
		mindistace = mindistace > distance_glass[i] ? distance_glass[i] : mindistace;
	}	
	for( i = 0; i < AVER_NUM_WALK; i++ )
	{
		mindistace = mindistace > distance_walk[i] ? distance_walk[i] : mindistace;
	}
	*distanceRate = 2 - mindistace / 100 ;
  if( *distanceRate < 0 )
	{
		*distanceRate = 0;
	}  

/**********************重置数组，避免干扰*******************************/
	for( i = 0; i < AVER_NUM_GLASS; i++ )
	{
		distance_glass[i] = INT16_MAX;
	}
	for( i = 0; i < AVER_NUM_WALK; i++ )
	{
		distance_walk[i] = INT16_MAX;
	}	
}

//判断障碍物位置，并触发提示
void HasObstacle()
{
	int distanceVoice, distanceRate,side;

	Obstacle(UltrasonicWave_Distance, UltrasonicWave_Distance_Walk,
	    &distanceVoice,&side, &distanceRate );      //分析障碍物信息

	if(flag_FALLING==1)   //如果盲人处于摔倒状态，则一直播放提醒功能，不在播放障碍物提示功能
			return ;
//	p_debug(" $$%d\r\n", distanceVoice);

	if( MODE_FLAG )
	{
		if(distanceVoice)
		{
			printf("调用语音模快\r\n");
			p_debug("\r\ndistanceVoice:%d  , %d\r\n", distanceVoice,side);
			PlayVoice(distanceVoice,side);                  //修改语音模式	
		}		
	}
	else
	{
		printf("频率：%d\r\n",distanceRate);
		PlayRate(distanceRate);                    //调用频率模式
	}
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
  ObstacleDelayUs(11);		                      //延时20US
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
//		p_debug("test\r\n");
		TIM_ICUserValueStructure[1].Capture_FinishFlag = 0;
	}	
	switch(portNum)          //开始测距，发送一个>10us的脉冲，
	{
		case 0: UltrasonicWave_StartMeasure(TRIG_PORT1,TRIG_PIN1); break;
		case 1: UltrasonicWave_StartMeasure(TRIG_PORT2,TRIG_PIN2); break;
	}
//	 ObstacleDelayUs(6000);
//p_debug("ssd\r\n");	
}




/******************* (C) 1209 Lab *****END OF FILE************/
