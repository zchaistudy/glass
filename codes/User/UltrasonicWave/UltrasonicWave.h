#ifndef __UltrasonicWave_H
#define	__UltrasonicWave_H

#include "stm32f10x.h"
#include "bsp_usart1.h"
#include "UltraConfig.h"

#define ONLY_GLASS            //眼镜单独测试


#define AVER_NUM_GLASS        2              //眼睛上超声波数量
#define AVER_NUM_WALK         5              //拐杖上超声波数量
#define AVER_NUM_ALL          7    //超声波总数量
#define LATE_NUM              3               //保留最近？次数据

#define OBSTACLE_HEAD            3
#define OBSTACLE_AHEAD           2
#define OBSTACLE_FOOT            1
#define OBSTACLE_NO              0

extern int8_t  GET_WALK_FLAG;

int getRange(void);

int minusDistance(void);
int addDistance(void);

void UltrasonicWave_Configuration(void);               //对超声波模块初始化
void UltrasonicWave(int);

void HasObstacle(void);
#endif /* __UltrasonicWave_H */

