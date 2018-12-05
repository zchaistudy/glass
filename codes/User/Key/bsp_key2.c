#ifdef HHH

#ifdef HHHHH
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
	
	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN; 
	// ���ð���������Ϊ��������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 

//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
	
	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN; 
	//���ð���������Ϊ��������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);	
	
		//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN; 
	//���ð���������Ϊ��������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);	

	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY4_GPIO_PIN; 
	//���ð���������Ϊ��������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY4_GPIO_PORT, &GPIO_InitStructure);	

	//ѡ�񰴼�������
	GPIO_InitStructure.GPIO_Pin = KEY5_GPIO_PIN; 
	//���ð���������Ϊ��������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
	//ʹ�ýṹ���ʼ������
	GPIO_Init(KEY5_GPIO_PORT, &GPIO_InitStructure);	
}

#define iTick 5000
static void delay(long i)
{
	for(;i>0;i--);
}

//delay(iTick);
//����ʱ����ߵ�ƽ���ɿ�ʱ����͵�ƽ
 /*
 * ��������Key_Scan
 * ����  ������Ƿ��а�������
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��KEY_OFF(û���°���)��KEY_ON�����°�����
 */
uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/* ����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON )  
	{	 
		delay(iTick);
		if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON)
		{
			/* �ȴ������ͷ� */
			while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_ON); 
				return 	KEY_ON;	 
		}
		else
			return KEY_OFF;			
	}
	else
		return KEY_OFF;
}

int KeyPolling(void)
{
	if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
	{
		printf("\n���¹���ѡ���");
		if(MODE_VOLUME == key4.current_mode)	//��ǰ������������ģʽ
		{
			printf("\n��ǰ��������������,��ǰ�����ȼ�Ϊ��%d", key4.key_rank[MODE_VOLUME]);
			//���ţ���ǰ������������ģʽ
		}
		else if(MODE_FREQUENCY == key4.current_mode)	//��ǰ����Ƶ�ʵ���ģʽ
		{
			printf("\n��ǰ����Ƶ�ʵ����У���ǰƵ�ʵȼ�Ϊ��%d", key4.key_rank[MODE_FREQUENCY]);			
			//���ţ���ǰ����Ƶ�ʵ���ģʽ
		}
		else if(MODE_DISTANCE == key4.current_mode)	//��ǰ���ھ������ģʽ
		{
			printf("\n��ǰ���ھ�������У���ǰ����ȼ�Ϊ��%d", key4.key_rank[MODE_DISTANCE]);
			//���ţ���ǰ���ھ������ģʽ
		}
//		else
//		{
//			printf("\nKEY2 error!");
//		}

		return KEY_MODE;
	}		
	
	if( Key_Scan(KEY3_GPIO_PORT,KEY3_GPIO_PIN) == KEY_ON  )
	{
		printf("\n���½�����һ��ģʽ��!");
		key4.current_mode=(++key4.current_mode)%key4.max_mode;

		return KEY_BACK;
	}		

	if( Key_Scan(KEY4_GPIO_PORT,KEY4_GPIO_PIN) == KEY_ON  )
	{
		printf("\n���¼Ӻż�");
		
		if(MODE_VOLUME == key4.current_mode)	//��ǰ������������ģʽ
		{
			if(key4.max_rank == key4.key_rank[MODE_VOLUME]+1)
			{
				printf("\n��ǰ��������Ϊ�ȼ���%d���ﵽ���ȼ������ٵ���", key4.key_rank[MODE_VOLUME]);				

			}
			else
			{
				//�������Ӻ�������
				key4.key_rank[MODE_VOLUME]=(++key4.key_rank[MODE_VOLUME])%key4.max_rank;					
				printf("\n��ǰ��������Ϊ�ȼ���%d", key4.key_rank[MODE_VOLUME]);
			}

		}
		else if(MODE_FREQUENCY == key4.current_mode)	//��ǰ����Ƶ�ʵ���ģʽ
		{
			if(key4.max_rank == key4.key_rank[MODE_FREQUENCY]+1)
			{
				printf("\n��ǰƵ�ʵ���Ϊ�ȼ���%d���ﵽ���ȼ������ٵ���", key4.key_rank[MODE_FREQUENCY]);			
			}
			else
			{
				//Ƶ�����Ӻ�������
				key4.key_rank[MODE_FREQUENCY]=(++key4.key_rank[MODE_FREQUENCY])%key4.max_rank;					
				printf("\n��ǰƵ�ʵ���Ϊ�ȼ���%d", key4.key_rank[MODE_FREQUENCY]);
			}

		}
		else if(MODE_DISTANCE == key4.current_mode)	//��ǰ���ھ������ģʽ
		{
			if(key4.max_rank == key4.key_rank[MODE_DISTANCE]+1)
			{			
				printf("\n��ǰ�������Ϊ�ȼ���%d���ﵽ���ȼ������ٵ���", key4.key_rank[MODE_DISTANCE]);
			}
			else
			{
				//�������Ӻ�������
				key4.key_rank[MODE_DISTANCE]=(++key4.key_rank[MODE_DISTANCE])%key4.max_rank;					
				printf("\n��ǰ�������Ϊ�ȼ���%d", key4.key_rank[MODE_DISTANCE]);
			}
		}
//		else
//		{
//			printf("KEY4 error!");
//		}

		return KEY_ADD;
	}		

	if( Key_Scan(KEY5_GPIO_PORT,KEY5_GPIO_PIN) == KEY_ON  )
	{
		printf("\n���¼��ż�");
		
		if(MODE_VOLUME == key4.current_mode)	//��ǰ������������ģʽ
		{
			if(key4.min_rank == key4.key_rank[MODE_VOLUME])
			{
				printf("\n��ǰ��������Ϊ�ȼ���%d���ﵽ��С�ȼ������ٵ�С", key4.key_rank[MODE_VOLUME]);
			}
			else
			{
				//�������ٺ�������
				key4.key_rank[MODE_VOLUME]=(--key4.key_rank[MODE_VOLUME])%key4.max_rank;					
				printf("\n��ǰ��������Ϊ�ȼ���%d", key4.key_rank[MODE_VOLUME]);
			}				
		}
		else if(MODE_FREQUENCY == key4.current_mode)	//��ǰ����Ƶ�ʵ���ģʽ
		{
			if(key4.min_rank == key4.key_rank[MODE_FREQUENCY])
			{
				printf("\n��ǰƵ�ʵ���Ϊ�ȼ���%d���ﵽ��С�ȼ������ٵ�С", key4.key_rank[MODE_FREQUENCY]);
			}
			else
			{
				//Ƶ�ʼ��ٺ�������
				key4.key_rank[MODE_FREQUENCY]=(--key4.key_rank[MODE_FREQUENCY])%key4.max_rank;					
				printf("\n��ǰƵ�ʵ���Ϊ�ȼ���%d", key4.key_rank[MODE_FREQUENCY]);
			}
		}
		else if(MODE_DISTANCE == key4.current_mode)	//��ǰ���ھ������ģʽ
		{
			if(key4.min_rank == key4.key_rank[MODE_DISTANCE])
			{
				printf("\n��ǰ�������Ϊ�ȼ���%d���ﵽ��С�ȼ������ٵ�С", key4.key_rank[MODE_DISTANCE]);			
			}
			else
			{
				//������ٺ�������
				key4.key_rank[MODE_DISTANCE]=(--key4.key_rank[MODE_DISTANCE])%key4.max_rank;					
				printf("\n��ǰ�������Ϊ�ȼ���%d", key4.key_rank[MODE_DISTANCE]);
			}
		}
//		else
//		{
//			printf("KEY5 error!");
//		}
		
		return KEY_SUB;
	}		

	else
		return KEY_NO;
}

#endif

/**
  * @brief  ��������ģ��
  * @param  ��  
  * @retval ��
  */
//void key_module()
//{
//	int key_n;
//	key_n=KeyPolling();	//������ѯ
//	
//	switch(key_n)
//	{
///*******************************************************************************************************/
//		case KEY_MODE:	//ģʽѡ��
//			
//			if(MODE_VOLUME == key4.current_mode)	//��ǰ������������ģʽ
//			{
//				//���ţ���ǰ������������ģʽ
//				break;
//			}
//			else if(MODE_FREQUENCY == key4.current_mode)	//��ǰ����Ƶ�ʵ���ģʽ
//			{
//				//���ţ���ǰ����Ƶ�ʵ���ģʽ
//				break;
//			}
//			else if(MODE_DISTANCE == key4.current_mode)	//��ǰ���ھ������ģʽ
//			{
//				//���ţ���ǰ���ھ������ģʽ
//				break;
//			}
//			else
//				break;
///*******************************************************************************************************/
//			
//		case KEY_BACK:	//������һ��ģʽ/��һ��ģʽ
//			
//			key4.current_mode=(++key4.current_mode)%key4.max_mode;
//			break;
///*******************************************************************************************************/
//		
//		case KEY_ADD:	//�Ӻ�

//			if(MODE_VOLUME == key4.current_mode)	//��ǰ������������ģʽ
//			{
//				if(key4.max_rank == key4.key_rank[MODE_VOLUME])
//				{
//				
//				}
//				else if(key4.min_rank == key4.key_rank[MODE_VOLUME])
//				{
//				
//				}
//				else
//				{
//					//�������Ӻ�������
//					key4.key_rank[MODE_VOLUME]=(++key4.key_rank[MODE_VOLUME])%key4.max_rank;					
//				}

//			}
//			else if(MODE_FREQUENCY == key4.current_mode)	//��ǰ����Ƶ�ʵ���ģʽ
//			{
//				if(key4.max_rank == key4.key_rank[MODE_FREQUENCY])
//				{
//				
//				}
//				else if(key4.min_rank == key4.key_rank[MODE_FREQUENCY])
//				{
//				
//				}
//				else
//				{
//					//Ƶ�����Ӻ�������
//					key4.key_rank[MODE_FREQUENCY]=(++key4.key_rank[MODE_FREQUENCY])%key4.max_rank;					
//				}

//			}
//			else if(MODE_DISTANCE == key4.current_mode)	//��ǰ���ھ������ģʽ
//			{
//				if(key4.max_rank == key4.key_rank[MODE_DISTANCE])
//				{
//				
//				}
//				else if(key4.min_rank == key4.key_rank[MODE_DISTANCE])
//				{
//				
//				}
//				else
//				{
//					//�������Ӻ�������
//					key4.key_rank[MODE_DISTANCE]=(++key4.key_rank[MODE_DISTANCE])%key4.max_rank;					
//				}
//			}
//			else
//				break;
///*******************************************************************************************************/
//			
//		case KEY_SUB:	//����
//			
//			if(MODE_VOLUME == key4.current_mode)	//��ǰ������������ģʽ
//			{
//				if(key4.max_rank == key4.key_rank[MODE_VOLUME])
//				{
//				
//				}
//				else if(key4.min_rank == key4.key_rank[MODE_VOLUME])
//				{
//				
//				}
//				else
//				{
//					//�������ٺ�������
//					key4.key_rank[MODE_VOLUME]=(--key4.key_rank[MODE_VOLUME])%key4.max_rank;					
//				}				
//			}
//			else if(MODE_FREQUENCY == key4.current_mode)	//��ǰ����Ƶ�ʵ���ģʽ
//			{
//				if(key4.max_rank == key4.key_rank[MODE_FREQUENCY])
//				{
//				
//				}
//				else if(key4.min_rank == key4.key_rank[MODE_FREQUENCY])
//				{
//				
//				}
//				else
//				{
//					//Ƶ�ʼ��ٺ�������
//					key4.key_rank[MODE_FREQUENCY]=(--key4.key_rank[MODE_FREQUENCY])%key4.max_rank;					
//				}
//			}
//			else if(MODE_DISTANCE == key4.current_mode)	//��ǰ���ھ������ģʽ
//			{
//				if(key4.max_rank == key4.key_rank[MODE_DISTANCE])
//				{
//				
//				}
//				else if(key4.min_rank == key4.key_rank[MODE_DISTANCE])
//				{
//				
//				}
//				else
//				{
//					//������ٺ�������
//					key4.key_rank[MODE_DISTANCE]=(--key4.key_rank[MODE_DISTANCE])%key4.max_rank;					
//				}
//			}
//			else
//				break;
///*******************************************************************************************************/

//		case KEY_SAFE:	//��ȫ
//			break;
///*******************************************************************************************************/
//		
//		default:
//			break;
//	}
//	
//}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KEY1 1
#define KEY2 2
#define KEY3 3
#define KEY4 4
#define KEY5 5


static void EXTI_n(int key_n)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	if(KEY1==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
		/* �ر��ж� */	
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);
	}
	else if(KEY2==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
		/* �ر��ж� */	
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else if(KEY3==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY3_INT_EXTI_LINE;
		/* �ر��ж� */	
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else if(KEY4==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY4_INT_EXTI_LINE;
		/* �ر��ж� */	
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else if(KEY5==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY5_INT_EXTI_LINE;
		/* �ر��ж� */	
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else
		;
		
}

static void EXTI_n_Open(int key_n)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	if(KEY1==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
		/* �ر��ж� */	
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
	}
	else if(KEY2==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
		/* �ر��ж� */	
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else if(KEY3==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY3_INT_EXTI_LINE;
		/* �ر��ж� */	
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else if(KEY4==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY4_INT_EXTI_LINE;
		/* �ر��ж� */	
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else if(KEY5==key_n)
	{
		EXTI_InitStructure.EXTI_Line = KEY5_INT_EXTI_LINE;
		/* �ر��ж� */	
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);	
	}
	else
		;
		
}

#define iCOUNT 1
#define jCOUNT 100
static void delay(int i, int j)
{
	int yj=j;
	while(i--)
	{
		j=yj;
		while(j--);
	}
}

#endif