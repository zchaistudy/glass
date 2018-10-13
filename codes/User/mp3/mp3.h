#ifndef __MP3_H
#define	__MP3_H


#include "stm32f10x.h"
#include <stdio.h>

#define head 3
#define hand 2
#define foot 1

#define first 		1
#define second 		2
#define third 		3
#define forth 		4
#define fifth 		5

#define InEast 					'6'
#define InSouth 				'0'
#define InWest 					'2'
#define InNorth 				'4'
#define InSouthEast 		'7'
#define InSouthWest 		'1'
#define InNorthWest 		'3'
#define InNorthEast 		'5'

#define            TIM6_TIM                   TIM6
#define            TIM6_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            TIM6_TIM_CLK               RCC_APB1Periph_TIM6
#define            TIM6_TIM_Period            (1000-1)
#define            TIM6_TIM_Prescaler         71
#define            TIM6_TIM_IRQ               TIM6_IRQn
#define            TIM6_TIM_IRQHandler        TIM6_IRQHandler

extern u8 rate_1[5];
extern u8 Call[5];
extern u8 Next[5];
extern u8 Last[5];
extern u8 Add[5];
extern u8 Sub[5];

extern u8 rate_2[7];
extern u8 rate_3[7];
extern u8 rate_4[7];
extern u8 rate_5[7];
extern u8 CareFootLeft[7];
extern u8 CareHandLeft[7];
extern u8 CareHeadLeft[7];
extern u8 CareFootRight[7];
extern u8 CareHandRight[7];
extern u8 CareHeadRight[7];

extern u8 East[7];
extern u8 South[7];
extern u8 West[7];
extern u8 North[7];
extern u8 SouthEast[7];
extern u8 SouthWest[7];
extern u8 NorthWest[7];
extern u8 NorthEast[7];

extern u8 ModeVolume[7];
extern u8 ModeRate[7];
extern u8 ModeDistance[7];
extern u8 ModeExit[7];

extern u8 Alarm[7];
extern u8 AutoAlarm[7];
extern u8 SendSucceed[7];
extern u8 QuitAlarm[7];

extern u8 AdjustVolume[7];      
extern u8 AdjustRate[7];      
extern int Rate;



void USART3_Config(void);
void USART3_Send_String(u8 *p,u8 cnt);
void USART3_Send_String_Key(u8 *p,u8 cnt);

void AddRate(void);
void SubRate(void);

void AddVolume(void);
void SubVolume(void);

int Weighting(int degree);
void PlayVoice(int position,int left_right);
void PlayRate(int degree);
void PlayDirection(char direction);

void TIM6_Config(void);
void TIM6_TIM_NVIC_Config(void);
void TIM6_TIM_Mode_Config(void);
#endif /* __MP3_H */
