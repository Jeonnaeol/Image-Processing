#include "stdafx.h"
#include <vector>
#include "Surf.h"

using namespace std;




intimg::intimg(int we, int hi){
	w=we;
	h=hi;
	img= new float*[h];
	for(int i=0;i<h; i++){
		img[i]=new float[w];
	}

}
intimg::~intimg(){
	for(int i = 0; i < h; i++){
		delete img[i];
	}
	delete img;
}

pyramid::pyramid(int we, int hi){
	this->w=we;
	this->h=hi;

	pyrmd=new float*[12];
	lap=new BYTE*[12];
	filter=new int[12];
	width=new int[12];
	height=new int[12];
	offset=new int[12];
	Hx=new float[109];
	Hy=new float[109];
	Dg=new float[109];


	gsz=7;

	gs=new float[gsz*gsz];
	int i=0;


	for(int i=0; i<gsz; i+=1){
		for(int j=0; j<gsz; j+=1){
			gs[i*gsz+j]=1.0/(pi*18)*exp(-(i*i+j*j)/(18.0));
		}
	}

	for(int oct=1; oct<=5; oct+=1){
		for(int intvl=1; intvl<=4; intvl+=1){
			filter[i]=(int)(3*(pow(2.0f,oct)*intvl+1));  

			if(oct<2){
				offset[i]=1;
				width[i]=w;
				height[i]=h;
				pyrmd[i]=new float[w*h];
				lap[i]=new BYTE[w*h];
				i+=1;
			}
			else if(intvl>2){
				offset[i]=(int)(pow(2.0f,(oct-1)));		
				width[i]=w/offset[i];
				height[i]=h/offset[i];
				pyrmd[i]=new float[width[i]*height[i]];
				lap[i]=new BYTE[width[i]*height[i]];
				i+=1;
			}
		}
	}


}

void findmatchfeatures(features &fpoints, features &refpoints, std::vector<int> len, mfeatures &matches, int clear)
{
	float diffrences,small1,small2,sum=0,tmp,tmp2;
	featurepoint *match;
	int r=0,b=0, yel=0;
	//printf("어디서탈난겨");
	matches.clear();
	//printf("어디서탈난겨3");
	int size1=fpoints.size();
	int size2=refpoints.size();

	printf("refsize=%d\n", refpoints.size());

	int length;
	int first=0;
	int lensum=0;

	//printf("어디서탈난겨2");
	for(int m=0; m<len.size(); m+=1){
		length=len.at(m);
		lensum+=len.at(m);
		
		first=lensum-length;
		printf("시작%d, 끝%d\n", first, lensum);
		for(int i=0;i<size1;i++) 
		{
			small1=3.402823466e+38F;
			small2=3.402823466e+38F;
			tmp2=0;
			for(int j = first; j < lensum; j++){	
				sum=0;
				for(int k=0;k<64;k+=1){
					tmp=fpoints[i].descriptor[k] - refpoints[j].descriptor[k]; //-> 이차이 자체가 에러라고 본다
					sum += tmp*tmp; //에러의 제곱을 죵나 더해줌
				}

				diffrences= sum; //에러당
				if(diffrences<small1){
					small2 = small1;
					small1 = diffrences; // 에러
					tmp2=j; //j점을 추가해줘
				}
				else if(diffrences<small2){
					small2 = diffrences;
				}
			}
			if(small1/small2 < 0.4225 && refpoints[tmp2].laplacian==fpoints[i].laplacian 
				 && refpoints[tmp2].color==fpoints[i].color){  //쓰레쉬홀드 만족하면 매칭포인트당?
				matches.push_back(std::make_pair(fpoints[i], refpoints[tmp2]));

			}
		}
	}
	
}
pyramid::~pyramid(){
	for(int y=0; y<12; y++){
		delete  lap[y];
		delete  pyrmd[y];
	}

}


int Surf::godonsurfstart(float** img, int redint[240][320], int blueint[240][320], int yellowint[240][320],std::vector<featurepoint> *fpoints,int octave,float thresh, bool saveornot){
	//printf("어디?");
	this->fpoints=fpoints;
	this->img=img;
	this->thresh = thresh;
	this->octaves=octave;
	this->saveornot=saveornot;

	int startipts;

	if(saveornot==false){
		fpoints->clear();
		startipts=0;
	}
	if(saveornot==true){printf("good");
		startipts=fpoints->size();
	}

	int thiswidth=py->w;
	int thisheight=py->h;


	int cntr;
	if(octaves>=1)cntr=4;
	if(octaves>1)cntr+=2*(octaves-1);
	for (int i = 0; i < cntr; ++i)
		makeintervals(py->pyrmd[i], py->height[i], py->width[i], py->lap[i], py->offset[i], py->filter[i]);
	
	float sumx=0, sumy=0, max=0, ori = 0,firstdeg=0, lastdeg=0;


	int bottomlayer, midlayer, toplayer;
	int howmany=0;
	for (int o = 0; o < octaves; ++o){
		for (int i = 0; i <= 1; ++i){
			if(i==0){
				bottomlayer= ((o+1)*2-3);
				if(o==0){
					bottomlayer+=1;
					midlayer= bottomlayer+1;
				}
				else
					midlayer= bottomlayer+2;
			}
			else{
				bottomlayer=(o+1)*2-1;
				midlayer=bottomlayer+1;
			}
			toplayer=midlayer+1;


			float* b2 = py->pyrmd[bottomlayer];
			float* m2 = py->pyrmd[midlayer];
			float* t2 = py->pyrmd[toplayer];
			int widthb=py->width[bottomlayer];
			int widthm=py->width[midlayer];
			int widtht=py->width[toplayer];
			int heightt=py->height[toplayer];
			int tfilter=py->filter[toplayer];
			int mfilter=py->filter[midlayer];
			int bfilter=py->filter[bottomlayer];
			BYTE* mlap=py->lap[midlayer];
			int toff=py->offset[toplayer];


			int border=(tfilter+1)/(2*toff);
			int offset2=mfilter-bfilter;
			int scm=widthm/widtht;
			int scb=widthb/widtht;
			double dx, dy, ds, dxx, dyy, dss, dxy, dxs, dys;
			double ff,xnorm1, ynorm1, snorm1, xnorm2, ynorm2, snorm2;
			double hessian[9], determin;
			double mok[3];
			double var[3];
			double interp1,interp2,interp3;
			int hb=heightt-border, hw=widtht-border;
			int hor,vert;
			for(hor=border+1;hor<hb;++hor){
				for(vert=border+1;vert<hw;++vert){
					if ((m2[hor*scm*widthm+(vert)*scm] >= thresh) && Surf::betterthanall(hor, vert, t2, m2, b2, widtht, widthm, widthb)){		//Start
						xnorm1=m2[hor*scm*widthm+(vert+1)*scm];
						xnorm2=m2[hor*scm*widthm+(vert-1)*scm];
						ynorm1=m2[(hor+1)*scm*widthm+(vert)*scm];
						ynorm2=m2[(hor-1)*scm*widthm+(vert)*scm];
						snorm1=t2[hor*widtht+vert];
						snorm2=b2[hor*scb*widthb+vert*scb];

						dx=(xnorm1-xnorm2)/2;
						dy=(ynorm1-ynorm2)/2;
						ds=(snorm1-snorm2)/2;

						ff=m2[(hor)*scm*widthm+(vert)*scm];
						dxx=xnorm1+xnorm2-2*ff;
						dyy=ynorm1+ynorm2-2*ff;
						dss=snorm1+snorm2-2*ff;
						dxy=(m2[(hor+1)*scm*widthm+(vert+1)*scm]-m2[(hor+1)*scm*widthm+(vert-1)*scm]-m2[(hor-1)*scm*widthm+(vert+1)*scm]+m2[(hor-1)*scm*widthm+(vert-1)*scm])/4;
						dxs=(t2[hor*widtht+vert+1] -t2[hor*widtht+vert-1]- b2[hor*scb*widthb+(vert+1)*scb] + b2[hor*scb*widthb+(vert-1)*scb])/4;
						dys=(t2[(hor+1)*widtht+vert]-t2[(hor-1)*widtht+vert]-b2[(hor+1)*scb*widthb+vert*scb]+b2[(hor-1)*scb*widthb+vert*scb])/4;

						hessian[0]=dxx;hessian[1]=dxy;hessian[2]=dxs;hessian[3]=dxy;hessian[4]=dyy;hessian[5]=dys;hessian[6]=dxs;hessian[7]=dys;hessian[8]=dss;
						var[0]=dx;var[1]=dy;var[2]=ds;
						determin=determinant(hessian,3);
						inverse_matrix(hessian, determin);
						mul_matrix(hessian,var,mok,3);
						interp1=mok[2];interp2=mok[1];interp3=mok[0];
						if(fabs(interp1)<0.5&&fabs(interp2)<0.5&&fabs(interp3)<0.5){
							featurepoint featpts;				//바꿔야할거
							featpts.scale=(0.1333f)*(mfilter+interp1*offset2);
							featpts.laplacian=mlap[hor*scm*widthm+(vert)*scm];
							featpts.qualify=0;
							featpts.x=(vert+interp3)*toff;
							featpts.y=(hor+interp2)*toff;
							fpoints->push_back(featpts);
							howmany+=1;
						}
					}
				}
			}

		}
	}
	if(saveornot){
		printf("\nhowmany=%d\n", howmany);
		printf("howmanyall=%d\n", howmany+startipts);
	}


	featurepoint* ipt;
	float *hx, *hy, *dg;
	int mgsz=-1*(py->gsz-1), gszm=py->gsz*py->gsz, idx;

	hx=py->Hx;
	hy=py->Hy;
	dg=py->Dg;

	int integratedR;
	int integratedB;
	int integratedY;
	int area, area2;
	int maxcolor;


	int desy,desx,xl,yl,count,i,ix,j,jx,xs,ys;
	float dscale,*dmap,dix,diy,mdx,mdy,co,si,gaus1,gaus2,rx,ry,rrx,rry,dlength,cx=-0.5,cy;
	float scale;
	float gauss,rp,cp,sp;

	for (int o = startipts; o < fpoints->size(); ++o){
		index = o;
		ipt=&fpoints->at(index);
		
		scale=ipt->scale;
		int s=rnd(scale),r=rnd(ipt->y),c=rnd(ipt->x);
		idx = 0;
		area=3*s;
		
		integratedR=getsums(redint, c-area, r-area, c+area, r+area, thisheight, thiswidth);
		integratedB=getsums(blueint, c-area, r-area, c+area, r+area, thisheight, thiswidth);
		integratedY=getsums(yellowint, c-area, r-area, c+area, r+area, thisheight, thiswidth);
		
		for(int i=mgsz;i<=py->gsz-1;++i){
			for(int j=mgsz;j<=py->gsz-1;++j){
				if(i*i+j*j<gszm){	
					gauss = py->gs[abs(i)*py->gsz+abs(j)];
					rp=c+i*s;
					cp=r+j*s;
					sp=2*s;
					hx[idx]=gauss*(getsums(img, rp, cp-sp, rp+sp, cp+sp, thisheight, thiswidth)-1*getsums(img, rp-sp, cp-sp, rp,cp+sp, thisheight, thiswidth));
					hy[idx]=gauss*(getsums(img, rp-sp, cp, rp+sp, cp+sp, thisheight, thiswidth)-1*getsums(img, rp-sp, cp-sp, rp+sp, cp, thisheight, thiswidth));

					if(hx[idx] > 0 && hy[idx] >= 0)
						dg[idx]=atan(hy[idx]/hx[idx]);
					else if(hx[idx] < 0 && hy[idx] >= 0)
						dg[idx]=pi - atan(-hy[idx]/hx[idx]);
					else if(hx[idx] < 0 && hy[idx] < 0)
						dg[idx]=pi + atan(hy[idx]/hx[idx]);
					else if(hx[idx] > 0 && hy[idx] < 0)
						dg[idx]=2*pi - atan(-hy[idx]/hx[idx]);
					else 
						dg[idx]=0;
					++idx;
				}
			}
		}
		//printf("어디4");
		sumx=0; sumy=0; max=0; ori = 0;firstdeg=0; lastdeg=0;
		for(firstdeg = 0; firstdeg < 2*pi;  firstdeg+=0.15) {
			if(firstdeg+pi/3.0>2*pi)lastdeg=firstdeg-5/3.0*pi;
			else lastdeg=firstdeg+pi/3.0;
			sumx=0;
			sumy=0; 
			for(unsigned int k=0;k<idx;++k){
				float deg=dg[k];
				if (firstdeg<lastdeg&&firstdeg<deg&&deg<lastdeg){
					sumx+=hx[k];  
					sumy+=hy[k];
				} 
				else if(lastdeg<firstdeg&&((deg>0&&deg<lastdeg)||(deg>firstdeg&&deg<2*pi))){
					sumx+=hx[k];  
					sumy+=hy[k];
				}
			}
			if (sumx*sumx + sumy*sumy > max){
				max = sumx*sumx + sumy*sumy;

				if(sumx>0&&sumy>=0)
					ori=atan(sumy/sumx);
				else if(sumx<0&&sumy>=0)
					ori=pi-atan(-sumy/sumx);
				else if(sumx<0&&sumy<0)
					ori=pi+atan(sumy/sumx);
				else if(sumx>0&&sumy<0)
					ori=2*pi-atan(-sumy/sumx);
				else
					ori=0;

			}
		}
		
		area2=area*area;
		
		maxcolor=max(integratedR,max(integratedB, integratedY));
		if(maxcolor==integratedR){
			ipt->level=integratedR/area2;
			ipt->color=1;
		}
		else if(maxcolor==integratedB){
			ipt->level=integratedB/area2;
			ipt->color=2;
		}
		else{
			ipt->level=integratedY/area2;
			ipt->color=3;
		}
		
		/*if(maxcolor!=0)
		printf("%d %d \n", maxcolor, ipt->level);
		*/
		ipt->orientation = ori;
		desy=0;desx=0;xl=0;yl=0;count=0;i=0;ix=0;j=0;jx=0;xs=0;ys=0;
		dscale=0;*dmap;dix=0;diy=0;mdx=0;mdy=0;co=0;si=0;gaus1=0;gaus2=0;rx=0;ry=0;rrx=0;rry=0;dlength=0;cx=-0.5;cy=0;
		featurepoint *ipt=&fpoints->at(index);
		dscale=ipt->scale;
		desx=rnd(ipt->x);
		desy=rnd(ipt->y);
		co=cos(ipt->orientation);
		si=sin(ipt->orientation);
		dmap = ipt->descriptor;
		i=-8;
		while(i<12){
			j=-8;
			i=i-4;
			cx+=1.0;
			cy=-0.5;
			while(j<12){
				dix=diy=mdx=mdy=0.0;
				cy+=1.0;
				j=j-4;
				ix=i+5;
				jx=j+5;
				xs=rnd(desx+(-jx*dscale*si+ix*dscale*co));
				ys=rnd(desy+(jx*dscale*co+ix*dscale*si));
				for(int k=i;k<i+9;++k){
					for(int l=j;l<j+9;++l){
						xl=rnd(desx+(-l*dscale*si+k*dscale*co));
						yl=rnd(desy+(l*dscale*co+k*dscale*si));
						gaus1=1.0/(pi*12.5*dscale*dscale)*exp(-((xs-xl)*(xs-xl)+(ys-yl)*(ys-yl))/(2.0*6.25*dscale*dscale));
						rx=getsums(img,xl,yl-rnd(dscale),xl+rnd(dscale),yl+rnd(dscale), thisheight, thiswidth)-1*getsums(img,xl-rnd(dscale),yl-rnd(dscale),xl,yl+rnd(dscale), thisheight, thiswidth);
						ry=getsums(img,xl-rnd(dscale),yl,xl+rnd(dscale),yl+rnd(dscale), thisheight, thiswidth)-1*getsums(img,xl-rnd(dscale),yl-rnd(dscale),xl+rnd(dscale),yl, thisheight, thiswidth);
						rrx=gaus1*(-rx*si+ry*co);
						rry=gaus1*(rx*co+ry*si);
						dix+=rrx;
						diy+=rry;
						mdx+=fabs(rrx);
						mdy+=fabs(rry);
					}
				}
				gaus2=1.0/(4.5*pi)*exp(-((cx-2.0)*(cx-2.0)+(cy-2.0)*(cy-2.0))/(4.5));
				dmap[count++]=dix;
				dmap[count++]=diy;
				dmap[count++]=mdx;
				dmap[count++]=mdy;
				dlength+=(dix*dix+diy*diy+mdx*mdx+mdy*mdy);
				j+=9;
			}
			i+=9;
		}
		dlength=sqrt(dlength);
		for(int i=0;i<64; ++i)
			dmap[i]/=dlength;

	}
	return fpoints->size()-startipts;
}

inline int Surf::rnd(float val){
	return (int)floor(val+0.5);
}

inline void Surf::getDescriptor(int &thisheight, int &thiswidth)
{
	int desy=0,desx=0,xl=0,yl=0,count=0,i=0,ix=0,j=0,jx=0,xs=0,ys=0;
	float scale=0,*desc,dix=0,diy=0,mdx=0,mdy=0,co=0,si=0,gaus1=0,gaus2=0,rx=0,ry=0,rrx=0,rry=0,dlength=0,cx=-0.5,cy=0;
	featurepoint *ipt=&fpoints->at(index);
	scale=ipt->scale;
	desx=rnd(ipt->x);
	desy=rnd(ipt->y);
	co=cos(ipt->orientation);
	si=sin(ipt->orientation);
	desc = ipt->descriptor;
	i=-8;
	while(i<12){
		j=-8;
		i=i-4;
		cx+=1.0;
		cy=-0.5;
		while(j<12){
			dix=diy=mdx=mdy=0.0;
			cy+=1.0;
			j=j-4;
			ix=i+5;
			jx=j+5;
			xs=rnd(desx+(-jx*scale*si+ix*scale*co));
			ys=rnd(desy+(jx*scale*co+ix*scale*si));
			for(int k=i;k<i+9;++k){
				for(int l=j;l<j+9;++l){
					xl=rnd(desx+(-l*scale*si+k*scale*co));
					yl=rnd(desy+(l*scale*co+k*scale*si));
					gaus1=1.0/(pi*12.5*scale*scale)*exp(-((xs-xl)*(xs-xl)+(ys-yl)*(ys-yl))/(2.0*6.25*scale*scale));
					rx=getsums(img,xl,yl-rnd(scale),xl+rnd(scale),yl+rnd(scale), thisheight, thiswidth)-1*getsums(img,xl-rnd(scale),yl-rnd(scale),xl,yl+rnd(scale), thisheight, thiswidth);
					ry=getsums(img,xl-rnd(scale),yl,xl+rnd(scale),yl+rnd(scale), thisheight, thiswidth)-1*getsums(img,xl-rnd(scale),yl-rnd(scale),xl+rnd(scale),yl, thisheight, thiswidth);
					rrx=gaus1*(-rx*si+ry*co);
					rry=gaus1*(rx*co+ry*si);
					dix+=rrx;
					diy+=rry;
					mdx+=fabs(rrx);
					mdy+=fabs(rry);
				}
			}
			gaus2=1.0/(4.5*pi)*exp(-((cx-2.0)*(cx-2.0)+(cy-2.0)*(cy-2.0))/(4.5));
			desc[count++]=dix;
			desc[count++]=diy;
			desc[count++]=mdx;
			desc[count++]=mdy;
			dlength+=(dix*dix+diy*diy+mdx*mdx+mdy*mdy);
			j+=9;
		}
		i+=9;
	}
	dlength=sqrt(dlength);
	for(int i=0;i<64;++i)
		desc[i]/=dlength;

}
inline void Surf::makeintervals(float *pyrmd, int ht, int wt, BYTE *lap, int off, int filter)
{
	float DXX,DYY,DXY;
	float *pyr=pyrmd;
	BYTE *la=lap;
	int step2=off,ys,xs,y,x,inter,fsize=filter,bdr=(filter-1)/2+1,lb=filter/3;
	float normalize =(float)(fsize*fsize);
	
	for(  y = 0, inter = 0; y < ht; ++y){
		for(x = 0; x < wt ; ++x, inter++){
			ys = y * step2;
			xs = x * step2; 
			DYY=getsums(img,xs-lb+1,ys-bdr,lb+xs,fsize+ys-bdr)-getsums(img,xs-lb+1,ys-lb/2,lb+xs,lb+ys-lb/2)*3;
			DXX=getsums(img,xs-bdr,ys-lb+1,fsize+xs-bdr,lb+ys)-getsums(img,xs-lb/2,ys-lb+1,lb+xs-lb/2,lb+ys)*3;
			DXY=getsums(img,xs+1,ys-lb,lb+xs+1,ys)+getsums(img,xs-lb,ys+1,xs,lb+ys+1)-getsums(img,xs-lb,ys-lb,xs,ys)-getsums(img,xs+1,ys+1,lb+xs+1,lb+ys+1);
			DXX/=normalize;
			DYY/=normalize;
			DXY/=normalize;
			pyr[inter] = (DXX*DYY-0.81*DXY*DXY);
			if(DXX+DYY>=0)lap[inter]=1;
			else lap[inter]=0;
		}
	}
}

int Surf::betterthanall(int r, int c, float *t, float *m, float *b, int scale, int scale2, int scale3)
{ 
	int sc2=scale2/scale;
	int sc3=scale3/scale;
	float center=m[(r*sc2)*scale2+(sc2*c)];
	for(int rr=-1;rr<=1;++rr){
		for(int cc=-1;cc<=1;++cc){
			if(t[(r+rr)*scale+(c+cc)]>=center||((rr!=0||cc!=0)&&m[(r+rr)*sc2*scale2+(c+cc)*sc2]>=center)||b[(r+rr)*sc3*scale3+(c+cc)*sc3]>=center)
				return 0;
		}
	}
	return 1;
}

