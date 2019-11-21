#ifndef IPOINT_H
#define IPOINT_H

#include <vector>
#include <math.h>

//struct featurepoint; 

#if 1

#else
struct featurepoint {
  float x, y;
  float scale;
  float orientation;
  int laplacian;
  float descriptor[64];
  int color;
};


#endif

#endif