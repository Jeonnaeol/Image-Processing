#include "stdafx.h"
#include <vector>
//#include "include_tracking.h"
#include "time.h"


#define FILTERSIZE 10

class dominic{
public:	
	//변수선언-------kalman--------------------------------------
	int start, start2, start3, start4, start5;	//시작했는지 안했는지, 1이면 시작함
	float timeslept;	//지난시간,
	float ctime;	//현재 입력받은시간
	float ptime;	 //전에 입력받았던시간
	int cx,cy;	//current x, y 현재 입력받은 x,y
	int px,py; //previous x,y 전에 입력받았던 x,y
	float xv,yv;	//x,y 속도
	//변수선언-------avr-----------------------------------------
	int avr_k; //더해진 좌표의 갯수
	int out_x, out_y; //돌려줄 x, y
	int past_x, past_y;
	//변수선언-------mov_avr-------------------------------------
	int data_x[FILTERSIZE];
	int data_y[FILTERSIZE];
	int sum_x, sum_y;
	int j;
	int mov_avr_x, mov_avr_y;
	//변수선언-------low_pass------------------------------------
	float low_pass_x, low_pass_y;
	float low_pass_px, low_pass_py;
	float low_pass_w;
	//변수선언-------kalman--------------------------------------
	float kalman_x, kalman_y;
	float kalman_Px, kalman_Py;
	float kalman_pre_Px, kalman_pre_Py;
	float kalman_pre_p, kalman_pre_Pp;
	float kalman_gain;
	float kalman_p, kalman_Pp;
	float A, A_t, H, H_t, Q, R;
	//함수선언---------------------------------------------------
	dominic();//생성자

	void dominic::kf(int xin, int yin);
	void dominic::avr(int in_x, int in_y);
	void dominic::mov_avr(int in_x, int in_y);
	void dominic::low_pass(int in_x, int in_y);
	void dominic::kalman(int in_x, int in_y);
	void dominic::kalman1d(float in_x);
};