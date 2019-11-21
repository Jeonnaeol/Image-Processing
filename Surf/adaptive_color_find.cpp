#include "stdafx.h"
#include "hm_adaptive_color_find.h"
#include "ImageTool.h"
#include "ImageToolDlg.h"
#include <stdio.h>
#include <fstream>
using namespace std;



void hm_integral(BYTE** source, int int_img[240][320])
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
			//printf("int_img[i][j]=%d\n", int_img[i][j]);
		}
	}

}

//r-g, r-b�� ���̷� r �κ� ǥ��
void draw_red_dif(BYTE** red,BYTE** green,BYTE** blue, BYTE** out){	
	int r,g,b;
	for(int i = 0; i < HEIGHT; i++) {
		for(int j = 0; j < WIDTH; j++) {
			r=red[i][j];
			g=green[i][j];
			b=blue[i][j];
			if(max(r,max(g,b)) == r)
				out[i][j]=((r-g)+(r-b) -abs(b-g))/2;
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
			if(max(r,max(g,b)) == g)
				out[i][j]=((g-r)+(g-b) -abs(r-b))/2;
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
			if(min(r,min(g,b)) == b)	//yellow�� ��� blue�� �ּ��̰�
				out[i][j]=((r-b+g-b)-abs(r-g))/2;	//r-g�� ���̰� ���� �������, /2�� �븻������
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
			if(max(r,max(g,b)) == b)	//blue�� �ִ��̰�
				out[i][j]=((b-r)+(b-g)-abs(r-g))/2;	//r-g�� ���̰� ���� �������, /2�� �븻������
		}
	}
}
