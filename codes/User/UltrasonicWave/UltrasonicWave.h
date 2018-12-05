#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H

#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "UltraConfig.h"

//#define ONLY_GLASS            //�۾���������


#define AVER_NUM_GLASS        2              //�۾��ϳ���������
#define AVER_NUM_WALK         5              //�����ϳ���������
#define AVER_NUM_ALL          7    //������������
#define LATE_NUM              3               //���������������

#define OBSTACLE_HEAD            3
#define OBSTACLE_AHEAD           2
#define OBSTACLE_FOOT            1
#define OBSTACLE_NO              0

extern int8_t  GET_WALK_FLAG;

int getRange(void);

int minusDistance(void);
int addDistance(void);

void UltrasonicWave_Configuration(void);               //�Գ�����ģ���ʼ��
void UltrasonicWave(int);

void HasObstacle(void);
#endif /* __UltrasonicWave_H */

