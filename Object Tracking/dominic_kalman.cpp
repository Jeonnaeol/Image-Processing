#include "stdafx.h"
#include "dominic_kalman.h"
#include "ImageTool.h"
#include "ImageToolDlg.h"
#include <stdio.h>
#include <fstream>

int debug_km = 0;

dominic::dominic(){
	start = 0;
	start2 = 0;
	low_pass_w = 0.5; //저주파 필터 가중치입니다.
	A = 1;
	H = 1;
	Q = 0.1;
	R = 0.1;
	A_t = 1;
	H_t = 1;
}

void dominic::kf(int xin, int yin){		//x, y값 입력받음
	ctime=clock();	//현재시간 입력
	//ptime;
	if(start==0){	//처음 함수가 동작할때 px,py,ptime값들을 현재 입력받는 값들로 갱신해줌
		px=xin;
		py=yin;
		ptime=ctime;
		start=1;
	}

	cx=xin;	//현재값갱신
	cy=yin;	//현재값갱신


	yv=(cy-py);	//속도 계산
	xv=(cx-px);
	if(debug_km)printf("현재x=%d, y=%d, 전x=%d,y=%d, 지난시간=%3f, 속도x=%3f,y=%3f\n", cx,cy, px,py, timeslept, yv,xv);

	
	timeslept=abs(ctime-ptime)/10;	//지난시간 = 현재 받은 시간 - 지난번에 받은 시간
	ptime=ctime;	//현재 받은 시간을 전에 받은 시간으로 갱신
	
	px=cx;	//현재 x,y를 지난 x,y로 갱신
	py=cy;

	//돌려주는 값은 속도.
}

void dominic::avr(int in_x, int in_y){
	if(start2 == 0){
		start2 = 1;
	}
	if(start2 == 1){	//처음 함수가 동작할때 px,py,ptime값들을 현재 입력받는 값들로 갱신해줌
		avr_k = avr_k + 1;
		start2 = 2;
	}
	past_x = past_x + in_x;
	past_y = past_y + in_y;

	out_x = past_x / avr_k;
	out_y = past_y / avr_k;

	start2 = 0;

	if(debug_km)printf("원소 개수  = %d, 이전 x = %d, y = %d, 현재 x=%d, y=%d, 평균값 x=%d, y=%d\n",avr_k, past_x, past_y, in_x, in_y, out_x, out_y);

	//돌려주는 값은 out_x, out_y;

}

void dominic::mov_avr(int in_x, int in_y)
{
	if(start3 == 0){
		j=0;
		for(int i=0;i<FILTERSIZE;i++){
			data_x[i] = 0;
			data_y[i] = 0;
			if(debug_km)printf("x : %d, y : %d / ", data_x[i], data_y[i]);
		}
		if(debug_km)printf("\n");
		start3 = 1;
	}//Array Initialize
	if(start3 == 1){
		data_x[j] = in_x;
		data_y[j] = in_y;
		if(debug_km)printf("%d in data_x[%d], %d in data_y[%d]\n", data_x[j], j, data_y[j], j);
		j++;
		if(j== FILTERSIZE)
			start3 = 2;
	}//초기값을 FILTERSIZE만큼 받아둠
	if(start3 ==2){
		for(int i=0;i<FILTERSIZE;i++){
			sum_x += data_x[i];
			sum_y += data_y[i];
			printf("sum_x = %d, sum_y = %d %d\n", sum_x, sum_y, i);//10번째까지의 합
			mov_avr_x = sum_x / FILTERSIZE;
			mov_avr_y = sum_y / FILTERSIZE;
			if(debug_km)printf("평균 : %d, %d\n", mov_avr_x, mov_avr_y);
		}
		j=0;
		start3 = 3;
	}
	//Init
	if(start3 == 3){
		int temp = data_x[j];
		int temp2 = data_y[j];
		data_x[j]= in_x;
		data_y[j]= in_y;
		mov_avr_x = sum_x/FILTERSIZE + ((data_x[j] - temp) / FILTERSIZE);
		mov_avr_y = sum_y/FILTERSIZE + ((data_y[j] - temp2) / FILTERSIZE);
		sum_x = sum_x - temp + data_x[j];
		sum_y = sum_y - temp2 + data_y[j];
		j = (j+1) % FILTERSIZE;
		if(debug_km)printf("in_x : %d, 평균 : %d, 바뀐 sum : %d \n", in_x, mov_avr_x, sum_x);
		if(debug_km)printf("in_y : %d, 평균 : %d, 바뀐 sum : %d \n", in_y, mov_avr_y, sum_y);
		//수식이 동작하게끔
	}
}	

void dominic::low_pass(int in_x, int in_y){
	if(start4 == 0){
		low_pass_px = 0;
		low_pass_py = 0;
		start4 = 1;
	}

	low_pass_x =  low_pass_w * low_pass_px + (1-low_pass_w) * in_x;
	low_pass_y =  low_pass_w * low_pass_py + (1-low_pass_w) * in_y;
	low_pass_px = low_pass_x;
	low_pass_py = low_pass_y;
	
}

void dominic::kalman(int in_x, int in_y){
	if(start5 == 0){
		kalman_pre_Px = 1;
		kalman_pre_Py = 1;
		kalman_pre_Pp = 1;
		start5 = 1;
	}
	kalman_Px = A*kalman_pre_Px;//추정 예측값 :: 변환행렬 * 이전의 추정예측값
	kalman_Py = A*kalman_pre_Py;
	kalman_Pp = (A * kalman_pre_Pp * A_t) + Q;//공분산 예측값 :: (변환행렬 * 이전의 공분산 예측값 * 변환행렬(T)) + Q

	kalman_gain = kalman_Pp * H_t * (1/(H*kalman_Pp*H_t + R));//칼만 이득 :공분산 예측값 * H * inv(H*공분산 예측값*H(T) + R)
	kalman_x = kalman_Px + (kalman_gain * (in_x - (H * kalman_Px))); 
	kalman_y = kalman_Py + (kalman_gain * (in_y - (H * kalman_Py))); 
	kalman_p = kalman_Pp - (kalman_gain * H * kalman_Pp);
	if(debug_km)printf("Px : %f, Pp : %f, gain : %f, x : %f, p : %f\n", kalman_Px, kalman_Pp, kalman_gain, kalman_x, kalman_p);
	if(debug_km)printf("Py : %f, Pp : %f, gain : %f, y : %f, p : %f\n", kalman_Py, kalman_Pp, kalman_gain, kalman_y, kalman_p);

	kalman_pre_Px = kalman_x;
	kalman_pre_Py = kalman_y;
	kalman_pre_Pp = kalman_p;
}

void dominic::kalman1d(float in_x){
	if(start5 == 0){
		kalman_pre_Px = 1;
		kalman_pre_Pp = 1;
		start5 = 1;
	}
	kalman_Px = A*kalman_pre_Px;//추정 예측값 :: 변환행렬 * 이전의 추정예측값
	kalman_Pp = (A * kalman_pre_Pp * A_t) + Q;//공분산 예측값 :: (변환행렬 * 이전의 공분산 예측값 * 변환행렬(T)) + Q

	kalman_gain = kalman_Pp * H_t * (1/(H*kalman_Pp*H_t + R));//칼만 이득 :공분산 예측값 * H * inv(H*공분산 예측값*H(T) + R)
	kalman_x = kalman_Px + (kalman_gain * (in_x - (H * kalman_Px))); 
	kalman_p = kalman_Pp - (kalman_gain * H * kalman_Pp);
	kalman_pre_Px = kalman_x;
	kalman_pre_Pp = kalman_p;
}