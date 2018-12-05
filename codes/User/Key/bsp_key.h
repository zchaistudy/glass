#ifndef __KEY_H
#define	__KEY_H
#include "stm32f10x.h"

//��һ��ģ�����ģ��ƽʱ����͵�ƽ
//����ʱ����ߵ�ƽ

//��һ��ģ��෴���ɿ�����ߵ�ƽ
//��������͵�ƽ

#define KEY_DOWN 1	//�������±�ʾ�ߵ�ƽ
#define KEY_UP 0		//�����ɿ���ʾ�͵�ƽ

#define iCOUNT 10000	//�жϵ���ʱ
#define iTick 10	//��ѯ����ʱ

#define BREAK_EXTI_OPEN 0	//1Ϊ���жϵĿ��Ʒ�ʽ��0Ϊ����ѯ�Ŀ��Ʒ�ʽ


//����
#define DEBUG_ON_OFF_CYM 0
#define p_debug_cym(restrict,args...)  do{if(DEBUG_ON_OFF_CYM){printf(restrict,##args);}}while(0)
#define p_err_cym(restrict,args...)  do{if(DEBUG_ON_OFF_CYM){printf("\nfile:%s\nfunction:%s\t\tline:%d:\n",__FILE__,__func__,__LINE__);printf(restrict,##args);}}while(0)


//--------------------------------------------------------------------------------
//����������ģʽ
#define MODE_VOLUME     0	//����ģʽ
#define MODE_FREQUENCY 	1 //Ƶ��ģʽ
#define MODE_SHAKE   2 //��ģʽ
#define MODE_DISTANCE 3  //����ģʽ

//����������״̬�ȼ�
#define NO_RANK			0	//0�ȼ�
#define LOW_RANK		1	//�͵ȼ�
#define MID_RANK		2 //�еȼ�
#define UP_RANK			3 //�ߵȼ�

#define MAX_MODE 3      //����ģʽ ��Ƶģʽ  ����ģʽ

//���ò���
#define SET_CLOSE       0//�ر����ò���
#define SET_VOLUME      1//�������ò���
#define SET_FREQUENCY 	2 //Ƶ�����ò���
#define SET_DISTANCE    3 //�������ò���
#define SET_SHAKE       4 //�����ò���
#define SET_MAX         5  //5�����ò���


//�������ֵ
#define MAX_RANK_VOLUME       15     //��������ȼ�
#define MAX_RANK_FREQUENCY    3      //���Ƶ�ʵȼ�
#define MAX_RANK_DISTANCE     6      //������ȼ�
#define MAX_RANK_SHAKE        3      //����𶯵ȼ�

#define MIN_RANK        1      //������ȼ�

typedef struct key{
	int current_mode;//��ǰģʽ
	int key_rank[SET_MAX];//ÿ��ģʽ�ĵȼ�
	int max_mode;//���ģʽ��2��0 1 2   MODE_VOLUME MODE_FREQUENCY MODE_DISTANCE
	int max_rank;//���ȼ���4��0 1 2 3 NO_RANK LOW_RANK MID_RANK UP_RANK
	int min_mode;//��Сģʽ��0
	int min_rank;//��С�ȼ���0
	uint8_t set_parameter; //��ǰ���ò���
}key_four;





#if BREAK_EXTI_OPEN

//���Ŷ���
#define KEY1_INT_GPIO_PORT         GPIOA
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_0
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource0
#define KEY1_INT_EXTI_LINE         EXTI_Line0
#define KEY1_INT_EXTI_IRQ          EXTI0_IRQn

#define KEY1_IRQHandler            EXTI0_IRQHandler


#define KEY2_INT_GPIO_PORT         GPIOB
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_1
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource1
#define KEY2_INT_EXTI_LINE         EXTI_Line1
#define KEY2_INT_EXTI_IRQ          EXTI1_IRQn

#define KEY2_IRQHandler            EXTI1_IRQHandler


#define KEY3_INT_GPIO_PORT         GPIOB
#define KEY3_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEY3_INT_GPIO_PIN          GPIO_Pin_2
#define KEY3_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY3_INT_EXTI_PINSOURCE    GPIO_PinSource2
#define KEY3_INT_EXTI_LINE         EXTI_Line2
#define KEY3_INT_EXTI_IRQ          EXTI2_IRQn


#define KEY3_IRQHandler            EXTI2_IRQHandler


#define KEY4_INT_GPIO_PORT         GPIOD
#define KEY4_INT_GPIO_CLK          (RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO)
#define KEY4_INT_GPIO_PIN          GPIO_Pin_3
#define KEY4_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOD
#define KEY4_INT_EXTI_PINSOURCE    GPIO_PinSource3
#define KEY4_INT_EXTI_LINE         EXTI_Line3
#define KEY4_INT_EXTI_IRQ          EXTI3_IRQn

#define KEY4_IRQHandler            EXTI3_IRQHandler

#define KEY5_INT_GPIO_PORT         GPIOD
#define KEY5_INT_GPIO_CLK          (RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO)
#define KEY5_INT_GPIO_PIN          GPIO_Pin_4
#define KEY5_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOD
#define KEY5_INT_EXTI_PINSOURCE    GPIO_PinSource4
#define KEY5_INT_EXTI_LINE         EXTI_Line4
#define KEY5_INT_EXTI_IRQ          EXTI4_IRQn

#define KEY5_IRQHandler            EXTI4_IRQHandler


void EXTI_Key_Config(void);
	
#else

#define KEY1_INT_GPIO_PORT         GPIOA
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_0
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOA
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource0
#define KEY1_INT_EXTI_LINE         EXTI_Line0
#define KEY1_INT_EXTI_IRQ          EXTI0_IRQn

#define KEY1_IRQHandler            EXTI0_IRQHandler


//  ���Ŷ���
#define    KEY1_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY1_GPIO_PORT    GPIOA			   
#define    KEY1_GPIO_PIN		 GPIO_Pin_0
 
#define    KEY2_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEY2_GPIO_PORT    GPIOB		   
#define    KEY2_GPIO_PIN		  GPIO_Pin_1

#define    KEY3_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEY3_GPIO_PORT    GPIOB			   
#define    KEY3_GPIO_PIN		 GPIO_Pin_2

#define    KEY4_GPIO_CLK     RCC_APB2Periph_GPIOB
#define    KEY4_GPIO_PORT    GPIOB
#define    KEY4_GPIO_PIN		 GPIO_Pin_0

#define    KEY5_GPIO_CLK     RCC_APB2Periph_GPIOA
#define    KEY5_GPIO_PORT    GPIOA
#define    KEY5_GPIO_PIN		 GPIO_Pin_1




#define TRUE 1
#define FALSE 0

void EXTI_Key_Config(void);

void Key_GPIO_Config(void);
void KeyPolling(void);
	

#endif




#endif /* __KEY_H */

