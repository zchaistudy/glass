#ifndef __FILTER_H
#define	__FILTER_H

typedef int smooth_type;
struct Filter
{
	int size;     //滤波数据组别
	int degree ;       //滤波次数上限
	smooth_type* data;  //储存数据，需要另外开辟空间
	int index;         //最新数据存储位置
};

void Init_Smooth_n(int size, smooth_type inital,int degree);
smooth_type smooth_n( int num, smooth_type data);
#endif
