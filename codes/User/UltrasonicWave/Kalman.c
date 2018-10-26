#include "Kalman.h"
#include "UltrasonicWave.h"

static KalmanInfo Kalman[AVER_NUM_ALL];

/**
* @brief Init_Kalman   初始化滤波器的初始值
*/
void Init_Kalman()
{
	int i;
	for( i = 0; i < AVER_NUM_ALL; ++i )
	{
		Kalman[i].P[0][0] = 1;
		Kalman[i].P[0][1] = 1;
		Kalman[i].P[1][0] = 1;
		Kalman[i].P[1][1] = 1;
		Kalman[i].Q[0] = 30;    //预测（过程）噪声方差 影响收敛速率，可以根据实际需求给出
		Kalman[i].Q[1] = 0.5; 
		Kalman[i].R = 60;    //测量（观测）噪声方差 可以通过实验手段获得
		Kalman[i].filterValue[0] = 200;// 测量的初始值
		Kalman[i].filterValue[1] = 1;// 测量的初始值
		Kalman[i].dt = 0.06;		
	}       
}

/**
* @brief Init_Kalman   对数据进行滤波
* @param Measurement   测量数据
* @param i  数据组别 
* @return 滤波数据
*/
KalmanType KalmanFilter(int i, KalmanType Measurement)
{
	//系统预测值
	Kalman[i].filterValue[0] +=  Kalman[i].filterValue[1]* Kalman[i].dt;           

    //求协方差
	Kalman[i].P[0][0] += Kalman[i].P[0][1]*Kalman[i].dt + Kalman[i].P[1][0]*Kalman[i].dt + Kalman[i].Q[0];
	Kalman[i].P[0][1] += Kalman[i].P[1][1]*Kalman[i].dt;
	Kalman[i].P[1][0] += Kalman[i].P[1][1]*Kalman[i].dt;
	Kalman[i].P[1][1] += Kalman[i].Q[1];

	//卡尔曼增益
	Kalman[i].kg[0] = Kalman[i].P[0][0] / (Kalman[i].P[0][0] + Kalman[i].R); 
	Kalman[i].kg[1] = Kalman[i].P[1][0] / (Kalman[i].P[1][0] + Kalman[i].R);

	//修正估计
    Kalman[i].filterValue[0] += Kalman[i].kg[0] * (Measurement -Kalman[i].filterValue[0]);

	//更新协方差
	Kalman[i].P[0][0] -= Kalman[i].P[0][0] * Kalman[i].kg[0];
	Kalman[i].P[0][1] -= Kalman[i].P[0][1] * Kalman[i].kg[0];
	Kalman[i].P[1][0] -= Kalman[i].P[0][0] * Kalman[i].kg[1];
	Kalman[i].P[1][1] -= Kalman[i].P[0][1] * Kalman[i].kg[1];
	
	return Kalman[i].filterValue[0];
}





