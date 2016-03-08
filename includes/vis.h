
//--- VISUALIZATION PARAMETERS ---------------------------------------------------------------------
int   winWidth, winHeight;      //size of the graphics window, in pixels


// Vector parameters
int   draw_vecs = 1;            //draw the vector field or not
float vec_scale = 10;			//scaling of hedgehogs
int   vec_col = 0;              //method for vector coloring
int   scaled = 1;
int   inv_vec = 0;
int   black_vec = 1;
int   disc_vector_col = 0;

// Flow object parameters
int   draw_flow = 0;            //draw the flow objects or not
int   flow_col = 0;              //method for vector coloring
int   inv_scalar = 0;
int   inv_flow = 0;
int   black_flow = 1;
int   flow_col_scale = 500;
int   seed_spacing = 30;
int   path_length = 100;

#include "drawSmoke.c"
#include "colors.h"
#include "seedFlow.c"
#include "drawGlyph.c"
#include "drawGrad.c"

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
		drawGlyph(vx, vy);
	}

	if (draw_smoke){
		legend();
	}

}


