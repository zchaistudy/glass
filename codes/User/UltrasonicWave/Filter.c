/**
* 避障数据滤波处理
**/
#include "Fliter.h"
#include<stdlib.h>

static struct Filter smooth; 

 /**
* @brief Init_Smooth_n   n次滤波处理初始化
* @param size  分别对size组数据进行滤波优化
* @param inital   数据初始值
* @return no
*/
void Init_Smooth_n(int size, smooth_type inital,int degree)
{
	int i;
	smooth.size = size;
	smooth.degree = degree;
	smooth.data = (smooth_type*)malloc(sizeof(smooth_type) * degree*size);
	for( i = 0; i < size*degree; i++ )
		smooth.data[i] = inital;
}


 /**
* @brief smoothAverage   对数据进行滤波,
* @param Measurement   测量数据
* @param num  数据组别 num: 0~x
* @return 滤波数据
*/
smooth_type smooth_n( int num, smooth_type data)
{
	int i;
	double temp;
	if( num >= smooth.size )     //数据不在已有数据组别内，直接返回原值
		return data;
	smooth.index = (smooth.index+1) %smooth.degree;
	smooth.data[num*smooth.degree+smooth.index] = data; 
	for( i = 0, temp = 0; i < smooth.degree; i++ )
	{
		printf("##: %d", smooth.data[num*smooth.degree+i]);
		temp += smooth.data[num*smooth.degree+i];
	}	
	printf("\r\n");
	return temp/smooth.degree + 0.5;
}

