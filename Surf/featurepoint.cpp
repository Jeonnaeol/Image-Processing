#include <vector>
#include "featurepoint.h"

#if 1

#else

void findmatchfeatures(features &fpoints, features &refpoints, mfeatures &matches)
{
	float diffrences, small1, small2;
	float sum=0, tmp;
	featurepoint *match;

	matches.clear();
	int r=0,b=0, yel=0;
	/*
	for(int i = 0; i < fpoints.size(); i++) {
		if(fpoints[i].color==1)
			r+=1;
		else if(fpoints[i].color==2)
			b+=1;
		else
			yel+=1;
	}

	printf("red=%d, blue=%d, yel=%d \n", r,b,yel);
	*/
	for(int i = 0; i < fpoints.size(); i++) 
	{
		small1 = small2 = FLT_MAX;
		for(int j = 0; j < refpoints.size(); j++) 
		{	
			sum=0;
			for(int k=0; k < 64; k+=1){
				tmp=fpoints[i].descriptor[k] - refpoints[j].descriptor[k];
				sum += tmp*tmp;
			}
			//diffrences= sqrt(sum);	//루트 씌우면 연산 느려지니까
			diffrences= sum; //루트 구지안씌워도 될듯
			if(diffrences<small1){
				small2 = small1;
				small1 = diffrences;
				match = &refpoints[j];
			}
			else if(diffrences<small2){
				small2 = diffrences;
			}
		}
		if(small1/small2 < 0.4225)//0.4225)  //안씌우면 이거로
		{ 
			//printf("%f\n", small1);
			matches.push_back(std::make_pair(fpoints[i], *match));
		}
	}

}
#endif
