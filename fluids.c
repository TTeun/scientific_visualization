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

void control_cb( int control )
{
	if (control == 0) {
		int i, j, idx;
		for (i = 0; i < DIM; i++) {
			for (j = 0; j < DIM; j++) {
				idx = (j * DIM) + i;
				rho[idx] = 0;
				vx[idx] = 0;
				vy[idx] = 0;
				vx0[idx] = 0;
				vy0[idx] = 0;
				fx[idx] = 0;
				fy[idx] = 0;
			}
		}
	}
}



//main: The main program
int main(int argc, char **argv)
{
// 	printf("Fluid Flow Simulation and Visualization\n");
// 	printf("=======================================\n");
// 	printf("Click and drag the mouse to steer the flow!\n");
// 	printf("T/t:   increase/decrease simulation timestep\n");
// 	printf("S/s:   increase/decrease hedgehog scaling\n");
// 	printf("c:     toggle direction coloring on/off\n");
// 	printf("i:     invert gray scale\n");
// 	printf("w:     toggle scaling to vector size on/off\n");
// 	printf("V/v:   increase decrease fluid viscosity\n");
// 	printf("x:     toggle drawing matter on/off\n");
// 	printf("y:     toggle drawing hedgehogs on/off\n");
// 	printf("m:     toggle thru scalar coloring\n");
// 	printf("k/l:   toggle thru scalar coloring\n");
// 	printf("b/n:   decrease/increase number of colors\n");
// 	printf("k/l:   decrease/increase clamp maximum\n");
// 	printf("o:     swap between clamping and scaling\n");
// 	printf("a:     toggle the animation on/off\n");
// 	printf("q:     quit\n\n");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Real-time smoke simulation and visualization");

	GLUI *glui = GLUI_Master.create_glui( "GLUI" );
	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);
	glui->add_button("Restart", 0, control_cb);

	GLUI_Panel *vec = new GLUI_Rollout(glui, "Vectors" );
	new GLUI_Checkbox( vec, "Draw vector field", &draw_vecs );
	GLUI_Rollout *vec_panel = new GLUI_Rollout(vec, "Vectors options", true );
	new GLUI_Checkbox( vec_panel, "Scaled to size", &scaled );
	new GLUI_Checkbox( vec_panel, "Invert color map", &inv_vec );
	new GLUI_Checkbox( vec_panel, "Discrete colormap", &disc_vector_col );
	new GLUI_Checkbox( vec_panel, "Black/Color", &black_vec );
	(new GLUI_Scrollbar( vec_panel, "Size:", GLUI_SCROLL_HORIZONTAL, &vec_scale ))->set_int_limits(1, 100);
	(new GLUI_Spinner( vec_panel, "glyph cut-off:", &glyph_cut ))->set_float_limits( 0.0, 1.0);
	GLUI_Listbox *vec_colors = glui->add_listbox_to_panel (vec_panel, "Color", &vec_col);
	vec_colors->add_item (0, "Rainbow");
	vec_colors->add_item (1, "BW");
	vec_colors->add_item (2, "Blue");
	vec_colors->add_item (3, "Psychedelic");
	vec_colors->add_item (4, "hue");

	GLUI_Panel *flow = new GLUI_Rollout(glui, "Flow" );
	new GLUI_Checkbox( flow, "Draw flow ojects", &draw_flow );
	GLUI_Rollout *flow_panel = new GLUI_Rollout(flow, "Flow options", true );
	new GLUI_Checkbox( flow_panel, "Invert color map", &inv_flow );
	new GLUI_Checkbox( flow_panel, "Black/Color", &black_flow );
	(new GLUI_Spinner( flow_panel, "Seed spacing:", &seed_spacing ))->set_int_limits( 5, 150);
	(new GLUI_Spinner( flow_panel, "Path length:", &path_length ))->set_int_limits( 20, 350);
	GLUI_Listbox *flow_colors = glui->add_listbox_to_panel (flow_panel, "Color", &flow_col);
	flow_colors->add_item (0, "Rainbow");
	flow_colors->add_item (1, "BW");
	flow_colors->add_item (2, "Blue");
	flow_colors->add_item (3, "Psychedelic");
	flow_colors->add_item (4, "hue");

	glui->add_column(true);

	GLUI_Panel *smoke = new GLUI_Rollout(glui, "Smoke" );
	new GLUI_Checkbox( smoke, "Draw smoke", &draw_smoke );
	new GLUI_Checkbox( smoke, "Direction hue", &color_dir );
	GLUI_Rollout *smoke_panel = new GLUI_Rollout(smoke, "Smoke options", true );
	new GLUI_Checkbox( smoke_panel, "Scaling/Clamping", &clamping );
	new GLUI_Checkbox( smoke_panel, "Invert color map", &inv_scalar );
	new GLUI_Checkbox( smoke_panel, "Discrete color map", &disc_scalar_col );
	// new GLUI_Checkbox( smoke_panel, "Cubic color scaler", &col_scaler );
	(new GLUI_Scrollbar( smoke_panel, "bar", GLUI_SCROLL_HORIZONTAL, &clamp_param))->set_float_limits(0.1, 2.0);
	(new GLUI_Spinner( smoke_panel, "Color levels:", &NLEVELS ))->set_int_limits( 1, 60 );
	GLUI_Listbox *smoke_colors = glui->add_listbox_to_panel (smoke_panel, "Color", &scalar_col);
	smoke_colors->add_item (0, "Rainbow");
	smoke_colors->add_item (1, "BW");
	smoke_colors->add_item (2, "Blue");
	smoke_colors->add_item (3, "Psychedelic");
	smoke_colors->add_item (4, "hue");

	// GLUI_RadioGroup *group1 = glui->add_radiogroup_to_panel(smoke_panel, &scalar_col,1,control_cb);
	// glui->add_radiobutton_to_group( group1, "Rainbow" );
	// glui->add_radiobutton_to_group( group1, "BW" );
	// glui->add_radiobutton_to_group( group1, "Blue" );
	// glui->add_radiobutton_to_group( group1, "Psychedelic" );


	GLUI_Rollout *sim_panel = new GLUI_Rollout(glui, "Simulation", true );
	(new GLUI_Spinner( sim_panel, "Viscosity:", &visc ))->set_float_limits( 0.00002, 0.005);
	(new GLUI_Spinner( sim_panel, "Time step dt:", &dt ))->set_float_limits( -0.8, 5);
	new GLUI_Checkbox( sim_panel, "Pause", &frozen );


	glui->set_main_gfx_window( 1 );

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(do_one_simulation_step);
	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc( do_one_simulation_step );

	glutKeyboardFunc(keyboard);
	glutMotionFunc(drag);


	init_simulation(DIM);	//initialize the simulation data structures
	glutMainLoop();			//calls do_one_simulation_step, keyboard, display, drag, reshape
	return 0;
}
