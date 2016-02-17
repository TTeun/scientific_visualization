
// Usage: Drag with the mouse to add smoke to the fluid. This will also move a "rotor" that disturbs
//        the velocity field at the mouse location. Press the indicated keys to change options
//--------------------------------------------------------------------------------------------------

#include <rfftw.h>              //the numerical simulation FFTW library
#include <stdio.h>              //for printing the help text
#include <math.h>               //for various math functions
#include <GL/glut.h>            //the GLUT graphics library
#include <GL/glui.h>
#include "sim.h"
#include "vis.h"
#include "inter.h"


//main: The main program
int main(int argc, char **argv)
{
	printf("Fluid Flow Simulation and Visualization\n");
	printf("=======================================\n");
	printf("Click and drag the mouse to steer the flow!\n");
	printf("T/t:   increase/decrease simulation timestep\n");
	printf("S/s:   increase/decrease hedgehog scaling\n");
	printf("c:     toggle direction coloring on/off\n");
	printf("i:     invert gray scale\n");
	printf("w:     toggle scaling to vector size on/off\n");
	printf("V/v:   increase decrease fluid viscosity\n");
	printf("x:     toggle drawing matter on/off\n");
	printf("y:     toggle drawing hedgehogs on/off\n");
	printf("m:     toggle thru scalar coloring\n");
	printf("k/l:   toggle thru scalar coloring\n");
	printf("b/n:   decrease/increase number of colors\n");
	printf("k/l:   decrease/increase clamp maximum\n");
	printf("o:     swap between clamping and scaling\n");
	printf("a:     toggle the animation on/off\n");
	printf("q:     quit\n\n");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("Real-time smoke simulation and visualization");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(do_one_simulation_step);

  GLUI *glui = GLUI_Master.create_glui( "GLUI" );
  new GLUI_Checkbox( glui, "Draw vector field", &draw_vecs );
  new GLUI_Checkbox( glui, "Draw draw", &draw_smoke );
  (new GLUI_Spinner( glui, "Color levels:", &NLEVELS ))
    ->set_int_limits( 3, 60 ); 
   
  glui->set_main_gfx_window( 1 );

  /* We register the idle callback with GLUI, *not* with GLUT */
  GLUI_Master.set_glutIdleFunc( do_one_simulation_step ); 

	glutKeyboardFunc(keyboard);
	glutMotionFunc(drag);

	
	init_simulation(DIM);	//initialize the simulation data structures
	glutMainLoop();			//calls do_one_simulation_step, keyboard, display, drag, reshape
	return 0;
}
