#include "stdafx.h"
#include "ImageTool.h"
#include "ImageToolDlg.h"
#include "include_tracking.h"

#include "dominic_kalman.h"
#include "include_color.h"
#include <iostream>

#define SHAPE_DETECT 4	//낮으면 사각형 인식 더 잘되게, 높을수록 까다롭
#define RATIO 1	// 낮을수록 더 추적 잘되게
#define KM	//칼만필터사용
#define MORPHOLOGY 3	//높을수록 침식 팽창 심하게
int debug=0;
int debug_1=0;

////////////////////////////////////////////////////////////////
BYTE histogram_image[240][320] = {0};
int histogram_WIDTH[320] = {0};
int histogram_HEIGHT[240] = {0};
int max_HEIGHT = 0;
int max_WIDTH = 0;
int max_x = 0;
int max_y = 0;
BYTE pre_image[240][320] = {};
BYTE pre_image1[240][320] = {};
BYTE pre_image2[240][320] = {};
///////////////////////////////////
int redON;
int greenON;
int blueON;
int yellowON;

int redstrong;
int greenstrong;
int bluestrong;
int yellowstrong;
///////////////////////////////////////

int search_x, search_y;

#define MAX_CONTOUR 10000

typedef struct _ContourPoints 
{
	int num;
	int x[MAX_CONTOUR];
	int y[MAX_CONTOUR];
} ContourPoints;

double H[240][320] = {};
double S[240][320] = {};
double I[240][320] = {};

using namespace std;


extern int p1_x, p1_y, p2_x, p2_y, p3_x, p3_y, p4_x, p4_y;
extern int p1_ax, p1_ay, p2_ax, p2_ay;

extern int flag2,flag3,flag4,flag5;

extern struct track track1;
extern struct track track2;
extern struct track track3;
extern struct track track4;


extern int x1_1, x1_2, y1_1, y1_2;
extern int x2_1, x2_2, y2_1, y2_2;
extern int x3_1, x3_2, y3_1, y3_2;
extern int x4_1, x4_2, y4_1, y4_2;


//도형판단
BYTE selection[240][320]={0,};
BYTE ch_image[240][320]= {0};
BYTE buffer[240][320]={0,};
int ch_count[100000]={0,};
int ch_num[10]={0,};
int ch_t=0;
int ch_sub=0;
int ch_division=0;
int ch_proportion1=0;
int ch_proportion2=0;


int shape_flag=0;
extern int result1,result2,result3,result4; 
int count1=0, count2=0, count3=0, count4=0;

extern int track1l;
extern int track2l;
extern int track3l;
extern int track4l;


int include_Detect_this_Color(BYTE** R, BYTE** G, BYTE** B,int color, BYTE** output)
{
	int result=0; 
	BYTE** temp1 = new BYTE*[HEIGHT];
	BYTE** temp2 = new BYTE*[HEIGHT];	
	BYTE** temp3 = new BYTE*[HEIGHT];
	BYTE** temp4 = new BYTE*[HEIGHT];
	BYTE** temp5 = new BYTE*[HEIGHT];

	for(int y=0; y<HEIGHT; y++)
	{

		temp1[y] = new BYTE[WIDTH];
		temp2[y] = new BYTE[WIDTH];
		temp3[y] = new BYTE[WIDTH];
		temp4[y] = new BYTE[WIDTH];
		temp5[y] = new BYTE[WIDTH];

		memset(temp1[y],0,sizeof(BYTE)*WIDTH);
		memset(temp2[y],0,sizeof(BYTE)*WIDTH);
		memset(temp3[y],0,sizeof(BYTE)*WIDTH);
		memset(temp4[y],0,sizeof(BYTE)*WIDTH);
		memset(temp5[y],0,sizeof(BYTE)*WIDTH);
	}


#if 1	
	clear_img(temp1);
	clear_img(temp5);
	if(color==1)
		include_detect_color(R,G,B,temp5,temp1,1);
	else if(color==2)
		include_detect_color(R,G,B,temp5,temp1,2);
	else if(color==3)
		include_detect_color(R,G,B,temp5,temp1,3);
	else if(color==4)
		include_detect_color(R,G,B,temp5,temp1,4);
#endif
	include_erode(temp1, temp2);
	include_dilate(temp2, temp3);	
	include_Labeling(temp3, temp4);
	include_Detect_Contour(temp4, temp5);
	

	int tmpr1=0, tmpr2=0, tmpr3=0, tmpr4=0;	

	if(color==1 && result1==0 && redstrong==0){
		tmpr1 = include_shape_detect(temp3, output);
		result = tmpr1;
	}

	if(color==1 && redstrong==1)
		result=1;


	if(color==2 && result2==0  && bluestrong==0){
		tmpr2 = include_shape_detect(temp3, output);
		result = tmpr2;
	}

	if(color==2 && bluestrong==1)
		result=1;

	if(color==3 && result3==0&& yellowstrong==0){
		tmpr3 = include_shape_detect(temp3, output);
		result = tmpr3;
	}
	if(color==3 && yellowstrong==1)
		result=1;
	if(color==4 && result4==0&& greenstrong==0){
		tmpr4 = include_shape_detect(temp3, output);
		result = tmpr4;
	}
	if(color==4 && greenstrong==1)
		result=1;

	if(tmpr1 && color==1){
		x1_1 = p4_x;
		x1_2 = p3_x;
		y1_1 = p3_y;
		y1_2 = p4_y;
		track1.x1 = p4_x;
		track1.x2 = p3_x;
		track1.y1 = p4_y;
		track1.y2 = p3_y;
		count1++;		//도형 체크를 위한 카우늩
	}

	if(tmpr2 && color==2){
		x2_1 = p4_x;
		x2_2 = p3_x;
		y2_1 = p3_y;
		y2_2 = p4_y;
		track2.x1 = p4_x;
		track2.x2 = p3_x;
		track2.y1 = p4_y;
		track2.y2 = p3_y;
		count2++;		//도형 체크를 위한 카우늩	
	}
	if(tmpr3 && color==3){
		x3_1 = p4_x;
		x3_2 = p3_x;
		y3_1 = p3_y;
		y3_2 = p4_y;
		track3.x1 = p4_x;
		track3.x2 = p3_x;
		track3.y1 = p4_y;
		track3.y2 = p3_y;
		count3++;		//도형 체크를 위한 카우늩

	}
	if(tmpr4 && color==4){
		x4_1 = p4_x;
		x4_2 = p3_x;
		y4_1 = p3_y;
		y4_2 = p4_y;
		track4.x1 = p4_x;
		track4.x2 = p3_x;
		track4.y1 = p4_y;
		track4.y2 = p3_y;
		count4++;		//도형 체크를 위한 카우늩
	}


	for(int y=0; y<HEIGHT; y++)
	{
		delete [] temp1[y];
		delete [] temp2[y];
		delete [] temp3[y];
		delete [] temp4[y];
		delete [] temp5[y];
		//delete [] temp6[y];
	}

	delete [] temp1;
	delete [] temp2;
	delete [] temp3;
	delete [] temp4;
	delete [] temp5;
	//delete [] temp6;

	return result;
}
//도형 판단
int include_shape_detect(BYTE** input, BYTE** output)
{   
    int gogo=0;
	int search_x=0;
	int search_y=0;

	int temp1=0;
	int temp2=0;
	int i, j;


	for(i=0; i<HEIGHT; i++) {
		for(j=0; j<WIDTH; j++) {
			ch_image[i][j] = input[i][j];
		}
	}
	for(int i=0; i<HEIGHT; i++) {
		for(int j=0; j<WIDTH; j++) {
			if(input[i][j] != 0) {
				histogram_HEIGHT[i] += 1;
				histogram_WIDTH[j] += 1;
			}
		}
	}

	for(int i = 0; i<HEIGHT; i++) {
		if(max_HEIGHT < histogram_HEIGHT[i]) {
			max_HEIGHT = histogram_HEIGHT[i];
			max_y = i; // y 좌표	
		}
	}
	for(int i= 0; i<WIDTH; i++) {
		if(max_WIDTH < histogram_WIDTH[i]) {
			max_WIDTH = histogram_WIDTH[i];
			max_x = i; // x 좌표
		}
	}

	for(int i=0;i<240;i++){
		for(int j=0; j<320; j++){
			buffer[i][j]=0;
		}
	}//buffer초기화

	for(int i=0; i<HEIGHT-max_y ;i++) 
	{
		if(ch_image[max_y+i][max_x]==0)
		{
			search_x=max_x;
			search_y=max_y+i-1;
			break;
		}
	}/////최고점에서 가장 윗부분으로 올린다.//외각선 첫 위치 좌표 


	int hs = 1;

	//외곽선 검출한 것을 임시버퍼에 저장 그리고 외곽선 체킹.
	for(;;){
		if(temp1==search_x && temp2==search_y ) // 첫번째 조사한 위치와 다 돌고 난뒤 마지막으로 조사한 위치가 같을경우 그만둔다.
			break;

		temp1=search_x;
		temp2=search_y; 

		if(ch_image[search_y+1][search_x]!=0             //내가 조사할 방향의 픽셀 값이 255인 경우 
			&& (ch_image[search_y+1][search_x+1]==0 ||	// 
			ch_image[search_y+1][search_x-1]==0 || ch_image[search_y+2][search_x]==0 || ch_image[search_y][search_x]==0) && buffer[search_y+1][search_x]==0)
		{
			ch_count[ch_t]=1;
			ch_t++;                                                   
			buffer[search_y][search_x]=ch_image[search_y][search_x];

			selection[search_y][search_x]=1;
			search_x=search_x;                                         
			search_y=search_y+1;							
		}
		//continue;

		else if(ch_image[search_y+1][search_x+1]!=0
			&& (ch_image[search_y+1][search_x]==0 ||
			ch_image[search_y+2][search_x+1]==0 || ch_image[search_y+1][search_x+2]==0 || ch_image[search_y][search_x+1]==0)&& buffer[search_y+1][search_x+1]==0)
		{
			ch_count[ch_t]=2;
			ch_t++;
			buffer[search_y][search_x]=ch_image[search_y][search_x];
			selection[search_y][search_x]=2;
			search_x=search_x+1;
			search_y=search_y+1;
		}

		else if(ch_image[search_y][search_x+1]!=0
			&& (ch_image[search_y][search_x+2]==0
			|| ch_image[search_y-1][search_x+1]==0 || ch_image[search_y+1][search_x+1]==0 || ch_image[search_y][search_x]==0) && buffer[search_y][search_x+1]==0)
		{
			ch_count[ch_t]=3;
			ch_t++;
			buffer[search_y][search_x]=ch_image[search_y][search_x];
			selection[search_y][search_x]=3;
			search_x=search_x+1;
			search_y=search_y;
		}

		else if(ch_image[search_y-1][search_x+1]!=0
			&& (ch_image[search_y-1][search_x+2]==0
			|| ch_image[search_y-1][search_x]==0 || ch_image[search_y-2][search_x+1]==0||ch_image[search_y][search_x+1]==0) && buffer[search_y-1][search_x+1]==0)
		{
			ch_count[ch_t]=4;
			ch_t++;
			buffer[search_y][search_x]=ch_image[search_y][search_x];
			selection[search_y][search_x]=4;
			search_x=search_x+1;
			search_y=search_y-1;
		}

		else if(ch_image[search_y-1][search_x]!=0
			&& (ch_image[search_y-1][search_x+1]==0 ||
			ch_image[search_y-2][search_x]==0 || ch_image[search_y-1][search_x-1]==0 || ch_image[search_y][search_x]==0)&& buffer[search_y-1][search_x]==0)
		{
			ch_count[ch_t]=5;
			ch_t++;
			buffer[search_y][search_x]=ch_image[search_y][search_x];
			selection[search_y][search_x]=5;
			search_x=search_x;
			search_y=(search_y-1);					
		}

		else if(ch_image[search_y-1][search_x-1]!=0
			&& (ch_image[search_y-1][search_x]==0 ||
			ch_image[search_y-1][search_x-2]==0 || ch_image[search_y-2][search_x-1]==0 || ch_image[search_y][search_x-1]==0)&& buffer[search_y-1][search_x-1]==0)
		{
			ch_count[ch_t]=6;
			ch_t++;
			buffer[search_y][search_x]=ch_image[search_y][search_x];
			selection[search_y][search_x]=6;
			search_x=search_x-1;
			search_y=search_y-1;
		}


		else if(ch_image[search_y][search_x-1]!=0
			&& (ch_image[search_y+1][search_x-1]==0 ||
			ch_image[search_y][search_x-2]==0 || ch_image[search_y-1][search_x-1]==0 || ch_image[search_y][search_x]==0)&& buffer[search_y][search_x-1]==0)
		{
			ch_count[ch_t]=7;
			ch_t++;
			buffer[search_y][search_x]=ch_image[search_y][search_x];
			selection[search_y][search_x]=7;
			search_x=search_x-1;
			search_y=search_y;
		}

		else if(ch_image[search_y+1][search_x-1]!=0
			&& (ch_image[search_y+1][search_x]==0 ||
			ch_image[search_y+1][search_x-2]==0 || ch_image[search_y+2][search_x-1]==0 || ch_image[search_y][search_x-1]==0)&& buffer[search_y+1][search_x-1]==0)
		{
			ch_count[ch_t]=8;
			ch_t++;
			buffer[search_y][search_x]=ch_image[search_y][search_x];
			selection[search_y][search_x]=8;
			search_x=search_x-1;
			search_y=search_y+1;
		}

	}
	//체크한 것을 카운트
	for(int i=0; i<100000; i++){

		if(ch_count[i]==1)
		{
			ch_num[1]++;
		}

		else if(ch_count[i]==2)
		{
			ch_num[2]++;
		}

		else if(ch_count[i]==3)
		{
			ch_num[3]++;
		}

		else if(ch_count[i]==4)
		{
			ch_num[4]++;
		}

		else if(ch_count[i]==5)
		{
			ch_num[5]++;
		}

		else if(ch_count[i]==6)
		{
			ch_num[6]++;
		}

		else if(ch_count[i]==7)
		{
			ch_num[7]++;
		}
		else if(ch_count[i]==8)
		{
			ch_num[8]++;
		}
	}

	if(ch_num[1]>90 || ch_num[3]>90 || ch_num[5]>90 || ch_num[7]>90){
		if(debug_1)printf("90일때\n");
		ch_proportion1=80;
	}
	else if(ch_num[1]>70 || ch_num[3]>70 || ch_num[5]>70 || ch_num[7]>70){
		if(debug_1)printf("70일때\n");
		ch_proportion1=60;
	}
	else if(ch_num[1]>50 || ch_num[3]>50 || ch_num[5]>50 || ch_num[7]>50){
		if(debug_1)printf("50일때\n");
		ch_proportion1=40;
	}
	else if(ch_num[1]>30 || ch_num[3]>30 || ch_num[5]>30 || ch_num[7]>30){
		if(debug_1)printf("30일때\n");
		ch_proportion1=20;
	}
	else{
		if(debug_1)printf("천만일때\n");
		ch_proportion1=10000000;
	}



	if(ch_num[1] - ch_num[2] > ch_proportion1)
		ch_sub++;
	if(ch_num[3] - ch_num[2] > ch_proportion1)
		ch_sub++;
	if(ch_num[5] - ch_num[4] > ch_proportion1)
		ch_sub++;
	if(ch_num[3] - ch_num[4] > ch_proportion1)
		ch_sub++;
	if(ch_num[7] - ch_num[6] > ch_proportion1)
		ch_sub++;
	if(ch_num[5] - ch_num[6] > ch_proportion1)
		ch_sub++;
	if(ch_num[1] - ch_num[8] > ch_proportion1)
		ch_sub++;
	if(ch_num[7] - ch_num[8] > ch_proportion1)
		ch_sub++;

	if(ch_num[1] - ch_num[2]  < 0)
		ch_division++;
	if(ch_num[3] - ch_num[2]  < 0)
		ch_division++;
	if(ch_num[5] - ch_num[4] < 0)
		ch_division++;
	if(ch_num[3] - ch_num[4] < 0)
		ch_division++;
	if(ch_num[7] - ch_num[6] < 0)
		ch_division++;
	if(ch_num[5] - ch_num[6] < 0)
		ch_division++;
	if(ch_num[1] - ch_num[8] < 0)
		ch_division++;
	if(ch_num[7] - ch_num[8] < 0)
		ch_division++;

	for(int i=0; i<HEIGHT; i++){
		for(int j=0; j<WIDTH; j++){
			if((ch_sub>=SHAPE_DETECT || ch_division >=SHAPE_DETECT))
			{
				if(buffer[i][j]!=0)
				{
					output[i][j]=255;
				}
				shape_flag=1;
			}
		}
	}


	for(int i = 0; i<HEIGHT; i++)
		histogram_HEIGHT[i] = 0;  

	for(int i= 0; i<WIDTH; i++) 
		histogram_WIDTH[i] = 0;

	max_HEIGHT = 0;
	max_WIDTH = 0;
	max_x = 0;
	max_y = 0;
	search_x=0;
	search_y=0;
	for(int i=0;i<240;i++){
		for(int j=0; j<320; j++){
			buffer[i][j]=0;
		}
	}
	for(int i=0 ; i<100000; i++)
		ch_count[i]=0;
	for(int i=0; i<10; i++)
		ch_num[i]=0;
	ch_t=0;
	ch_sub=0;
	ch_division=0;
	ch_proportion1=0;
	ch_proportion2=0;
	if(shape_flag){
		shape_flag=0;
		return 1;
	}
	else{
		return 0;	
	}
}

void include_erode(BYTE** input, BYTE** output) {	//침식
	int i, j;
	int l, k;
	for(i=(MORPHOLOGY+1); i<HEIGHT-(MORPHOLOGY+1); i++) {
		for(j=(MORPHOLOGY+1); j<WIDTH-(MORPHOLOGY+1); j++) {
			pre_image1[i][j] = input[i][j];

			if(pre_image1[i][j] == 0) {
				for(l=i-(MORPHOLOGY+1); l<i+(MORPHOLOGY+1); l++) {
					for(k=j-(MORPHOLOGY+1); k<j+(MORPHOLOGY+1); k++) {
						output[l][k] = 0;
					}
				}
			}
			else
				output[i][j] = input[i][j];
		}
	}
}

void include_dilate(BYTE** input, BYTE** output) {	//팽창
	int i, j;
	int l, k;
	for(i=MORPHOLOGY; i<HEIGHT-MORPHOLOGY; i++) {
		for(j=MORPHOLOGY; j<WIDTH-MORPHOLOGY; j++) {
			pre_image2[i][j] = input[i][j];

			if(pre_image2[i][j] == 255) {
				for(l=i-MORPHOLOGY; l<i+MORPHOLOGY; l++) {
					for(k=j-MORPHOLOGY; k<j+MORPHOLOGY; k++) {
						output[l][k] = 255;
					}
				}
			}
			else 
				output[i][j] = input[i][j];
		}
	}	
}

void include_Detect_Contour(BYTE** input, BYTE** output) {
	int i, j;

	int x, y, nx, ny;
	int dold, d, cnt;


	int p1_ax = 0, p1_ay = 0, p2_ax = 0, p2_ay = 0;

	bool re_temp = false;
	int dir[8][2] = {
		{1, 0},
		{1, 1},
		{0, 1},
		{-1, 1},
		{-1, 0},
		{-1, -1},
		{0, -1},
		{1, -1}
	};

	ContourPoints cp;
	cp.num = 0;

	for(i = 0 ; i < HEIGHT - 1; i++) {
		for(j = 0; j < WIDTH - 1; j++) {
			if(input[i][j] != 0) {
				y = i;
				x = j;

				dold = 0; d = 0; cnt = 0;


				while(1) {
					nx = x + dir[d][0];
					ny = y + dir[d][1];

					if(nx < 0 || nx >= WIDTH - 1|| ny < 0 || ny >= HEIGHT - 1 || input[ny][nx] == 0) {
						if(++d > 7) {
							d = 0;
						}
						cnt++;

						if(cnt >= 8) {
							cp.x[cp.num] = x;
							cp.y[cp.num] = y;
							cp.num++;

							break;
						}
					}
					else {
						cp.x[cp.num] = x;
						cp.y[cp.num] = y;
						cp.num++;

						if(cp.num >= MAX_CONTOUR) {
							re_temp=true;
							break;

						}
						x = nx;
						y = ny;
						cnt = 0;
						dold = d;
						d = (dold + 6) % 8;
					}

					if(y == i && x == j && d == 0) {
						re_temp=true;
						break;
					}
				}
			}
			if(re_temp)
				break;
		}

	}


	//추가코드
	for(i = 0; i < cp.num; i++) 
	{
		if(p1_ax < cp.x[i]) 
			p1_ax = cp.x[i];	

		if(p1_ay < cp.y[i]) 
			p1_ay = cp.y[i];
	}

	p3_y = p1_ay;
	p3_x = p1_ax;	



	// ---------------------- 최대좌표 ---------------------- //
	// ---------------------- 최소좌표 ---------------------- //

	p2_ax = 319;
	p2_ay = 239;

	for(i = cp.num -1; i >= 0; i--) {
		if(p2_ax > cp.x[i])
			p2_ax = cp.x[i];

		if(p2_ay > cp.y[i]) 
			p2_ay = cp.y[i];
	}

	p4_y = p2_ay;
	p4_x = p2_ax;

	if(p3_x-p4_x > 20 &&  p3_y-p4_y >20)
		for(i = p4_y; i <= p3_y + 1; i++) {
			for(j = p4_x; j <= p3_x + 1; j++) {
				if((p3_y < HEIGHT && p3_x < WIDTH) && (p4_x > 0 && p4_y > 0) && (p4_x < WIDTH && p4_y < HEIGHT) && (p3_x > 0 && p3_y > 0)) {
					if(((i > p4_y && i <= p3_y) && (j > p4_x && j <= p3_x)))
						output[i][j] = input[i][j];
					else
						output[i][j] = 255;
					// 중심좌표 표시
					for(int i = max_y - 1; i <= max_y + 1; i++)
						for(int j = max_x - 1; j <= max_x + 1; j++) {
							if(i < HEIGHT - 1 && j < WIDTH - 1 && i > 1 && j > 1)
								output[i][j] = 0;
						}
				}
			}
		}	
		for(j = 0; j < cp.num; j++)  
			if(((cp.y[j] < HEIGHT ) && (cp.y[j] > 0)) && ((cp.x[j] < WIDTH ) && (cp.x[j] > 0)))
				output[cp.y[j]][cp.x[j]] = 255;

		for(i = 0; i < cp.num; i++) {
			cp.x[i] = 0;
			cp.y[i] = 0;
		}

		cp.num = 0;

		for(int i = 0; i<HEIGHT; i++)
			histogram_HEIGHT[i] = 0;

		for(int i= 0; i<WIDTH; i++) 
			histogram_WIDTH[i] = 0;

		max_HEIGHT = 0;
		max_WIDTH = 0;
		max_x = 0;
		max_y = 0;
}

void include_integral_roi(BYTE** source, int int_img[240][320], int x1, int x2, int y1, int y2)
{
	int height = HEIGHT;
	int width = WIDTH;

	int rs = 0;

	for(int j=x1; j<x2; j++) 
	{
		rs += (int)source[y1][j];
		int_img[y1][j] = rs;
	}

	for(int i=y1+1; i<y2; ++i) 
	{
		rs = 0;
		for(int j=x1; j<x2; ++j) 
		{
			rs += (int)source[i][j];
			int_img[i][j]=rs+int_img[i-1][j];
		}
	}
}

int include_roi_size(BYTE** in,int size, int *y, int *x, int *maxval, int x1, int x2, int y1, int y2){	
	int max=0;
	int val;
	int sz=size/2;
	int BigorNot=0;
	*x=0;
	*y=0;
	if(x1!=0)
		if(debug==1)printf("x1=%d,x2=%d,y1=%d,y2=%d sz=%d \n",x1,x2,y1,y2,size);
	int integral[HEIGHT][WIDTH]={0,};

	//영상의 범위를 벗어나지 않도록 조정해줌
	if(y1<0){	
		y1=0;
	}
	if(y2>=HEIGHT){
		y2=HEIGHT-1;
	}
	if(x1<0){
		x1=0;
	}
	if(x2>=WIDTH){
		x2=WIDTH-1;
	}


	include_integral_roi(in, integral, x1,x2,y1,y2);	//적분이미지 구함

	for(int i=y1+sz; i<y2-sz; i+=1){		//roi의 총합의 평균이 가장 큰 위치를 구함
		for(int j=x1+sz; j<x2-sz; j+=1){
			val=integral[i+sz][j+sz]-integral[i+sz][j-sz]-integral[i-sz][j+sz]+integral[i-sz][j-sz];
			if(max<val){
				*y=i;
				*x=j;
				max=val;
			}
		}
	}
	max=max/((2*sz)*(2*sz)); //정규화

	if(max!=0){
		int max2=0;

		if(sz<50 && sz>5){	//사이즈 줄이고 해보기(이전 프레임의 roi와 같은 크기)
			if(debug==1)printf("되나? 처음 max1=%d \n", max);
			sz-=5;
			val=integral[*y+sz][*x+sz]-integral[*y+sz][*x-sz]-integral[*y-sz][*x+sz]+integral[*y-sz][*x-sz];

			if(max2<val){
				max2=val;
			}

			max2=max2/((2*sz)*(2*sz));	//정규화

			if(debug==1)printf("원본크기 max2=%d\n", max2);

			if(max+3>max2){
				BigorNot=1;	//더커짐
			}
			else{
				max=max2;
				BigorNot=0;	//원래크기
			}

		}



		int max3=0;
		if(BigorNot==0){	//사이즈가 커지지않았다면
			if(sz>5){	//사이즈 줄이고 해보기(작아진크기)
				sz-=5;
				val=integral[*y+sz][*x+sz]-integral[*y+sz][*x-sz]-integral[*y-sz][*x+sz]+integral[*y-sz][*x-sz];
				if(max3<val){
					max3=val;
				}

				max3=max3/((2*sz)*(2*sz));	//정규화
				if(debug==1)printf("작은크기 max3=%d\n", max3);
				if(max>max3){
					BigorNot=0;	//원래크기
				}
				else{
					max=max3;
					BigorNot=-1; 	//작아짐
				}

			}
		}
	}

	*maxval = max;

	if(debug==1)printf("BigorNot=%d\n", BigorNot);
	return BigorNot;
}

dominic spd1;
dominic spd2;
dominic spd3;
dominic spd4;

float spdx, spdy;
float roispeed;


float maxlimit;


track include_tracking_roi(BYTE** red, track ori, int widen,BYTE** out, int *max, BYTE** rimg, BYTE** gimg, BYTE** bimg){
	if(debug==1)printf("include_tracking_roi1\n");
	int i =0;
	int x1, x2, y1, y2, cx, cy;
	int tmp;
	int widensize=widen;
	x1=ori.x1;
	x2=ori.x2;
	y1=ori.y1;
	y2=ori.y2;

	if(redON)maxlimit=track1.level*0.7;
	if(blueON)maxlimit=track2.level*0.7;
	if(yellowON)maxlimit=track3.level*0.7;
	if(greenON)maxlimit=track4.level*0.7;

	int maxclr=20;

	if(redON){
		spd1.kf(ori.x,ori.y);
		spdx=spd1.xv;
		spdy=spd1.yv;
	}

	if(blueON){ 
		spd2.kf(ori.x,ori.y);
		spdx=spd2.xv;
		spdy=spd2.yv;
	}
	if(yellowON){
		spd3.kf(ori.x,ori.y);
		spdx=spd3.xv;
		spdy=spd3.yv;
	}
	if(greenON){
		spd4.kf(ori.x,ori.y);
		spdx=spd4.xv;
		spdy=spd4.yv;
	}

	roispeed= (spdx*spdx+spdy*spdy);

	maxlimit=(-0.9*roispeed+maxlimit);
	maxclr=maxlimit-20;

	maxlimit=30;
	if(maxlimit<5.0f)
		maxlimit=5;

	if(maxclr<0)
		maxclr=0;

	do{

		if(debug==1)printf("1 x2-x1=%d, y2-y1=%d x1=%d,x2=%d,y1=%d,y2=%d\n",x2-x1, y2-y1,x1,x2,y1,y2);
		if(debug==1)printf("1 after+-50 x2-x1=%d, y2-y1=%d x1=%d,x2=%d,y1=%d,y2=%d\n",x2-x1, y2-y1,x1,x2,y1,y2);
		if(debug==1)printf("1 after calib x2-x1=%d, y2-y1=%d x1=%d,x2=%d,y1=%d,y2=%d\n",x2-x1, y2-y1,x1,x2,y1,y2);



		int xp=0;
		int yp=0;
		int *ymax;
		int *xmax;
		int *maxintegral= max;
		ymax=&yp;
		xmax=&xp;
		int bigger;


		//커젓는지작아졋는지 return값 받음
		bigger=include_roi_size(red, (((y2-y1)+(x2-x1))/4)+10, ymax, xmax, maxintegral, x1-widensize, x2+widensize, y1-widensize, y2+widensize);

		cy=yp;
		cx=xp;


		if(debug==1)printf("cy=%d, cx=%d \n", cy, cx);

		if(((y2-y1)+(x2-x1))/2>20){
			if(debug==1)printf("bigger=%d y1=%d,y2=%d,x1=%d,x2=%d\n", bigger, y1,y2,x1,x2);
			if(bigger==1){
				y1-=5;
				y2+=5;
				x1-=5;
				x2+=5;
			}
			else if(bigger==-1){
				y1+=5;
				y2-=5;
				x1+=5;
				x2-=5;
			}
			if(debug==1)printf("1 y1=%d,y2=%d,x1=%d,x2=%d\n", y1,y2,x1,x2);
		}
		else{
			y1-=5;
			y2+=5;
			x1-=5;
			x2+=5;
		}

		if(y1<0){
			y2=y2-y1;
			y1=0;
		}
		if(y2>=HEIGHT){
			y1-=(y2+1)-(HEIGHT-1);
			y2=HEIGHT-1;
		}
		if(x1<0){
			x2=x2-x1;
			x1=0;
		}
		if(x2>=WIDTH){
			x1-=x2+1-(WIDTH-1);
			x2=WIDTH-1;
		}


		if(debug==1)printf("2 x2-x1=%d, y2-y1=%d x1=%d,x2=%d,y1=%d,y2=%d\n",x2-x1, y2-y1, x1,x2,y1,y2);

		tmp=(y2-y1)/2;
		y1=cy-tmp;
		y2=cy+tmp;		

		tmp=(x2-x1)/2;
		x1=cx-tmp;
		x2=cx+tmp;


		//수정중
		if(redON==1){
			if(track1.level<*maxintegral)track1.level=*maxintegral;

			if(*maxintegral>maxclr*RATIO && x2-x1>=maxlimit)
				redstrong=1;
			else{
				redstrong=0;
				if(*maxintegral<=maxclr)
					if(debug)printf("빨강이 위험합니다!!\n");
				if(x2-x1<maxlimit)
					if(debug)printf("빨강이 죽었습니다!!\n");
				break;
			}
		}
		if(blueON==1){
			if(track2.level<*maxintegral)track2.level=*maxintegral;
			if(*maxintegral>maxclr*RATIO && x2-x1>=maxlimit)
				bluestrong=1;
			else{
				bluestrong=0;
				if(*maxintegral<=maxclr)
					if(debug)printf("파랭이 위험합니다!!\n");
				if(x2-x1<maxlimit)
					if(debug)printf("파랭이 죽었습니다!!\n");
				break;
			}
		}
		if(yellowON==1){
			if(track3.level<*maxintegral)track3.level=*maxintegral;

			if(*maxintegral>maxclr*RATIO && x2-x1>=maxlimit)
				yellowstrong=1;
			else{
				yellowstrong=0;
				if(*maxintegral<=maxclr)
					if(debug)printf("노랭이 위험합니다!!\n");
				if(x2-x1<maxlimit)
					if(debug)printf("노랭이 죽었습니다!!\n");
				break;
			}
		}
		if(greenON==1){
			if(track4.level<*maxintegral)track4.level=*maxintegral;
			if(*maxintegral>maxclr*RATIO && x2-x1>=maxlimit)
				greenstrong=1;
			else{
				greenstrong=0;
				if(*maxintegral<=maxclr)
					if(debug)printf("초록이 위험합니다!!\n");
				if(x2-x1<maxlimit)
					if(debug)printf("초록이 죽었습니다!!\n");
				break;
			}
		}

		if(debug==1)printf("2 y1=%d,y2=%d,x1=%d,x2=%d\n", y1,y2,x1,x2);
		i+=1;
	}while(i<2);

	if(redON)include_drawing_circle(cx, cy, (x2-x1)/2, rimg);
	if(blueON)include_drawing_circle(cx, cy, (x2-x1)/2, bimg);
	if(yellowON)include_drawing_circle(cx, cy, (x2-x1)/2, rimg, gimg);
	if(greenON)include_drawing_circle(cx, cy, (x2-x1)/2, gimg);

	if(redON)spd1.kalman(cx+(int)spd1.xv,cy+(int)spd1.yv);
	if(blueON)spd2.kalman(cx+(int)spd2.xv,cy+(int)spd2.yv);
	if(yellowON)spd3.kalman(cx+(int)spd3.xv,cy+(int)spd3.yv);
	if(greenON)spd4.kalman(cx+(int)spd4.xv,cy+(int)spd4.yv);

	include_drawing_circle(spd1.kalman_x, spd1.kalman_y, 6, rimg);
	include_drawing_circle(spd2.kalman_x, spd2.kalman_y, 6, bimg);
	include_drawing_circle(spd3.kalman_x, spd3.kalman_y, 6, rimg, gimg);
	include_drawing_circle(spd4.kalman_x, spd4.kalman_y, 6, gimg);		

	if(debug==1)printf("3 y1=%d,y2=%d,x1=%d,x2=%d\n", y1,y2,x1,x2);

	ori.x1=x1;
	ori.x2=x2;
	ori.y1=y1;
	ori.y2=y2;

	if(redON){ori.x=spd1.kalman_x;ori.y=spd1.kalman_y;}
	if(blueON){ori.x=spd2.kalman_x;ori.y=spd2.kalman_y;}
	if(yellowON){ori.x=spd3.kalman_x;ori.y=spd3.kalman_y;}
	if(greenON){ori.x=spd4.kalman_x;ori.y=spd4.kalman_y;}

	if(debug==1)printf("복사 전= x=%d, y=%d \n", ori.x, ori.y);

	if(debug==1)printf("include_tracking_roi2\n");
	return ori;
}

void include_drawing_circle(int x1, int y1, int r, BYTE** out){
	if(debug==1)printf("include_drawing_circle1\n");

	//x축을 기준으로 한번
	for(int x=-r; x<r; x++){
		int y=powf( (r*r-x*x),0.5);	//뮤ㅜ슨연산?

		if(((x+x1)>=0)&&((x+x1)<WIDTH)){

			if(((y+y1)>=0)&&((y+y1)<HEIGHT)){//윗반원
				out[y+y1][x+x1]=255;
			}
			if(((y1-y)>=0)&&((y1-y)<HEIGHT)){//아랫반원
				out[y1-y][x+x1]=255;
			}

		}
	}
	//y축을 기준으로 두번 그린다.
	for(int y=-r; y<r; y++){
		int x=powf( (r*r-y*y),0.5);
		if(((y+y1)>=0)&&((y+y1)<HEIGHT)){
			if(((x+x1)>=0)&&((x+x1)<WIDTH))//윗반원
				out[y+y1][x+x1]=255;
			if(((x1-x)>=0)&&((x1-x)<WIDTH))//아랫반원
				out[y1+y][x1-x]=255;
		}
	}	
	if(debug==1)printf("include_drawing_circle2\n");
}

void include_drawing_circle(int x1, int y1, int r, BYTE** out, BYTE** out2){
	if(debug==1)printf("include_drawing_circle1\n");

	//x축을 기준으로 한번
	for(int x=-r; x<r; x++){
		int y=powf( (r*r-x*x),0.5);	//뮤ㅜ슨연산?

		if(((x+x1)>=0)&&((x+x1)<WIDTH)){

			if(((y+y1)>=0)&&((y+y1)<HEIGHT)){//윗반원
				out[y+y1][x+x1]=255;
				out2[y+y1][x+x1]=255;
			}
			if(((y1-y)>=0)&&((y1-y)<HEIGHT)){//아랫반원
				out[y1-y][x+x1]=255;
				out2[y1-y][x+x1]=255;
			}

		}
	}
	//y축을 기준으로 두번 그린다.
	for(int y=-r; y<r; y++){
		int x=powf( (r*r-y*y),0.5);

		if(((y+y1)>=0)&&((y+y1)<HEIGHT)){
			if(((x+x1)>=0)&&((x+x1)<WIDTH)){//윗반원
				out[y+y1][x+x1]=255;
				out2[y+y1][x+x1]=255;
			}
			if(((x1-x)>=0)&&((x1-x)<WIDTH)){//아랫반원
				out[y1+y][x1-x]=255;
				out2[y1+y][x1-x]=255;
			}
		}
	}

	if(debug==1)printf("include_drawing_circle2\n");
}

void Init_track(track trk){
	trk.level=0;
	trk.x=WIDTH/2;
	trk.x1=0;
	trk.x2=WIDTH;
	trk.y=HEIGHT/2;
	trk.y1=0;
	trk.y2=HEIGHT/2;
}

void include_Labeling(BYTE** input, BYTE** output) {
	register int i, j;
	int **map = new int*[HEIGHT];

	for(i=0; i<HEIGHT; i++) {
		map[i] = new int[WIDTH];
		memset(map[i], 0, sizeof(int)*WIDTH);
	}

	int eq_tbl[MAX_LABEL][2] = { {0, }, };
	int lable_cnt[MAX_LABEL][2] = { {0, }, };	//레이블 카운터 배열

	
	int label = 0, maxl, minl, min_eq, max_eq;

	for(i=1; i<HEIGHT; i++) {
		for(j=1; j<WIDTH; j++) {
			if(input[i][j] != 0) {
				if((map[i-1][j] != 0) && (map[i][j-1] != 0)) { 
					if(map[i-1][j] == map[i][j-1]) {
						map[i][j] = map[i-1][j];
					}
					else {
						maxl = max(map[i-1][j], map[i][j-1]);
						minl = min(map[i-1][j], map[i][j-1]);

						map[i][j] = minl;

						min_eq = min(eq_tbl[maxl][1], eq_tbl[minl][1]);
						max_eq = max(eq_tbl[maxl][1], eq_tbl[minl][1]);

						eq_tbl[maxl][1] = min_eq;
						eq_tbl[minl][1] = min_eq;
					}
				}
				else if(map[i-1][j] != 0) {
					map[i][j] = map[i-1][j];
				}
				else if(map[i][j-1] != 0) {
					map[i][j] = map[i][j-1];
				}
				else {
					label++;
					map[i][j] = label;
					eq_tbl[label][0] = label;
					eq_tbl[label][1] = label;
					lable_cnt[label][0] = label;
				}
			}
		}
	}

	int temp;
	
	//page.664 그림 g과정
	for(i=1; i<label; i++) {
		temp = eq_tbl[i][1];
		if(temp != eq_tbl[i][0])
			eq_tbl[i][1] = eq_tbl[temp][1];
	}


	//순서대로 레이블을 1부터 증가
	int* hash = new int[label+1];
	memset(hash, 0, sizeof(int)*(label+1));

	for(i=1; i<=label; i++)
		hash[eq_tbl[i][1]] = eq_tbl[i][1];

	int cnt = 1;
	for(i=1; i<=label; i++)
		if(hash[i] != 0)
			hash[i] = cnt++;

	for(i=1; i<=label; i++)
		eq_tbl[i][1] = hash[eq_tbl[i][1]];

	delete [] hash;

	//lable_cnt의 값을 카운트한다.
	for(i=1; i<HEIGHT; i++){
		for(j=1; j<WIDTH; j++) {
			if(map[i][j] != 0) {
				temp = map[i][j];
				int temp_cnt = eq_tbl[temp][1];
				
				if(eq_tbl[temp][1] == lable_cnt[temp_cnt][0])
					lable_cnt[temp_cnt][1]++;
			}
		}
	}

	int xy_table[10000][4]={{0,},};
	int max_table = 0;

	for(j=1; j<10000; j++)
	{
		if((eq_tbl[j][1])) 
			max_table = max((eq_tbl[j][1]),(eq_tbl[j-1][1]));
		else	
			break;
	}

	//버블정렬로 lable_cnt의 값을 정렬한다.
	for(i=0; i<max_table; i++){
		for(j=0; j<max_table;j++){
			if(lable_cnt[j][1] < lable_cnt[j+1][1]){
				temp = lable_cnt[j][1];
				lable_cnt[j][1]= lable_cnt[j+1][1];
				lable_cnt[j+1][1] = temp;

				temp = lable_cnt[j][0];
				lable_cnt[j][0]= lable_cnt[j+1][0];
				lable_cnt[j+1][0] = temp;
			}
		}
	}

	//최대 크기 3개만 출력. 근데 소스가 너무 허접해서 수정 생각중.
	for(i=1; i<HEIGHT; i++)
		for(j=1; j<WIDTH; j++) {
			if(map[i][j] != 0) {
				temp = map[i][j];
				if(eq_tbl[temp][1] == lable_cnt[0][0]) {
					output[i][j] = 255;
				}
			}
		}

	for(i=0; i<HEIGHT; i++)
		delete [] map[i];

	delete [] map;
}