#ifndef __MP3_H
#define	__MP3_H


#include "stm32f10x.h"

extern u8 Play[5];
extern u8 Call[5];
extern u8 Next[5];
extern u8 Last[5];
extern u8 Add5[];
extern u8 Sub[5];
extern u8 LF[7];
extern u8 MF[7];
extern u8 HF[7];
extern u8 CareFootay[7];
extern u8 CareHand[7];
extern u8 CareHead[7];

extern u8 East[7];
extern u8 South[7];
extern u8 West[7];
extern u8 North[7];
extern u8 SouthEast[7];
extern u8 SouthWest[7];
extern u8 NorthWest[7];
extern u8 NorthEast[7];





void USART3_Config(void);
void USART3_Send_String(u8 *p,u8 cnt);
#endif /* __MP3_H */
