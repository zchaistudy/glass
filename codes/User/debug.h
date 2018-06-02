#ifndef DEBUG_H_
#define DEBUG_H_
#include "stm32f10x.h"

//#define DEBUG  0
#define  DEBUG_ON_OFF  0 

void CloseDebug(void);   //�رյ���
void OnDebug(void);    //�򿪵���
/**
////////���Կ���//////////////
#ifdef DEBUG_ON_OFF 
#undef  DEBUG_ON_OFF
#endif
#define DEBUG_ON_OFF 1       //1�򿪵��ԡ�0�ر�
//////////////////////////////
*/
#define p_debug(restrict,args...)  do{if(DEBUG_ON_OFF){printf(restrict,##args);}}while(0)
#define p_err(restrict,args...)  do{if(DEBUG_ON_OFF){printf("file:%s line:%d: \r\n",__FILE__,__LINE__);printf(restrict,##args);}}while(0)

#endif
