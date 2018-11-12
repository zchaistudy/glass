#ifndef __KEY_H
#define	__KEY_H


struct key{
	int current_mode;   //当前提示模式
    int set_parameter;  //当前调节参数
	int set_volume;     //当前音量等级
	int set_frequency;  //当前频率等级
	int set_shake;      //当前震动等级
	int set_distance;   //当前距离等级
};

//提示模式
#define MODE_VOLUME     0	//音量模式
#define MODE_FREQUENCY 	1   //频率模式
#define MODE_SHAKE      2   //震动模式
#define MODE_MAX        3   //模式数量

//参数设置
#define SET_CLOSE       0//关闭参数设置
#define SET_VOLUME      1//音量参数设置
#define SET_FREQUENCY 	2 //频率参数设置
#define SET_DISTANCE    3 //距离参数设置
#define SET_SHAKE       4 //震动参数设置
#define SET_MAX         5  //5种参数设置

//参数最大值
#define MAX_RANK_VOLUME       15     //最大音量等级
#define MAX_RANK_FREQUENCY    3      //最大频率等级
#define MAX_RANK_DISTANCE     6      //最大距离等级
#define MAX_RANK_SHAKE        3      //最大震动等级

//参数最小值
#define MIN_RANK_VOLUME       1     //最小音量等级
#define MIN_RANK_FREQUENCY    1      //最小频率等级
#define MIN_RANK_DISTANCE     1      //最小距离等级
#define MIN_RANK_SHAKE        1      //最小震动等级

void Key_Config(void);
void KeyPolling(void);



#endif

