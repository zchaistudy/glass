/***************************************************************************************
 *	File Name				:		delay.c
 *	CopyRight				:
 *	ModuleName				:	
 *
 *	CPU						:
 *	RTOS					:
 *
 *	Create Data				:	
 *	Author/Corportation		:	
 *
 *	Abstract Description	:	��ʱ����ͷ�ļ� 
 *
 *--------------------------------Revision History--------------------------------------
 *	No	version		Data			Revised By			Item			Description
 *	
 *
 ***************************************************************************************/


/**************************************************************
*	Multi-Include-Prevent Section
**************************************************************/
#ifndef __DELAY_H
#define __DELAY_H

 
/**************************************************************
*	Include File Section
**************************************************************/
#include "stm32f10x.h"

/**************************************************************
*	Prototype Declare Section
**************************************************************/
/**
*  @name  : void delayInit()
*	@description : ��ʼ����ʱ����
 *	@param		��none
 *	@return		:none
 *  @notice   : none
 */
 void delayInit(void);
 
 /**
 *  @name    void delayUs( uint32_t t )
 *	@description   us��ʱ
 *	@param		t ��ʱtus	
 *	@return		none
 *  @notice   t us<=��2^24��/fac_us
 */
void delayUs( uint32_t t );

/**
 *  @name   void delayMs(uint16_t t)
 *	@description    ms��ʱ
 *	@param		t ��ʱtms
 *	@return		none
 *  @notice  �� �����ʱ t<=0xffffff*8*1000/SYSCLK
 */
void delayMs(uint16_t t);

/**************************************************************
*	End-Multi-Include-Prevent Section
**************************************************************/
#endif
