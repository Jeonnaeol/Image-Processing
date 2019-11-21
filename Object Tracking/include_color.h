#include "stdafx.h"
#include <vector>

void draw_yellow_dif(BYTE** red,BYTE** green,BYTE** blue, BYTE** out);
void draw_blue_dif(BYTE** red,BYTE** green,BYTE** blue, BYTE** out);
void draw_red_dif(BYTE** red,BYTE** green,BYTE** blue, BYTE** out);
void draw_green_dif(BYTE** red,BYTE** green,BYTE** blue, BYTE** out);

void include_integral(BYTE** source, int int_img[240][320]);

void include_max_roi(BYTE** in,int size, int *y, int *x, int *maxval);

void clear_img(BYTE** tmp);

void include_draw_color(BYTE** dif, BYTE** out, int* maxv, int variable);

void include_detect_color(BYTE** red, BYTE** green, BYTE** blue, BYTE** tmp, BYTE** out, int whichcolor);
