#include "stdafx.h"
#include "include_color.h"
#include "ImageTool.h"
#include "ImageToolDlg.h"
#include <stdio.h>
#include <fstream>
using namespace std;


struct track{
	int x;
	int y;
	int x1;
	int x2;
	int y1;
	int y2;
	int level;
};

extern int redON;
extern int blueON;
extern int yellowON;
extern int greenON;

extern struct track track1;
extern struct track track2;
extern struct track track3;
extern struct track track4;

extern int redlost;
extern int bluelost;
extern int yellowlost;
extern int greenlost;

extern int prstrong;
extern int pbstrong;
extern int pystrong;
extern int pgstrong;

//입력 받은 영상을 적분 이미지로 변환시키는 함수
void include_integral(BYTE** source, int int_img[240][320])
{
	int height = HEIGHT;
	int width = WIDTH;

	int rs = 0;
	for(int j=0; j<width; j++) 
	{
		rs += (int)source[0][j];
		int_img[0][j] = rs;
	}
	for(int i=1; i<height; ++i) 
	{
		rs = 0;
		for(int j=0; j<width; ++j) 
		{
			rs += (int)source[i][j];
			int_img[i][j]=rs+int_img[i-1][j];
		}
	}


}

//최대 평균을 가지는 블록의 중앙점 (y,x)와 그 평균값인 maxval을 리턴
void include_max_roi(BYTE** in,int size, int *y, int *x, int *maxval){	
	int max=0;
	int val;
	int sz=size/2;
	*x=0;
	*y=0;
	int integral[HEIGHT][WIDTH]={0,};
	include_integral(in, integral);
	for(int i=0+sz; i<HEIGHT-sz; i+=1){
		for(int j=0+sz; j<WIDTH-sz; j+=1){
			val=integral[i+sz][j+sz]-integral[i+sz][j-sz]-integral[i-sz][j+sz]+integral[i-sz][j-sz];
			if(max<val){
				*y=i;
				*x=j;
				max=val;
			}
		}
	}
	*maxval = max/((2*sz)*(2*sz));
}

//r-g, r-b의 차이로 r 부분 표시
void draw_red_dif(BYTE** red,BYTE** green,BYTE** blue, BYTE** out){	
	int r,g,b;              
	//int temp;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			r=red[i][j];
			g=green[i][j];
			b=blue[i][j];
			if(max(r,max(g,b)) == r){
				out[i][j]=((r-g)+(r-b) -abs(b-g)/2)/2;
				if((out[i][j]<80||out[i][j]>190))      // sh_modified
					out[i][j]=0;
				else
					out[i][j]=255;
				if(abs(b-g)>50)
					out[i][j]=0;
			}
		}
	}
}

void draw_green_dif(BYTE** red,BYTE** green,BYTE** blue, BYTE** out){	
	int r,g,b;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			r=red[i][j];
			g=green[i][j];
			b=blue[i][j];
			if(max(r,max(g,b)) == g){
				if(max(r,b)==r){
					out[i][j]=((g-r)+(g-b) -abs(r-b)/2)/2;
					if(out[i][j]<120&&out[i][j]>50)
						out[i][j]=255;
					else
						out[i][j]=0;
				}
			}
		}
	}
}

void draw_yellow_dif(BYTE** red,BYTE** green,BYTE** blue, BYTE** out){	
	int r,g,b;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			r=red[i][j];
			g=green[i][j];
			b=blue[i][j];
			if(min(r,min(g,b)) == b){ //yellow의 경우 blue가 최소이고
				out[i][j]=((r-b)+(g-b)-abs(r-g)/2)/2; //r-g가 차이가 거의 없어야함, /2는 노말라이즈
				if(out[i][j]<180&&out[i][j]>50){    // sh_modified
					if(abs(r-g)<50)
						out[i][j]=255;
					else
						out[i][j]=0;
				}
				else
					out[i][j]=0;
			}
		}
	}
}

void draw_blue_dif(BYTE** red ,BYTE** green,BYTE** blue, BYTE** out){	
	int r,g,b;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			r=red[i][j];
			g=green[i][j];
			b=blue[i][j];
			if(max(r,max(g,b)) == b){	//blue가 최대이고
				out[i][j]=((b-r)+(b-g)-abs(r-g)/2)/2;	//r-g가 차이가 거의 없어야함, /2는 노말라이즈
				if(out[i][j]<130&&out[i][j]>50) 
					out[i][j]=255;
				else
					out[i][j]=0;
				if(abs(b-r)<100)
					out[i][j]=0;
			}
		}
	}
}

//maxv-variable보다 큰 값을 가지는 부분을 out에 출력
void include_draw_color(BYTE** dif, BYTE** out, int* maxv, int variable){
	int thresh=*maxv;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			if(dif[i][j]>thresh-variable)
				out[i][j]=255;
		}
	}
}

void clear_img(BYTE** tmp){
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			tmp[i][j]=0;
		}
	}
}


int boxblock=5;	//박스 검색시 범위
void include_detect_color(BYTE** red,BYTE** green,BYTE** blue, BYTE** tmp, BYTE** out, int whichcolor){
	int *xmax;
	int *ymax;
	int *maxval;
	int maxv=0;
	int a=0;
	int b=0;
	xmax=&a;
	ymax=&b;
	maxval=&maxv;
	int limit;
	float variable=0.3f;

	if(whichcolor==1){
		draw_red_dif(red,green,blue, tmp);
		include_max_roi(tmp, boxblock, ymax, xmax, maxval);

		if(redlost==0 && prstrong==0){
			track1.level=*maxval;
		}
		limit=*maxval*variable;
		include_draw_color(tmp, out, maxval, limit);
	}
	else if(whichcolor==2){
		draw_blue_dif(red,green,blue, tmp);
		include_max_roi(tmp, boxblock, ymax, xmax, maxval);

		if(bluelost==0 && pbstrong==0) 
			track2.level=*maxval;

		limit=*maxval*variable;
		include_draw_color(tmp, out, maxval, limit);

	}
	else if(whichcolor==3){
		draw_yellow_dif(red,green,blue, tmp);
		include_max_roi(tmp, boxblock, ymax, xmax, maxval);

		if(yellowlost==0 && pystrong==0) 
			track3.level=*maxval;

		limit=*maxval*variable;
		include_draw_color(tmp, out, maxval, limit);
	}
	else if(whichcolor==4){
		draw_green_dif(red,green,blue, tmp);
		include_max_roi(tmp,boxblock, ymax, xmax, maxval);

		if(greenlost==0 && pgstrong==0) 
			track4.level=*maxval;

		limit=*maxval*variable*1.5;
		include_draw_color(tmp, out, maxval, limit);
	}
}



