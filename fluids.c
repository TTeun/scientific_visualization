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
	if (control == 0 || control == 1) {
		init_surf = 0;
		duration = 0;
		start_index  = 0;
	}
	if (control == 0){
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
	glutInit (&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	int main_window = glutCreateWindow("Real-time smoke simulation and visualization");

	GLUI *glui = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_RIGHT);

	glui->add_button("Restart", 0, control_cb);
	new GLUI_Checkbox( glui, "Pause", &frozen );
	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);
	glui->add_button("Reset Stream", 1, control_cb);

	// Simulation controls
	GLUI_Rollout *sim_panel = new GLUI_Rollout(glui, "Simulation", false );
	(new GLUI_Spinner( sim_panel, "Viscosity:", &visc ))->set_float_limits( 0.00002, 0.005);
	(new GLUI_Spinner( sim_panel, "Time step dt:", &dt ))->set_float_limits( -0.8, 5);

	// Stream surface controls
	new GLUI_Checkbox( glui, "surf", &draw_surf );
	GLUI_Rollout *surf_panel = new GLUI_Rollout(glui, "Stream Surface", false );
	
	(new GLUI_Spinner( surf_panel, "Height:", &tilter ))->set_int_limits( -15, 15);
	(new GLUI_Spinner( surf_panel, "X center:", &X_center ))->set_int_limits( 0, 2000);
	(new GLUI_Spinner( surf_panel, "Y center:", &Y_center ))->set_int_limits( 0, 2000);
	(new GLUI_Spinner( surf_panel, "Radius:", &radius ))->set_int_limits( 1, 800);
	(new GLUI_Spinner( surf_panel, "Angle:", &angle ))->set_float_limits( .5, 6.283);
	(new GLUI_Spinner( surf_panel, "Angle offset:", &angle_offset ))->set_float_limits( 0, 6.283);


	// Vector controls
	new GLUI_Checkbox( glui, "Draw vector field", &draw_vecs );
	GLUI_Rollout *vec_panel = new GLUI_Rollout(glui, "Vectors options", false );

	GLUI_RadioGroup *group1 = glui->add_radiogroup_to_panel(vec_panel, &vec_grad, 1, control_cb);
	glui->add_radiobutton_to_group( group1, "Gradient" );
	glui->add_radiobutton_to_group( group1, "Velocity" );

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

	// Flow controls
	new GLUI_Checkbox( glui, "Draw flow ojects", &draw_flow );
	GLUI_Rollout *flow_panel = new GLUI_Rollout(glui, "Flow options", false );
	
	new GLUI_Checkbox( flow_panel, "Invert color map", &inv_flow );
	(new GLUI_Spinner( flow_panel, "Seed spacing:", &seed_spacing ))->set_int_limits( 5, 150);
	GLUI_Rollout *flow_col_panel = new GLUI_Rollout(flow_panel, "custom color", false );
	new GLUI_Checkbox( flow_col_panel, "Custom color", &black_flow );
	(new GLUI_Spinner( flow_col_panel, "custom R:", &cust_R ))->set_float_limits( 0, 1);
	(new GLUI_Spinner( flow_col_panel, "custom G:", &cust_G ))->set_float_limits( 0, 1);
	(new GLUI_Spinner( flow_col_panel, "custom B:", &cust_B ))->set_float_limits( 0, 1);
	(new GLUI_Spinner( flow_panel, "Path length:", &path_length ))->set_int_limits( 20, 350);
	GLUI_Listbox *flow_colors = glui->add_listbox_to_panel (flow_panel, "Color", &flow_col);
	flow_colors->add_item (0, "Rainbow");
	flow_colors->add_item (1, "BW");
	flow_colors->add_item (2, "Blue");
	flow_colors->add_item (3, "Psychedelic");
	flow_colors->add_item (4, "hue");


	// Smoke controls
	new GLUI_Checkbox( glui, "Draw smoke", &draw_smoke );
	GLUI_Rollout *smoke_panel = new GLUI_Rollout(glui, "Smoke options", false );
	new GLUI_Checkbox( smoke_panel, "Direction hue", &color_dir );
	new GLUI_Checkbox( smoke_panel, "Scaling/Clamping", &clamping );
	new GLUI_Checkbox( smoke_panel, "Invert color map", &inv_scalar );
	new GLUI_Checkbox( smoke_panel, "Discrete color map", &disc_scalar_col );
	// new GLUI_Checkbox( smoke_panel, "Cubic color scaler", &col_scaler );
	(new GLUI_Scrollbar( smoke_panel, "bar", GLUI_SCROLL_HORIZONTAL, &clamp_param))->set_float_limits(0.1, 2.0);
	(new GLUI_Spinner( smoke_panel, "Color levels:", &NLEVELS ))->set_int_limits( 2, 255 );
	GLUI_Listbox *smoke_colors = glui->add_listbox_to_panel (smoke_panel, "Color", &scalar_col);
	smoke_colors->add_item (0, "Rainbow");
	smoke_colors->add_item (1, "BW");
	smoke_colors->add_item (2, "Blue");
	smoke_colors->add_item (3, "Psychedelic");
	smoke_colors->add_item (4, "hue");

	glutDisplayFunc(display);
	GLUI_Master.set_glutReshapeFunc( reshape );
	/* We register the idle callback with GLUI, *not* with GLUT */
	glutIdleFunc(do_one_simulation_step);
	GLUI_Master.set_glutIdleFunc( do_one_simulation_step );

	GLUI_Master.set_glutKeyboardFunc( keyboard );
	glutMotionFunc(drag);

	init_simulation(DIM);	//initialize the simulation data structures
	glutMainLoop();			//calls do_one_simulation_step, keyboard, display, drag, reshape
	return 0;
}
