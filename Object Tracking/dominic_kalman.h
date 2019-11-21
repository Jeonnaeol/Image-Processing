#include "stdafx.h"
#include <vector>
//#include "include_tracking.h"
#include "time.h"


#define FILTERSIZE 10

class dominic{
public:	
	//��������-------kalman--------------------------------------
	int start, start2, start3, start4, start5;	//�����ߴ��� ���ߴ���, 1�̸� ������
	float timeslept;	//�����ð�,
	float ctime;	//���� �Է¹����ð�
	float ptime;	 //���� �Է¹޾Ҵ��ð�
	int cx,cy;	//current x, y ���� �Է¹��� x,y
	int px,py; //previous x,y ���� �Է¹޾Ҵ� x,y
	float xv,yv;	//x,y �ӵ�
	//��������-------avr-----------------------------------------
	int avr_k; //������ ��ǥ�� ����
	int out_x, out_y; //������ x, y
	int past_x, past_y;
	//��������-------mov_avr-------------------------------------
	int data_x[FILTERSIZE];
	int data_y[FILTERSIZE];
	int sum_x, sum_y;
	int j;
	int mov_avr_x, mov_avr_y;
	//��������-------low_pass------------------------------------
	float low_pass_x, low_pass_y;
	float low_pass_px, low_pass_py;
	float low_pass_w;
	//��������-------kalman--------------------------------------
	float kalman_x, kalman_y;
	float kalman_Px, kalman_Py;
	float kalman_pre_Px, kalman_pre_Py;
	float kalman_pre_p, kalman_pre_Pp;
	float kalman_gain;
	float kalman_p, kalman_Pp;
	float A, A_t, H, H_t, Q, R;
	//�Լ�����---------------------------------------------------
	dominic();//������

	void dominic::kf(int xin, int yin);
	void dominic::avr(int in_x, int in_y);
	void dominic::mov_avr(int in_x, int in_y);
	void dominic::low_pass(int in_x, int in_y);
	void dominic::kalman(int in_x, int in_y);
	void dominic::kalman1d(float in_x);
};