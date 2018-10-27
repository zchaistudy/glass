#ifndef __KEY_H
#define	__KEY_H


struct key{
	int current_mode;   //��ǰ��ʾģʽ
    int set_parameter;  //��ǰ���ڲ���
	int set_volume;     //��ǰ�����ȼ�
	int set_frequency;  //��ǰƵ�ʵȼ�
	int set_shake;      //��ǰ�𶯵ȼ�
	int set_distance;   //��ǰ����ȼ�
};

//��ʾģʽ
#define MODE_VOLUME     0	//����ģʽ
#define MODE_FREQUENCY 	1   //Ƶ��ģʽ
#define MODE_SHAKE      2   //��ģʽ
#define MODE_MAX        3   //ģʽ����

//��������
#define SET_CLOSE       0//�رղ�������
#define SET_VOLUME      1//������������
#define SET_FREQUENCY 	2 //Ƶ�ʲ�������
#define SET_DISTANCE    3 //�����������
#define SET_SHAKE       4 //�𶯲�������
#define SET_MAX         5  //5�ֲ�������

//�������ֵ
#define MAX_RANK_VOLUME       15     //��������ȼ�
#define MAX_RANK_FREQUENCY    3      //���Ƶ�ʵȼ�
#define MAX_RANK_DISTANCE     6      //������ȼ�
#define MAX_RANK_SHAKE        3      //����𶯵ȼ�

//������Сֵ
#define MIN_RANK_VOLUME       1     //��С�����ȼ�
#define MIN_RANK_FREQUENCY    1      //��СƵ�ʵȼ�
#define MIN_RANK_DISTANCE     1      //��С����ȼ�
#define MIN_RANK_SHAKE        1      //��С�𶯵ȼ�

void Key_Config(void);
void KeyPolling(void);



#endif

