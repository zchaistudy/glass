#ifndef __MPU6050_H
#define	__MPU6050_H


#include "stm32f10x.h"

void MPU6050Config(void);

void MPU6050Triaxial(float Angle[4]);

void Filter(float Angle[4]);
#endif /* __MPU6050_H */
