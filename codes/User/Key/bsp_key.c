//����Ӳ������
#include "./key/bsp_key.h"  
#include "./usart/bsp_usart1.h"
#include "./systick/bsp_SysTick.h"
#include "mp3.h"

extern int flag_FALLING;

void delay_key(int i)
{
	for(;i>0;i--);
}


/**
  * @brief  ���ð����õ���I/O��
  * @param  ��
  * @retval ��
  */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/*���������˿ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK|KEY2_GPIO_CLK|KEY3_GPIO_CLK|KEY4_GPIO_CLK|KEY5_GPIO_CLK,ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//������������������Ϊ50MHz

	
	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 					//K1
	// ���ð���������Ϊ��������
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 					//K2
	//���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
	
		//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN; 					//K3
	//���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);	

	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN; 					//K4
	//���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);	

	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY5_GPIO_PIN; 					//K5
	//���ð���������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY5_GPIO_PORT, &GPIO_InitStructure);	
}


static void NVIC_Configuration2(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* �����ж�Դ������1 */
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
  /* ������ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* ���������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/*��������GPIO�ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK,ENABLE);
												
	/* ���� NVIC �ж�*/
	NVIC_Configuration2();
	  
/*--------------------------KEY1����-----------------------------*/
	/* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
  /* ����Ϊ�������� */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

	/* ѡ��EXTI���ź�Դ */
  GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
	
	/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* ���ж� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* ʹ���ж� */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}


/*********************************************END OF FILE**********************/


