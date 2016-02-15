//------ INTERACTION CODE STARTS HERE -----------------------------------------------------------------

void drawColorLegend(){
	int i, j;
	double px, py;
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glutSetWindow(1);
	for (j = 0; j < winHeight - 1; j++)			
	{
		glBegin(GL_TRIANGLE_STRIP);

		i = 0;
		px = i;
		py = j;
		glVertex2f(px,py);

		for (i = 0; i < 20 - 1; i++)
		{
			px = i;
			py = (j + 1);
			set_colormap(j * clamp_param/winHeight,clamp_param);
			glVertex2f(px, py);
			px = (i + 1);
			py = j;
			set_colormap(j * clamp_param/winHeight,clamp_param);
			glVertex2f(px, py);
		}
		glVertex2f(px, py);
		glEnd();
	}	
}


//display: Handle window redrawing events. Simply delegates to visualize().
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	visualize();
	drawColorLegend();
	glutPostRedisplay();
	glFlush();
	glutSwapBuffers();
}

//reshape: Handle window resizing (reshaping) events
void reshape(int w, int h)
{
 	glViewport(0.0f, 0.0f, (GLfloat)w, (GLfloat)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
	winWidth = w; winHeight = h;
}

//keyboard: Handle key presses
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	  case 't': if (dt > -.8) dt -= 0.1; break;
	  case 'T': dt += 0.1; break;
	  case 'c': color_dir = 1 - color_dir;  break;
	  case 'i': inv_gray = 1 - inv_gray;  break;
	  case 'S': vec_scale *= 1.2; break;
	  case 's': vec_scale *= 0.8; break;
	  case 'w': scaled = 1 - scaled; break;
	  case 'V': visc *= 5; break;
	  case 'v': visc *= 0.2; break;
	  case 'x': draw_smoke = 1 - draw_smoke;
		    if (draw_smoke==0) draw_vecs = 1; break;
	  case 'y': draw_vecs = 1 - draw_vecs;
		    if (draw_vecs==0) draw_smoke = 1; break;
	  case 'm': scalar_col++; if (scalar_col>3) scalar_col=COLOR_BLACKWHITE; break;
	  case 'n': NLEVELS ++; break;
	  case 'b': if (NLEVELS > 0) NLEVELS --; break;
	  case 'a': frozen = 1-frozen; break;
	  case 'l': clamp_param += .1; break;
	  case 'k': clamp_param -= .1; break;
	  case 'o': clamping = 1 - clamping; break;
	  case 'q': exit(0);
	}
}

// drag: When the user drags with the mouse, add a force that corresponds to the direction of the mouse
//       cursor movement. Also inject some new matter into the field at the mouse location.
void drag(int mx, int my)
{
	int xi,yi,X,Y; double  dx, dy, len;
	static int lmx=0,lmy=0;				//remembers last mouse location

	// Compute the array index that corresponds to the cursor location
	xi = (int)clamp((double)(DIM + 1) * ((double)mx / (double)winWidth));
	yi = (int)clamp((double)(DIM + 1) * ((double)(winHeight - my) / (double)winHeight));

	X = xi; Y = yi;

	if (X > (DIM - 1))  X = DIM - 1; if (Y > (DIM - 1))  Y = DIM - 1;
	if (X < 0) X = 0; if (Y < 0) Y = 0;

	// Add force at the cursor location
	my = winHeight - my;
	dx = mx - lmx; dy = my - lmy;
	len = sqrt(dx * dx + dy * dy);
	if (len != 0.0) {  dx *= 0.1 / len; dy *= 0.1 / len; }
	fx[Y * DIM + X] += dx;
	fy[Y * DIM + X] += dy;
	rho[Y * DIM + X] = 10.0f;
	lmx = mx; lmy = my;
}

