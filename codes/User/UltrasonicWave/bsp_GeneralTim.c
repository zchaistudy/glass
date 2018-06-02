
#include "bsp_GeneralTim.h"

// ��ʱ�����벶���û��Զ�������ṹ�嶨��
TIM_ICUserValueTypeDef TIM_ICUserValueStructure = {0,0,0,0};
// �ж����ȼ�����


static void TIM3_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 	
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn ;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


static void TIM3_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // ���벶��ͨ�� GPIO ��ʼ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

static void TIM3_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
  // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	GENERAL_TIM_APBxClock_FUN(RCC_APB1Periph_TIM3,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/	
  
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_PERIOD;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	/*--------------------���벶��ṹ���ʼ��-------------------*/	
	// ���벶���źŵļ�������
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// �������Ҫ��������źŵķ�Ƶϵ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// �������Ҫ��������źŵ��˲�ϵ��
	TIM_ICInitStructure.TIM_ICFilter = 0;
		
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(TIM3, &TIM_ICInitStructure);	
		// �������벶���ͨ������Ҫ���ݾ����GPIO������
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	// ������ºͲ����жϱ�־λ
	TIM_ClearFlag(TIM3, TIM_FLAG_Update|TIM_IT_CC1);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update|TIM_IT_CC2);

  // �������ºͲ����ж�  
	TIM_ITConfig (TIM3, TIM_IT_Update | TIM_IT_CC2, ENABLE );
	TIM_ITConfig (TIM3, TIM_IT_Update | TIM_IT_CC1, ENABLE );
	// ʹ�ܼ�����
	TIM_Cmd(TIM3, ENABLE);
}


void GENERAL_TIM_Init(void)
{

	TIM3_GPIO_Config();
	TIM3_NVIC_Config();
	TIM3_Mode_Config();
}
void TIM2_Init()
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = 9999; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =7199; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM2, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx����
							 
}


/*********************************************END OF FILE**********************/
