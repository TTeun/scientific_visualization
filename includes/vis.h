
//--- VISUALIZATION PARAMETERS ---------------------------------------------------------------------
int   winWidth, winHeight;      //size of the graphics window, in pixels


#include "drawSmoke.h"
#include "colors.h"
#include "seedFlow.h"
#include "drawGlyph.c"
#include "drawGrad.c"
#include "streamsurf.c"
int vec_grad = 1;

// [0 2 3 ... DIM-1]

//visualize: This is the main visualization function

void visualize(void)
{
	if (draw_smoke)
	{
		smoke();
	} else { // white background
		glBegin(GL_QUAD_STRIP);
		glColor3d(1, 1, 1);
		glVertex2f(winWidth, 0);
		glVertex2f(winWidth, winHeight);
		glVertex2f(0, 0);
		glVertex2f(0, winHeight);
		glEnd();
	}

	if (draw_flow) {
		seed();
	}

	if (draw_vecs) {
		if (vec_grad) {
			drawGlyph(vx, vy);
		} else {					
			drawGrad(rho);
		}
	}


	if (draw_smoke && !(color_dir)) {
		legend();
	}
	if (draw_surf) {
		stream_surf();
	}
}


