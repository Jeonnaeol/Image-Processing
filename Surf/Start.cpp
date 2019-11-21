#include "stdafx.h"
#include "Start.h"
#include "surfstart.h"

#include <stdio.h>
#include <fstream>
#include "ImageTool.h"
#include "ImageToolDlg.h"
#include <conio.h>

using namespace std;

#define HEIGHT2 240*2
#define WIDTH2 320*2

int sh_test = 36;
extern int global_variable_test;
///////////////////////////////////////////////
int c_x1=0, c_y1=0;
int c_x2=0, c_y2=0;
int c_x3=0, c_y3=0;
int c_x4=0, c_y4=0;

int selected_h=90;
int feature_size[4] = {0, };
////////////////////////////////////////////////
mfeatures matchedfeats1,matchedfeats2,matchedfeats3,matchedfeats4,matchedfeats5;
features featpoints, conchip, saeu, cola,georg,allfeatures, gamja2;
std::vector<int> conchipl, saeul, colal, georgl;

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) < (b) ? (b) : (a))

struct yj_line{
	int x1,y1;
	int x2,y2;
	double d;
};
///////////////////////////////////
#define bg 0
#if bg
extern int sh_test;
extern BYTE sh_bg[10][HEIGHT][WIDTH];
extern bool sh_bgBlock[48][64];	// 5x5pixel = 1Block
extern BYTE sh_bgBlock_cnt[48][64];	// 5x5pixel = 1Block
extern long tick1, tick2;
long tick3;
#endif

int isExist[4]={0, };





#define HEIGHT 480/2
#define WIDTH 640/2

inline void sh_object_square2(BYTE** image, BYTE** image2, BYTE** image3, int x, int y, mfeatures feature, int num) {

	int top=0, bottom=0, left=0, right=0;
	int size = feature.size();
	//int th = (size / 10) + 1;
	int th = 0;
	if(size != 0) {
		isExist[num-1] = 0;

		for(int i=0; i<240; i++) {
			for(int j=0; j<320; j++) {
				image[i][j] = 0;
			}
		}

		for(int i=0; i<size; i++) {
			image[(int)((feature[i].first.y)/2)][(int)((feature[i].first.x)/2)] = 255;
		}

		
		// top
	for(int i=240-10; i>10; i--) {
		int line_cnt=0;
		for(int j=0; j<320; j++) {
			if(image[i][j] == 255) {
				line_cnt++;
			}
		}
		if(line_cnt > th) {
			top = i;
			break;
		}
		top = i;
	}

	// bottom
	for(int i=10; i<240-10; i++) {
		int line_cnt=0;
		for(int j=0; j<320; j++) {
			if(image[i][j] == 255) {
				line_cnt++;
			}
		}
		if(line_cnt > th) {
			bottom = i;
			break;
		}
		bottom = i;
	}

	// left
	int kk=0;
	for(int j=10; j<320-10; j++) {
		int line_cnt=0;
		
		for(int i=0; i<240; i++) {
			if(image[i][j] == 255) {
				line_cnt++;
			}
		}
		if(line_cnt > th) {
			if(num == 2) {
				if(kk==0) {
					kk++;
				}else {
					left = j;
					break;
				}
			}else {
				left = j;
				break;
			}
		}
		left = j;
	}

	// right
	kk=0;
	for(int j=320-10; j>10; j--) {
		int line_cnt=0;
		for(int i=0; i<240; i++) {
			if(image[i][j] == 255) {
				line_cnt++;
			}
		}
		
		if(line_cnt > th) {
			if(num == 2) {
				if(kk==0) {
					kk++;
				}else {
					right = j;
					break;
				}
			}else {
				right = j;
				break;
			}
		}
		right = j;
	}
	}else {
		
		isExist[num-1] ++;
		top = y+10;
		bottom = y-10;
		left = x-10;
		right = x+10;

	}

	
		top = top+10>239 ? 239 : top+10;
		bottom = bottom-10<0 ? 0 : bottom-10;
		left = left-10<0 ? 0 : left-10;
		right = right+10>319 ? 319 : right+10;


	for(int i=0; i<240; i++) {
		for(int j=0; j<320; j++) {
			image[i][j] = 0;
			image2[i][j] = 0;
			image3[i][j] = 0;
		}
	}
	if(isExist[num-1] < 4) {
		switch(num) {
		case 1:
			for(int i=bottom; i<top; i++) {
				image[i][left] = 255;
				image[i][right] = 255;
			}
			for(int j=left; j<right; j++) {
				image[bottom][j] = 255;
				image[top][j] = 255;
			}
			break;
		case 2:
			for(int i=bottom; i<top; i++) {
				image2[i][left] = 255;
				image2[i][right] = 255;
			}
			for(int j=left; j<right; j++) {
				image2[bottom][j] = 255;
				image2[top][j] = 255;
			}
			printf("$$$$%d, %d, %d, %d\n", top, bottom, left, right);
			break;
		case 3:
			for(int i=bottom; i<top; i++) {
				image3[i][left] = 255;
				image3[i][right] = 255;
			}
			for(int j=left; j<right; j++) {
				image3[bottom][j] = 255;
				image3[top][j] = 255;
			}
			
			break;
		case 4:
			for(int i=bottom; i<top; i++) {
				image[i][left] = 255;
				image[i][right] = 255;
				image3[i][left] = 255;
				image3[i][right] = 255;
			}
			for(int j=left; j<right; j++) {
				image[bottom][j] = 255;
				image[top][j] = 255;
				image3[bottom][j] = 255;
				image3[top][j] = 255;
			}
			break;
		}
	}
	
	
}



///////////////////////////////////

void yj_drawline(int x1,int y1,int x2,int y2, BYTE** outputCam){
	x1/=2;
	y1/=2;
	x2/=2;
	y2/=2;
	if(x2-x1 != 0){
		double d=((double)y2-y1)/((double)x2-x1);
		if((0<=d && d<1)||(-1<d && d<0))
			for(int x=min(x1,x2);x<=max(x1,x2);x++){
				int y =(int)(d*((double)x-x1)+y1);
				outputCam[y][x]=255;
				
			}
		else
			for(int y=min(y1,y2);y<=max(y1,y2);y++){
				int x = (int)(((double)y-y1)/d + x1);
				outputCam[y][x] = 255;
				
			}
	}
	else{
		for(int y=min(y1,y2);y<=max(y1,y2);y++){
			outputCam[y][x1] = 255;
		}
	}
}

void yj_drawcircle(int x1, int y1, int r, BYTE** outputCam){
	x1/=2;
	y1/=2;
	r/=2;

	for(int x=-r; x<r; x++){
		int y=(int)(powf( (float)(r*r-x*x),0.5));

		if(((x+x1)>=0)&&((x+x1)<320)){
			if(((y+y1)>=0)&&((y+y1)<240))
				outputCam[y+y1][x+x1]=255;
			if(((y1-y)>=0)&&((y1-y)<240))
				outputCam[y1-y][x+x1]=255;
		}
	}

	for(int y=-r; y<r; y++){
		int x=(int)(powf( (float)(r*r-y*y),0.5));

		if(((y+y1)>=0)&&((y+y1)<240)){
			if(((x+x1)>=0)&&((x+x1)<320))
				outputCam[y+y1][x+x1]=255;
			if(((x1-x)>=0)&&((x1-x)<320))
				outputCam[y1+y][x1-x]=255;
		}
	}
}

void yj_drawcircle2(int x1, int y1, int r, BYTE** outputCam){
	x1/=2;
	y1/=2;
	r/=2;
	for(int x=-r; x<r; x++){
		int y=(int)(powf( (float)(r*r-x*x),0.5));

		if(((x+x1)>=0)&&((x+x1)<320)){
			if(((y+y1)>=0)&&((y+y1)<240))
				outputCam[y+y1][x+x1]=0;
			if(((y1-y)>=0)&&((y1-y)<240))
				outputCam[y1-y][x+x1]=0;
		}
	}

	for(int y=-r; y<r; y++){
		int x=(int)(powf( (float)(r*r-y*y),0.5));

		if(((y+y1)>=0)&&((y+y1)<240)){
			if(((x+x1)>=0)&&((x+x1)<320))
				outputCam[y+y1][x+x1]=0;
			if(((x1-x)>=0)&&((x1-x)<320))
				outputCam[y1+y][x1-x]=0;
		}
	}


}

inline int rnd(float flt)
{
  return (int) floor(flt+0.5f);
}

void drawIpoints(BYTE** img, std::vector<featurepoint> &featpoints)
{
  featurepoint *ipt;
  float s, o;
  int r1, c1, r2, c2, lap;

  for(unsigned int i = 0; i < featpoints.size(); i++) 
  {
    ipt = &featpoints.at(i);
    s = (2.5f * ipt->scale);
    r1 = rnd(ipt->y);
    c1 = rnd(ipt->x);
    c2 = rnd(s * cos(o)) + c1;
    r2 = rnd(s * sin(o)) + r1;

      yj_drawline(c1, r1, c2, r2, img);
    
      yj_drawcircle(c1,r1,rnd(s),img); 
  }
}


void saveSurf(char *filename, std::vector<featurepoint> &featpoints)
{
  ofstream outfile(filename);

  outfile << "64\n";
  outfile << featpoints.size() << "\n";
  printf("how many saved =%d \n", featpoints.size());

  for(unsigned int i=0; i < featpoints.size(); i++) 
  {
    outfile << featpoints.at(i).scale << "  ";
    outfile << featpoints.at(i).x << " ";
    outfile << featpoints.at(i).y << " ";
	outfile << featpoints.at(i).color << " ";
    outfile << featpoints.at(i).orientation << " ";
    outfile << featpoints.at(i).laplacian << " ";
    outfile << featpoints.at(i).level << " ";
    for(int j=0; j<64; j++)
      outfile << featpoints.at(i).descriptor[j] << " ";

    outfile << "\n";
  }
  printf("asdasdasd");
  outfile.close();
}
void loadSurf(char *filename, std::vector<featurepoint> &featpoints)
{
  int descriptorLength, count;
  ifstream infile(filename);

  featpoints.clear();

  infile >> descriptorLength;
  infile >> count;
  printf("how many loaded =%d \n", count);

  for (int i = 0; i < count; i++) 
  {
    featurepoint ipt;

    infile >> ipt.scale; 
    infile >> ipt.x;
    infile >> ipt.y;
	infile >> ipt.color;
    infile >> ipt.orientation;
    infile >> ipt.laplacian;
    infile >> ipt.level;

    for (int j = 0; j < 64; j++)
      infile >> ipt.descriptor[j];

    featpoints.push_back(ipt);

  }
}
void saveLen(char *filename, std::vector<int> &length)
{
  ofstream outfile(filename);

  outfile << "999\n";
  outfile << length.size() << "\n";
  printf("how many lengths saved =%d \n", length.size());

  for(unsigned int i=0; i < length.size(); i++) 
  {
	  printf("%d ", length.at(i));
    outfile << length.at(i);
    outfile << "\n";
  }

  outfile.close();
}


void loadLen(char *filename, std::vector<int> &length)
{
  int lengths, count;
  ifstream infile(filename);

  length.clear();

  infile >> lengths;
  infile >> count;

  printf("how many length loaded=%d\n",count);

  for (int i = 0; i < count; i++) 
  {
    int len;
    infile >> len; 
	//printf("len");
    length.push_back(len);
  }
}


int setup(){
	first:
	printf("\n1.ÃÔ¿µ, 2.ÀúÀå 3.·Îµå, 4.ºñ±³, 0.Á¾·á\n");
	char k;
	flushall();
	scanf("%c", &k);
	if(k=='1'){
		printf("\n1.ÄÜÄ¨, 2.»õ¿ì±ø, 3.ÄÚÄ«Äİ¶ó, 4.Á¶Áö¾Æ, 0.¼±ÅÃÁö\n");
		flushall();
		scanf("%c", &k);

		if(k=='1')return 1;
		else if(k=='2')return 2;
		else if(k=='3')return 3;
		else if(k=='4')return 4;
		else goto first;	//µ¹¾Æ°¡±â
		
	}
	else if(k=='2'){	//ÀúÀå
		printf("\n1.ÄÜÄ¨, 2.»õ¿ì±ø, 3.ÄÚÄ«Äİ¶ó, 4.Á¶Áö¾Æ, 5. ¸ğµÎ ÀúÀå, 0.¼±ÅÃÁö\n");
		flushall();
		scanf("%c", &k);
		if(k=='1') return 11;
		else if(k=='2') return 12;
		else if(k=='3') return 13;
		else if(k=='4') return 14;
		else if(k=='5') return 15;
		else{
			goto first;
		}
	}
	else if(k=='3'){
		return 21;
	}
	else if(k=='4'){	//ºñ±³ÇÏ±â
		return 31;
	}
	else{	
		printf("¾Æ¹«°Íµµ¾Æ´Ï³× %c\n", k);
		return -1;
	}
}


int surffunction(int func, int save, int x, float thres, int oct, BYTE** input, BYTE** input2,
	int redint[240][320], int blueint[240][320], int yellowint[240][320], BYTE** Red, BYTE** Green, BYTE** Blue, BYTE** H, BYTE** S, BYTE** L, 
	BYTE** BigImage1, BYTE** BigImage2, BYTE** BigImage3){
	
	int length;
	int returnvalue=func;

	if(func==31){	//ºñ±³
		surfing1(input, redint, blueint, yellowint, featpoints, oct, thres, false);
		
		findmatchfeatures(featpoints,conchip,conchipl,matchedfeats1,0);
		findmatchfeatures(featpoints,saeu,saeul,matchedfeats2, 0);
		findmatchfeatures(featpoints,cola,colal,matchedfeats3, 0);
		findmatchfeatures(featpoints,georg,georgl,matchedfeats4, 0);
		
		////////////////////////////////////////////////////// Àü¿ªº¯¼öµµ Ãß°¡ÇÒ °Í
		
		
		sh_object_square2(H, S, L, c_x1, c_y1, matchedfeats1, 1);
		big_print(H, S, L, BigImage1, BigImage2, BigImage3);
		sh_object_square2(H, S, L, c_x2, c_y2, matchedfeats2, 2);
		big_print(H, S, L, BigImage1, BigImage2, BigImage3);
		sh_object_square2(H, S, L, c_x3, c_y3, matchedfeats3, 3);
		big_print(H, S, L, BigImage1, BigImage2, BigImage3);
		sh_object_square2(H, S, L, c_x4, c_y4, matchedfeats4, 4);
		big_print(H, S, L, BigImage1, BigImage2, BigImage3);

		



		////////////////////////////////////////////////////////////

		int size=matchedfeats1.size();
		printf("ÄÜ¯… ¸ÅÄª Á¡=%d\n", size);
		for (int i = 0; i < size; ++i){
			yj_drawcircle((int)(matchedfeats1[i].first.x),(int)(matchedfeats1[i].first.y),20,Blue);
			yj_drawcircle2((int)(matchedfeats1[i].first.x),(int)(matchedfeats1[i].first.y),20,Red);
			yj_drawcircle2((int)(matchedfeats1[i].first.x),(int)(matchedfeats1[i].first.y),20,Green);
		}

		size=matchedfeats2.size();
		printf("»õ¿ì±ø ¸ÅÄª Á¡=%d\n", size);
		for (int i = 0; i < size; ++i){
			yj_drawcircle((int)(matchedfeats2[i].first.x),(int)(matchedfeats2[i].first.y),20,Red);
			yj_drawcircle((int)(matchedfeats2[i].first.x),(int)(matchedfeats2[i].first.y),20,Green);
			yj_drawcircle2((int)(matchedfeats2[i].first.x),(int)(matchedfeats2[i].first.y),20,Blue);
		}

		size=matchedfeats3.size();
		printf("ÄÚÄ«Äİ¶ó ¸ÅÄª Á¡=%d\n", size);
		for (int i = 0; i < size; ++i){
			yj_drawcircle((int)(matchedfeats3[i].first.x),(int)(matchedfeats3[i].first.y),20,Red);
			yj_drawcircle2((int)(matchedfeats3[i].first.x),(int)(matchedfeats3[i].first.y),20,Green);
			yj_drawcircle((int)(matchedfeats3[i].first.x),(int)(matchedfeats3[i].first.y),20,Blue);
		}

		size=matchedfeats4.size();
		printf("Á¶Áö¾Æ ¸ÅÄª Á¡=%d\n\n", size);
		for (int i = 0; i < size; ++i){
			yj_drawcircle2((int)(matchedfeats4[i].first.x),(int)(matchedfeats4[i].first.y),20,Red);
			yj_drawcircle((int)(matchedfeats4[i].first.x),(int)(matchedfeats4[i].first.y),20,Green);
			yj_drawcircle2((int)(matchedfeats4[i].first.x),(int)(matchedfeats4[i].first.y),20,Blue);
		}
	}
	else if(func==1&&save==1){
		printf("\nºÎºĞ ´­¸² x=%d, Ã³À½=%d, Áß°£=%d\n", x,(int)(WIDTH2/2.0*(2/3.0)), (int)(WIDTH2/2.0*(1/3.0)));
		
		if(x > WIDTH2/2.0*(2/3.0)){
			printf("\nÄÜ¯… ÀüÃ¼ ÃÔ¿µ");
			length=surfing1(input, redint, blueint, yellowint, conchip, oct, thres, true);
			drawIpoints(input2, conchip);
		}
		else if(x<=WIDTH2/2.0*(2/3.0) && x>WIDTH2/2.0*(1/3.0)){
			printf("\nÄÜ¯… 1/2 ÃÔ¿µ");
			length=surfing2(input,  redint, blueint, yellowint, conchip, oct, thres, true);
			drawIpoints(input2, conchip);
		}
		else{
			printf("\nÄÜ¯… 1/4 ÃÔ¿µ");
			length=surfing3(input,  redint, blueint, yellowint, conchip, oct, thres, true);
			drawIpoints(input2, conchip);
		}
		conchipl.push_back(length);
		return 1;
	}
	else if(func==2&&save==1){
		if(x > WIDTH2/2.0*(2/3.0)){
			printf("\n»õ¿ì±ø ÀüÃ¼ ÃÔ¿µ");
			length=surfing1(input, redint, blueint, yellowint,  saeu, oct, thres, true);
			drawIpoints(input2, saeu);
		}
		else if(x<=WIDTH2/2.0*(2/3.0) && x>WIDTH2/2.0*(1/3.0)){
			printf("\n»õ¿ì±ø 1/2 ÃÔ¿µ");
			length=surfing2(input, redint, blueint, yellowint,  saeu, oct, thres, true);
			drawIpoints(input2, saeu);
		}
		else{
			printf("\n»õ¿ì±ø 1/4 ÃÔ¿µ");
			length=surfing3(input, redint, blueint, yellowint,  saeu, oct, thres, true);
			drawIpoints(input2, saeu);
		}
		saeul.push_back(length);
		return 2;
	}
	else if(func==3&&save==1){
		if(x > WIDTH2/2.0*(2/3.0)){
			printf("\nÄÚÄ«Äİ¶ó ÀüÃ¼ ÃÔ¿µ");
			length=surfing1(input,  redint, blueint, yellowint, cola, oct, thres, true);
			drawIpoints(input2, cola);
		}
		else if(x<=WIDTH2/2.0*(2/3.0) && x>WIDTH2/2.0*(1/3.0)){
			printf("\nÄÚÄ«Äİ¶ó 1/2 ÃÔ¿µ");
			length=surfing2(input,  redint, blueint, yellowint, cola, oct, thres, true);
			drawIpoints(input2, cola);
		}
		else{
			printf("\nÄÚÄ«Äİ¶ó 1/4 ÃÔ¿µ");
			length=surfing3(input, redint, blueint, yellowint,  cola, oct, thres, true);
			drawIpoints(input2, cola);
		}
		colal.push_back(length);
		return 3;
	}
	else if(func==4&&save==1){
		if(x > WIDTH2/2.0*(2/3.0)){
			printf("\nÁ¶Áö¾Æ ÀüÃ¼ ÃÔ¿µ");
			length=surfing1(input, redint, blueint, yellowint,  georg, oct, thres, true);
			drawIpoints(input2, georg);
		}
		else if(x<=WIDTH2/2.0*(2/3.0) && x>WIDTH2/2.0*(1/3.0)){
			printf("\nÁ¶Áö¾Æ 1/2 ÃÔ¿µ");
			length=surfing2(input, redint, blueint, yellowint,  georg, oct, thres, true);
			drawIpoints(input2, georg);
		}
		else{
			printf("\nÁ¶Áö¾Æ 1/4 ÃÔ¿µ");
			length=surfing3(input, redint, blueint, yellowint,  georg, oct, thres, true);
			drawIpoints(input2, georg);
		}
		georgl.push_back(length);
		return 4;
	}
	
	else if(func==11){	//Æ¯Â¡Á¡ ÀúÀå
		printf("\nÄÜ¯… ÀúÀå");
		saveSurf("conchip.txt", conchip);
		saveLen("conchipl.txt", conchipl);
		return -1;
	}
	else if(func==12){	//Æ¯Â¡Á¡ ÀúÀå
		printf("\n»õ¿ì±ø ÀúÀå");
		saveSurf("saeu.txt", saeu);
		saveLen("saeul.txt", saeul);
		return -1;
	}
	else if(func==13){	//Æ¯Â¡Á¡ ÀúÀå
		printf("\nÄÚÄ«Äİ¶ó ÀúÀå");
		saveSurf("cola.txt", cola);
		saveLen("colal.txt", colal);
		return -1;
	}
	else if(func==14){	//Æ¯Â¡Á¡ ÀúÀå
			printf("\nÁ¶Áö¾Æ ÀúÀå");
		saveSurf("georg.txt", georg);
		saveLen("georgl.txt", georgl);
		return -1;
	}
	else if(func==15){	//¸ğµÎÀúÀå
		printf("\n¸ğµÎÀúÀå ÀúÀå");
		saveSurf("conchip.txt", conchip);
		saveSurf("saeu.txt", saeu);
		saveSurf("cola.txt", cola);
		saveSurf("georg.txt", georg);
		saveLen("conchipl.txt", conchipl);
		saveLen("saeul.txt", saeul);
		saveLen("colal.txt", colal);
		saveLen("georgl.txt", georgl);
		return -1;
	}
	else if(func==21){	//·Îµå
		printf("\n¸ğµÎ ·Îµå");
		loadSurf("conchip.txt", conchip);
		loadSurf("saeu.txt", saeu);
		loadSurf("cola.txt", cola);
		loadSurf("georg.txt", georg);
		loadLen("conchipl.txt", conchipl);
		loadLen("saeul.txt", saeul);
		loadLen("colal.txt", colal);
		loadLen("georgl.txt", georgl);
		return -1;
	}

		for(int k=0; k<WIDTH2/2; k+=1)
			input[HEIGHT2/2][k]=255;

		for(int k=0; k<WIDTH2/4; k+=1)
			input[HEIGHT2/4][k]=255;

		for(int k=0; k<HEIGHT2/2; k+=1)
			input[k][WIDTH2/2]=255;

		for(int k=0; k<HEIGHT2/4; k+=1)
			input[k][WIDTH2/4]=255;


	return returnvalue;
}
void big_print(BYTE** R, BYTE** G, BYTE** B, BYTE** bigR, BYTE** bigG, BYTE** bigB) {
	for(int i=0; i<480; i++) {
		for(int j=0; j<640; j++) {
			int x = j/2;
			int y = i/2;
			if(R[y][x]==255 || G[y][x]==255 || B[y][x]==255) {
				bigR[i][j] = R[y][x];
				bigG[i][j] = G[y][x];
				bigB[i][j] = B[y][x];
			}
		}
	}
}