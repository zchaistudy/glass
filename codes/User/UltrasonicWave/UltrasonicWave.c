/******************** (C) 1209 Lab **************************
 * �ļ���  : UltrasonicWave.c
 * ����    �����������ģ�飬UltrasonicWave_Configuration��������
             ��ʼ������ģ�飬UltrasonicWave_StartMeasure��������
			 ������࣬������õ�����ͨ������1��ӡ����         
 * ʵ��ƽ̨��Mini STM32������  STM32F103RBT6
 * Ӳ�����ӣ�------------------
 *          | PC8  - TRIG      |
 *          | PC7  - ECHO      |
 *           ------------------
 * ��汾  ��ST3.5.0
 *
 * ����    ��Lee 
*********************************************************************************/

#include "UltrasonicWave.h"
#include "debug.h"
#include "mp3.h"
#include "Kalman.h"
#include "Fliter.h"

////////���Կ���//////////////
#ifdef DEBUG_ON_OFF 
#undef  DEBUG_ON_OFF
#endif
#define DEBUG_ON_OFF 0      //1�򿪵��ԡ�0�ر�
//////////////////////////////

//�ϰ���������жϣ���distance>MAX_DISTACE����obstacleNum++��obstacleNum���ֵΪLATE_NUM
#define    HAS_OBSTACLE_NUM(distance,obstacleNum)  do\
			 {if(distance<MAX_DISTACE)\
			    {if(obstacleNum<LATE_NUM){obstacleNum++;}}\
			  else{obstacleNum=0;}}while(0)

			  
		
#define    OBSTACLE_LEFT_SIDE      1     //�ϰ��������
#define    OBSTACLE_RIGHT_SIDE     0     //�ϰ������ұ�

int UltrasonicWave_Distance_Walk[AVER_NUM_WALK] = { 500, 500, 500, 500, 500};   //���Ȳɼ�����
int MODE_FLAG = 1;       //1 ���� 0 Ƶ�� 2 ��
int8_t  MEASURE_FLAG = 1;   // 1 �۾��ɼ����ݣ� 0 �ȴ����Ȳɼ�����
int8_t GET_WALK_FLAG = 0;       //���չ������ݱ�־

extern int flag_volume;  
extern int flag_FALLING;             
static int UltrasonicWave_Distance[AVER_NUM_GLASS];      //������ľ���    
static int16_t MAX_DISTACE =150;        //������
static int8_t lateobstacle[AVER_NUM_WALK+AVER_NUM_GLASS] = {0};      //��¼������β���ϰ���״̬����������ϰ���ʱ+1��2δ��⵽�ϰ���ʱ����

static void UltrasonicWave_StartMeasure(GPIO_TypeDef *  port, int32_t pin); 





//��ʱ����
static void ObstacleDelayUs( uint32_t t )
{

	int i;
    for( i = 0; i < t * 10; i++ )
 	{
		__NOP();
	}	
}

/*
 * ��������addDistance
 * ����  �����Ӽ����룬 ����ȡֵΪ300 200 100
 * ����  ��num ��ⷶΧ����num����
 * ���  ���޸ĺ�ļ�ⷶΧ
 */
int addDistance()
{
	MAX_DISTACE += 50;
	if( MAX_DISTACE > 300 )           //����ⷶΧΪ3oocm	
		MAX_DISTACE = 300;
	return MAX_DISTACE;
}

/*
 * ��������addDistance
 * ����  ����С�����룬����ȡֵΪ300 200 100
 * ����  ��distance ��ⷶΧ��Сdistanc����
 * ���  ���޸ĺ�ļ�ⷶΧ
 */
int minusDistance()
{
	MAX_DISTACE -= 50;
	if( MAX_DISTACE < 50  )
		MAX_DISTACE = 50;		
	return MAX_DISTACE;
}

/*
 * ��������getDistance
 * ����  ����õ�ǰ��ⷶΧ
 * ����  ����
 * ���  ����ǰ�ļ�ⷶΧ
 */
int getDistance()
{
	return MAX_DISTACE;
}

/*
 * ��������dealTIM_ICUserValueStructureData
 * ����  ������׽����ʱ������ת��Ϊ����
 * ����  ��TIM_ICUserValueStructurex ��TIM_ICUserValueStructure1-6
			i����Ӧ���������
 * ���  ����	
 */
static void dealTIM_ICUserValueStructureData(TIM_ICUserValueTypeDef TIM_ICUserValueStructurex)
{

	double ftime;
	int i;
	
	i = TIM_ICUserValueStructurex.Capture_CCx -1;
	ftime = ((double) TIM_ICUserValueStructurex.Capture_CcrValue+1)/TIM_PscCLK;
	UltrasonicWave_Distance[i] = ftime * 340 / 2  * 100;
	UltrasonicWave_Distance[i] = KalmanFilter(i, UltrasonicWave_Distance[i]); //�˲�
//	p_debug( "\r\n%d : distance %d\r\n",i, UltrasonicWave_Distance[i]);
}

/**
*�ϰ����ж�
*distance �ɼ��ľ�������
*distanceVoice ����ģʽ���ϰ�����ʾ�� 0 ���ϰ��� 1 �ϰ����ڽ��� 2 �ϰ�������ǰ�� 3�ϰ�����ͷ��λ��
*sides         ����ģʽ�£�OBSTACLE_LEFT_SIDE���ϰ������ұߣ� OBSTACLE_RIGHT_SIDE:�ϰ��������
*distanceRate Ƶ��ģʽ���ϰ�����ʾ�� 0 ���ϰ��� 1 �ϰ�����3~2m�� 2 �ϰ�����2~1m ,3 �ϰ���<1m
*/
static void Obstacle(int distance_glass[], int distance_walk[], int* distanceVoice,int *sides, int* distanceRate )
{
	
	
	int i = 0; 
	int mindistace = 300 ;    //��¼������ϰ������

	
	*distanceVoice = OBSTACLE_NO;
	*distanceRate = 0;
	*sides = 0;
	
	distance_walk[4] = 500;      //���һ��������ģ�����ݲ���
	
	
	
//ͷ���ϰ������ͳ��	
	HAS_OBSTACLE_NUM(distance_glass[0],lateobstacle[0]);	
	HAS_OBSTACLE_NUM(distance_glass[1],lateobstacle[1]);	
//�����ϰ������ͳ��
	HAS_OBSTACLE_NUM(distance_walk[0],lateobstacle[2]);	
	HAS_OBSTACLE_NUM(distance_walk[1],lateobstacle[3]);	
	HAS_OBSTACLE_NUM(distance_walk[2],lateobstacle[4]);	
	HAS_OBSTACLE_NUM(distance_walk[3],lateobstacle[5]);		
	HAS_OBSTACLE_NUM(distance_walk[4],lateobstacle[6]);	
	
	
	p_debug("%d ",distance_glass[0]);
	p_debug("%d   ",distance_glass[1]);
//	p_debug("\r\ndistance  ");
	for( i = 0; i < 5;i++ )
	{
		p_debug("%d@",distance_walk[i]);
		distance_walk[i] = KalmanFilter(AVER_NUM_GLASS+i, distance_walk[i]);
		p_debug("%d ",distance_walk[i]);
	}
	
	p_debug("\r\n");
 //   p_debug("macx: %d", MAX_DISTACE);
 
//�ж�ͷ���Ƿ����ϰ���
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
//�ж�ǰ���Ƿ����ϰ���
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
//�жϽ����Ƿ����ϰ���
	else if( lateobstacle[4] == LATE_NUM )
	{
		*distanceVoice = OBSTACLE_FOOT;
		*sides = OBSTACLE_RIGHT_SIDE;
	}
	else if( lateobstacle[5] == LATE_NUM && lateobstacle[6] == LATE_NUM )
	{
		*distanceVoice = OBSTACLE_FOOT;
		*sides = OBSTACLE_LEFT_SIDE;
	}	
/***************Ƶ��ģʽ***********************/	
//Ƶ��ģʽ���ϰ�����ʾ,ȡ����ϰ������
//	for( i = 0; i < AVER_NUM_GLASS; i++ )                
//	{
//		mindistace = mindistace > distance_glass[i] ? distance_glass[i] : mindistace;
//	}
//	for( i = 0; i < AVER_NUM_WALK; i++ )
//	{
//		mindistace = mindistace > distance_walk[i] ? distance_walk[i] : mindistace;
//	}	

	mindistace = distance_glass[0] > distance_glass[1]?distance_glass[0] :distance_glass[1];
	if( distance_walk[0] < distance_walk[1] )
	{
		mindistace =  mindistace > distance_walk[1]? distance_walk[1]:mindistace;
	}
	else
	{
		mindistace =  mindistace > distance_walk[0]? distance_walk[0]:mindistace;
	}
	if( distance_walk[2] < distance_walk[3] )
	{
		mindistace =  mindistace > distance_walk[3]? distance_walk[3]:mindistace;
	}
	else
	{
		mindistace =  mindistace > distance_walk[2]? distance_walk[2]:mindistace;
	}	
	mindistace =  mindistace > distance_walk[4]? distance_walk[4]:mindistace;
	*distanceRate = 2 - mindistace / 100 ;
  if( *distanceRate < 0 )
	{
		*distanceRate = 0;
	}  
//	*distanceRate = smooth_n(0,*distanceRate);        //ȥë��
/**********************�������飬�������*******************************/
	for( i = 0; i < AVER_NUM_GLASS; i++ )
	{
		distance_glass[i] = INT16_MAX;
	}
	for( i = 0; i < AVER_NUM_WALK; i++ )
	{
		distance_walk[i] = INT16_MAX;
	}	
}

//�ж��ϰ���λ�ã���������ʾ
void HasObstacle()
{
	int distanceVoice, distanceRate,side;

	Obstacle(UltrasonicWave_Distance, UltrasonicWave_Distance_Walk,
	    &distanceVoice,&side, &distanceRate );      //�����ϰ�����Ϣ

	if(flag_FALLING==1)   																//���ä�˴���ˤ��״̬����һֱ�������ѹ��ܣ����ڲ����ϰ�����ʾ����
			return ;
//	p_debug(" $$%d\r\n", distanceVoice);

	if( MODE_FLAG == 1)																			//�жϵ�ǰ��������ģʽ��
	{
		if(distanceVoice)
		{
			printf("��������ģ�� flag_volume = %d \r\n",flag_volume);
//			p_debug("\r\ndistanceVoice:%d  , %d\r\n", distanceVoice,side);
			PlayVoice(distanceVoice,side);                  //�޸�����ģʽ	
		}		
	}
	else if(MODE_FLAG == 0)
	{
		printf("Ƶ�� = %d , flag_volume = %d\r\n",distanceRate,flag_volume);
		PlayRate(distanceRate);                    //����Ƶ��ģʽ
	}
	else if(MODE_FLAG == 2)
	{
		printf("�𶯵ȼ���%d\r\n",distanceRate);
		AdjustVibrationFrequencyGlasses(distanceRate);   //��ģʽ
	}
}

/*
 * ��������UltrasonicWave_StartMeasure
 * ����  ����ʼ��࣬����һ��>10us�����壬Ȼ��������صĸߵ�ƽʱ��
 * ����  ��port = TRIG_PORTX ,pin = TRIG_PINX
 * ���  ����	
 */
static void UltrasonicWave_StartMeasure(GPIO_TypeDef *  port, int32_t pin)
{
  GPIO_SetBits(port,pin); 		  //��>10US�ĸߵ�ƽ�TRIG_PORT,TRIG_PIN��������define����?
  ObstacleDelayUs(11);		                      //��ʱ20US
  GPIO_ResetBits(port,pin);

}


/****************************************************************************
* ��    �ƣ�void UltrasonicWave��)
* ��    �ܣ�����������߳�
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
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
		TIM_ICUserValueStructure[1].Capture_FinishFlag = 0;
	}	
//��ʼ��࣬����һ��>10us�����壬
	UltrasonicWave_StartMeasure(TRIG_PORT1,TRIG_PIN1); 
	UltrasonicWave_StartMeasure(TRIG_PORT2,TRIG_PIN2);
}

	

/******************* (C) 1209 Lab *****END OF FILE************/
