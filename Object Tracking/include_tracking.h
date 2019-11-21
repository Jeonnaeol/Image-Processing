struct track{
	int x;
	int y;
	int x1;
	int x2;
	int y1;
	int y2;
	int level;
};

void Init_track(track trk);

//세중 
int include_Detect_this_Color(BYTE** R, BYTE** G, BYTE** B, int color, BYTE** output);

//창현 도형
int include_shape_detect(BYTE** input, BYTE** output);


//세중 roi
void include_erode(BYTE** input, BYTE** output);
void include_dilate(BYTE** input, BYTE** output);
void include_Detect_Contour(BYTE** input, BYTE** output) ;
void include_Labeling(BYTE** input, BYTE** output) ;
//동균 추적
void include_integral_roi(BYTE** source, int int_img[240][320], int x1, int x2, int y1, int y2);
int include_roi_size(BYTE** in,int size, int *y, int *x, int *maxval, int x1, int x2, int y1, int y2);
track include_tracking_roi(BYTE** red, track ori, int widen,BYTE** out, int *max, BYTE** rimg, BYTE** gimg, BYTE** bimg);
void include_drawing_circle(int x1, int y1, int r, BYTE** outputCam);
void include_drawing_circle(int x1, int y1, int r, BYTE** out, BYTE** out2);
void include_detect_color(BYTE** red,BYTE** green,BYTE** blue, BYTE** tmp, BYTE** out, int whichcolor);