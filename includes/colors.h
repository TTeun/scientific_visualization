#ifndef _COLORS
#define _COLORS

#include "drawSmoke.h"
#include "colors.c"

float scaler(float x) ;

float absolute(float a) ;

void summer(float value, float* R, float* G, float* B);

void psychedelic1(float value, float* R, float* G, float* B);

void rainbow_long(float value, float* R, float* G, float* B);

void hue(float H);

void set_colormap(float vy, float maxvy, int method, int inv, int disc_col);

#endif
