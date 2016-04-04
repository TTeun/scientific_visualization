
// Smoke parameters
int   color_dir = 0;            //use direction color-coding or not
int   draw_smoke = 1;           //draw the smoke or not
int   scalar_col = 0;           //method for scalar coloring
int   disc_scalar_col = 0;
int   col_scaler = 0;
int   inv_scalar = 0;
int   draw_rho = 1;


#include "colors.h"

void smoke(void) {
	int i, j, k, idx;
	double px, py;
	fftw_real  wn = (fftw_real)winWidth / (fftw_real)(DIM + 1);   // Grid cell width
	fftw_real  hn = (fftw_real)winHeight / (fftw_real)(DIM + 1);  // Grid cell heigh

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	fftw_real values[(DIM- 1) * (DIM - 1)];
	if (draw_rho){
		for (i = 0; i < (DIM- 1) * (DIM - 1); i++){
			values[i] = rho[i];
		}
	} else {
		for (i = 0; i < (DIM- 1) * (DIM - 1); i++){
			values[i] = vx[i]*vx[i] + vy[i]+vy[i];
			values[i] = (values[i] == 0) ? 0 : sqrt(values[i]);
		}		
	}
		
	// Find max(rho)
	fftw_real max = 0.0f;
	for (k = 0; k < (DIM - 1) * (DIM - 1); k++) {
		max = (values[k] > max) ? values[k] : max;
	}
	
	for (j = 0; j < DIM - 1; j += 1)			//draw smoke
	{
		glBegin(GL_QUAD_STRIP);
		i = 0;
		px = wn + (fftw_real)i * wn;
		py = hn + (fftw_real)j * hn;
		idx = (j * DIM) + i;
		if (!color_dir) {
			set_colormap(values[idx], max, scalar_col, inv_scalar, disc_scalar_col);
		} else {
			hue( ( atan2(vy[idx] , vx[idx]) + 3.14) / 6.28 );
		}
		glVertex2f(px, py);

		for (i = 0; i < DIM - 1; i++)
		{
			px = wn + (fftw_real)i * wn;
			py = hn + (fftw_real)(j + 1) * hn;
			idx = ((j + 1) * DIM) + i;
			if (!color_dir) {
				set_colormap(values[idx], max, scalar_col, inv_scalar, disc_scalar_col);
			} else {
				hue( ( atan2(vy[idx] , vx[idx]) + 3.14) / 6.28 );
			}
			glVertex2f(px, py);
			px = wn + (fftw_real)(i + 1) * wn;
			py = hn + (fftw_real)j * hn;
			idx = (j * DIM) + (i + 1);
			if (!color_dir) {
				set_colormap( values[idx] , max, scalar_col, inv_scalar, disc_scalar_col);
			} else {
				hue( ( atan2(vy[idx] , vx[idx]) + 3.14) / 6.28 );
			}
			glVertex2f(px, py);
		}


		px = wn + (fftw_real)(DIM - 1) * wn;
		py = hn + (fftw_real)(j + 1) * hn;
		idx = ((j + 1) * DIM) + (DIM - 1);
		if (!color_dir) {
			set_colormap(values[idx], max, scalar_col, inv_scalar, disc_scalar_col);
		} else {
			hue( ( atan2(vy[idx] , vx[idx]) + 3.14) / 6.28 );
		}
		glVertex2f(px, py);
		glEnd();
	}
}


// Draw the color legend
void legend(void) {
	int i, j;
	double px, py;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for (j = 0; j < winHeight - 1; j++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		i = 0;
		px = i;
		py = j;
		glVertex2f(px, py);
		for (i = 0; i < 20 - 1; i++)
		{
			px = i;
			py = (j + 1);
			set_colormap(j * clamp_param / winHeight, clamp_param, scalar_col, inv_scalar, disc_scalar_col);
			glVertex2f(px, py);
			px = (i + 1);
			py = j;
			set_colormap(j * clamp_param / winHeight, clamp_param, scalar_col, inv_scalar, disc_scalar_col);
			glVertex2f(px, py);
		}
		glVertex2f(px, py);
		glEnd();
	}
}
