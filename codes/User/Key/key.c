//�����߼�ʵ��
#include "key.h"
#include "./key/bsp_key.h"  
#include "./usart/bsp_usart1.h"
#include "mp3.h"
#include "UltrasonicWave.h"

extern int flag_volume;  
extern int flag_FALLING;
extern  int MODE_FLAG;       //1 ���� 0 Ƶ��
static struct key KeyConfig;
extern int flag_volume;
 /*
 * ��������Key_Scan
 * ����  ������Ƿ��а�������
 * ����  ��GPIOx��x ������ A��B��C��D���� E
 *		     GPIO_Pin������ȡ�Ķ˿�λ 	
 * ���  ��0(û���°���)��1�����°�����
 */
static uint8_t Key_Scan_down(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin)
{			
	/* ����Ƿ��а������� */
	if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_DOWN )  
	{	 
			delay_key(10);	
			if(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_DOWN )  
			{
					while(GPIO_ReadInputDataBit(GPIOx,GPIO_Pin) == KEY_DOWN);	//�ȴ������ſ�
				//			printf("�ȴ������ͷ�\r\n"); 
					
					return 1;	 
			}
			else
			{
					return 0;
			}
	}
	else
	{
			return 0;
	}
}



/*
* @brief ��������,ѡ��ָ���������е���
* @param  no
* @return no
*/
static void setFunction()
{
	flag_volume=0;
	printf("\n���¹������ü�!\r\n");
	 KeyConfig.set_parameter = (KeyConfig.set_parameter+1) % SET_MAX;    //ѡ����һ�����ò���
	
	switch(KeyConfig.set_parameter)
	{
		case SET_CLOSE:	    		
						printf("\t�˳�����\r\n");
						USART3_Send_String(ModeExit,sizeof(ModeExit)); 
						break;
		case SET_VOLUME:	
						printf("\t��ǰ��������������,��ǰ�����ȼ�Ϊ��%d\r\n", KeyConfig.set_volume);
						USART3_Send_String(AdjustVolume,sizeof(AdjustVolume));  //���ţ���ǰ������������ģʽ			
						break;   
		case SET_FREQUENCY:
						printf("\t��ǰ����Ƶ�ʵ����У���ǰƵ�ʵȼ�Ϊ��%d\r\n", KeyConfig.set_frequency);			
						USART3_Send_String(AdjustRate,sizeof(AdjustRate));     //���ţ���ǰ����Ƶ�ʵ���ģʽ
						break;    
		case SET_DISTANCE: 	
						printf("\t��ǰ���ھ�������У���ǰ����ȼ�Ϊ��%d\r\n", KeyConfig.set_distance);
						USART3_Send_String(ModeDistance,sizeof(ModeDistance));  //���ţ���ǰ���ھ������ģʽ
						break;   
		case SET_SHAKE:
						printf("\t��ǰ�����𶯵�����,��ǰ�𶯵ȼ�Ϊ��%d\r\n", KeyConfig.set_shake);
			//			USART3_Send_String(AdjustVolume,sizeof(AdjustVolume));  //���ţ���ǰ�����𶯵���ģʽ			
						break; 
		default :printf("\nsetparamter error! KeyConfig.current_mode = %d", KeyConfig.set_parameter);
	}			
}


/*
* @brief �л���ʾģʽ
* @param  no
* @return no
*/
void switchMode()
{
	flag_volume=0;
	printf("\n���½�����һ��ģʽ��!\r\n");
	KeyConfig.current_mode= (++KeyConfig.current_mode) % MODE_MAX;
	if(MODE_VOLUME == KeyConfig.current_mode)
	{
		MODE_FLAG=1;
		printf("\t��ǰģʽ����Ϊ��%s", "����ģʽ\r\n");
		USART3_Send_String(ModeVolume,sizeof(ModeVolume));
		//����
		AdjustVibrationFrequencyGlasses(0);   //��������Ϊ0
	}
	else if(MODE_FREQUENCY == KeyConfig.current_mode)
	{
		MODE_FLAG=0;
		printf("\t��ǰģʽ����Ϊ��%s", "Ƶ��ģʽ\r\n");
		USART3_Send_String(ModeRate,sizeof(ModeRate));
		//����
	}
	else if(MODE_SHAKE == KeyConfig.current_mode)
	{
		printf("\t��ǰģʽ����Ϊ��%s", "��ģʽ\r\n");
		MODE_FLAG = 2;
	}	
}	

/*
* @brief ��������
* @param  no
* @return no
*/
static void volumeUp()
{
	if(MAX_RANK_VOLUME <= KeyConfig.set_volume)
	{
		printf("\t��ǰ��������Ϊ�ȼ���%d���ﵽ���ȼ������ٵ���\r\n", KeyConfig.set_volume);				
		AddVolume();         //�������Ӻ�������
	}
	else
	{
		AddVolume();         //�������Ӻ�������
		++KeyConfig.set_volume;					
		printf("\t��ǰ��������Ϊ�ȼ���%d\r\n", KeyConfig.set_volume);
	}	
}	

/*
* @brief ����Ƶ��
* @param  no
* @return no
*/
static void rateUp()
{
	if(MAX_RANK_FREQUENCY == KeyConfig.set_frequency)
	{
		printf("\t��ǰƵ�ʵ���Ϊ�ȼ���%d���ﵽ���ȼ������ٵ���\r\n", KeyConfig.set_frequency);			
	}
	else
	{
		AddRate();//Ƶ�����Ӻ�������
		++KeyConfig.set_frequency;					
		printf("\t��ǰƵ�ʵ���Ϊ�ȼ���%d\r\n", KeyConfig.set_frequency);
	}		
}

/*
* @brief ���������
* @param  no
* @return no
*/
void distanceUp()
{
	if(MAX_RANK_DISTANCE == KeyConfig.set_distance)
	{			
		printf("\t��ǰ�������Ϊ�ȼ���%d���ﵽ���ȼ������ٵ���\r\n", KeyConfig.set_distance);
	}
	else
	{
		addDistance();             //�������Ӻ�������
		++KeyConfig.set_distance;					
		printf("\t��ǰ�������Ϊ�ȼ���%d\r\n", KeyConfig.set_distance);
	}	
}
/*
* @brief �����𶯵ȼ�
* @param  no
* @return no
*/
void shakeUp()
{
	if(MAX_RANK_SHAKE == KeyConfig.set_shake)
	{			
		printf("\t��ǰ�𶯵���Ϊ�ȼ���%d���ﵽ���ȼ������ٵ���\r\n", KeyConfig.set_shake);
	}
	else
	{
		AddShake();             //�������Ӻ�������
		++KeyConfig.set_shake;					
		printf("\t��ǰ�𶯵���Ϊ�ȼ���%d\r\n", KeyConfig.set_shake);
	}	
}

/*
* @brief ���������ȼ�
* @param  no
* @return no
*/
static void volumeDown()
{
	if(MIN_RANK_VOLUME == KeyConfig.set_volume)
	{
		printf("\t��ǰ��������Ϊ�ȼ���%d���ﵽ��С�ȼ������ٵ�С\r\n", KeyConfig.set_volume);
		SubVolume();//�������ٺ�������
	}
	else
	{
		SubVolume();//�������ٺ�������
		--KeyConfig.set_volume;					
		printf("\t��ǰ��������Ϊ�ȼ���%d\r\n", KeyConfig.set_volume);
	}				
}
	
/*
* @brief ����Ƶ�ʵȼ�
* @param  no
* @return no
*/
static void rateDown()
{
	if(MIN_RANK_FREQUENCY == KeyConfig.set_frequency)
	{
		printf("\t��ǰƵ�ʵ���Ϊ�ȼ���%d���ﵽ��С�ȼ������ٵ�С\r\n", KeyConfig.set_frequency);
	}
	else
	{
		SubRate();//Ƶ�ʼ��ٺ�������
		--KeyConfig.set_frequency;					
		printf("\t��ǰƵ�ʵ���Ϊ�ȼ���%d\r\n", KeyConfig.set_frequency);
	}	
}	

/*
* @brief ��С������ȼ�
* @param  no
* @return no
*/
static void distanceDown() 
{
	if( MIN_RANK_DISTANCE == KeyConfig.set_distance)
	{
		printf("\t��ǰ�������Ϊ�ȼ���%d���ﵽ��С�ȼ������ٵ�С\r\n", KeyConfig.set_distance);			
	}
	else
	{
		minusDistance();       //������ٺ�������
		--KeyConfig.set_distance;					
		printf("\t��ǰ�������Ϊ�ȼ���%d\r\n", KeyConfig.set_distance);
	}
}

/*
* @brief �����𶯵ȼ�
* @param  no
* @return no
*/
static void shakeDown()
{
	if(MIN_RANK_SHAKE == KeyConfig.set_shake)
	{
		printf("\t��ǰƵ�ʵ���Ϊ�ȼ���%d���ﵽ��С�ȼ������ٵ�С\r\n", KeyConfig.set_shake);
	}
	else
	{
		SubShake();//�𶯼��ٺ�������
		--KeyConfig.set_shake;					
		printf("\t��ǰƵ�ʵ���Ϊ�ȼ���%d\r\n", KeyConfig.set_shake);
	}	
}	

/*
* @brief ��ⰴ������
* @param  no
* @return no
*/
void KeyPolling(void)
{	

	if( Key_Scan_down(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == 1  )
	{
	   setFunction();    //��������      
	}	
	
	else if( Key_Scan_down(KEY3_GPIO_PORT,KEY3_GPIO_PIN) == 1  )
	{
       switchMode();    //ģʽ�л�
	}		

	else if( Key_Scan_down(KEY4_GPIO_PORT,KEY4_GPIO_PIN) == 1 )
	{
	    flag_volume=0;
		printf("\n���¼Ӻż�!\r\n");
		switch(KeyConfig.set_parameter)
		{
			case SET_CLOSE:	    printf("��ѡ������ģʽ\r\n"); break;
			case SET_VOLUME:	volumeUp();   				  break;    //��������
			case SET_FREQUENCY: rateUp();  			     	  break;    //����Ƶ��
			case SET_DISTANCE: 	distanceUp();				  break;    //���������
			case SET_SHAKE: 	shakeUp();				      break;    //�����𶯵ȼ�
			default :printf("KEY5 error! KeyConfig.set_parameter = %d\r\n", KeyConfig.set_parameter);
		}			
	}		
	else if( Key_Scan_down(KEY5_GPIO_PORT,KEY5_GPIO_PIN) == 1 )
	{
		flag_volume=0;
			printf("\n���¼��ż�!\r\n");
		switch(KeyConfig.set_parameter)
		{
			case SET_CLOSE:	    printf("��ѡ������ģʽ\r\n"); break;
			case SET_VOLUME:	volumeDown();  				  break;    //�µ�����
			case SET_FREQUENCY: rateDown();  				  break;    //�µ�Ƶ��
			case SET_DISTANCE: 	distanceDown();				  break;    //��С������
			case SET_SHAKE: 	shakeDown();				  break;    //��С�𶯵ȼ�
			default :printf("KEY5 error! KeyConfig.set_parameter = %d\r\n", KeyConfig.set_parameter);
		}			
    }		

}




















