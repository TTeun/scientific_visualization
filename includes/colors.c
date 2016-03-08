#ifndef _COLORS
#define _COLORS

const int COLOR_BLACKWHITE = 1; 
const int COLOR_RAINBOW = 2;
const int COLOR_BANDS = 0;
const int COLOR_PSYCH1 = 3;
const int COLOR_HUE = 4;
int   clamping = 1;
float clamp_param = 0.1;
int   NLEVELS = 5;

float scaler(float x) {
	float y = (x >= 0) ? x : (-x);
	return (x > 0) ? y : (-y);
}

float absolute(float a) {
	return (a > 0) ? a : (-a);
}

void summer(float value, float* R, float* G, float* B)
{
	*B = 1;
	*G = 1 - value;
	*R = 1 - value;

	if ((0.3 < value) && (0.35 > value)) {
		*G -= 40 * (absolute(value - 0.325) - 0.025);
		*R -= 40 * (absolute(value - 0.325) - 0.025);
		*B -= 1 - 10 * (absolute(value - 0.325) - 0.025);
	}

	if ((0.55 < value) && (0.6 > value)) {
		*G -= 20 * (absolute(value - 0.575) - 0.025);
	}
}

void psychedelic1(float value, float* R, float* G, float* B)
{

	*G = *R = *B = 1;
	if (value < 0.25)
	{
		*B = 1 - 4 * value;
	}
	else if (value < 0.5)
	{
		*G = 1 - 4 * (value - 0.25);
		*B = -1 + 4 * value;
	}
	else if (value < 0.75)
	{
		*R = 1 - 4 * (value - 0.5);
		*G = -2 + 4 * value;
	}
	else if (value <= 1)
	{
		*B -= 0.3 * (value - 0.75);
		*R = 3.6 * (value - 0.75);
	}
}

void rainbow_long(float value, float* R, float* G, float* B)
{
	*G = *R = *B = 0.0;
	if (value < 0.25)
	{
		*B = 1;
		*R = 1 - 4 * value;
		*G = 1 - 4 * value;
	}
	else if (value < 0.5)
	{
		*G = 4 * (value - 0.25);
		*B = 2 - 4 * value;
	}
	else if (value < 0.75)
	{
		*R = 4 * (value - 0.5);
		*G = 3 - 4 * value;
	}
	else if (value <= 1)
	{
		*R = 4 - 4 * (value - 0.75);
	}
}

void hue(float H)
{
	H = H;
	float R, G, B;
	R = absolute(H * 6 - 3) - 1;
	G = 2 - absolute(H * 6 - 2);
	B = 2 - absolute(H * 6 - 4);
	glColor3f(.5 * R, .5 * G, .5 * B);
}


void set_colormap(float vy, float maxvy, int method, int inv, int disc_col)
{
	float R, G, B;
	if (clamping) {
		if (vy > clamp_param) vy = clamp_param;
		vy /= clamp_param;
	} else {
		vy = vy / maxvy;
	}

	if (col_scaler) {
		vy = scaler(vy);
	}

	vy = (inv) ? (1 - vy) : vy;
	if (disc_col) {
		vy *= NLEVELS; vy = (int)(vy); vy /= NLEVELS;
	}

	if (method == COLOR_BLACKWHITE)
		R = G = B = vy;
	if (method == COLOR_RAINBOW)
		summer(vy, &R, &G, &B);
	if (method == COLOR_PSYCH1)
		psychedelic1(vy, &R, &G, &B);
	if (method == COLOR_BANDS)
		rainbow_long(vy, &R, &G, &B);

	glColor3f(R, G, B);

	if (method == COLOR_HUE)
		hue(0.1 + 0.90 * vy);

}

#endif
