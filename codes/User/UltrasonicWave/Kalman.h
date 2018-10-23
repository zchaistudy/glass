#ifndef __KALMAN_H
#define	__KALMAN_H

typedef int KalmanType;

// 一维滤波器信息结构体
typedef  struct{
	KalmanType filterValue[2];  //k-1时刻的滤波值，即是k-1时刻的值,第一个值表示距离，第二个值表示速度
	double kg[2];   //   Kalamn增益 
	double Q[2];   //预测过程噪声偏差的方差, Q[0]距离估计误差 Q[1]速度估计误差
	double R;   //测量噪声偏差，(系统搭建好以后，通过测量统计实验获得)
	double P[2][2];   //估计误差协方差
	double dt;    //时间分量
}  KalmanInfo;



void Init_Kalman(void);
KalmanType KalmanFilter(int i,KalmanType Measurement);	

#endif
