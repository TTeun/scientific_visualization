//------ VISUALIZATION CODE STARTS HERE -----------------------------------------------------------------


//--- VISUALIZATION PARAMETERS ---------------------------------------------------------------------
int   winWidth, winHeight;      //size of the graphics window, in pixels
int   color_dir = 0;            //use direction color-coding or not
float vec_scale = 600;			//scaling of hedgehogs
int   draw_smoke = 1;           //draw the smoke or not
int   draw_vecs = 0;            //draw the vector field or not
const int COLOR_BLACKWHITE=0;   //different types of color mapping: black-and-white, rainbow, banded
const int COLOR_RAINBOW=1;
const int COLOR_BANDS=2;
int   scalar_col = 0;           //method for scalar coloring
int   clamping = 1;
float clamp_param = 0.3;

void summer(float value,float* R,float* G,float* B)
{
	value = sin(value);
	*B = 1;
	*G = 1-value;
	*R = 1-value;
}


void rainbow_long(float value,float* R,float* G,float* B)
{
	value = asin(.95*value) * 2 / 3.14;
	float a=(1-value)/0.25;
	int X=floor(a);

	switch(X)
	{
		case 0: *R=.6;*G=0.1;*B=0.1;break;
		case 1: *R=.6;*G=.5;*B=0.1;break;
		case 2: *R=0;*G=.5;*B=0.1;break;
		case 3: *R=0;*G=.4;*B=.4;break;
		case 4: *R=0;*G=0.1;*B=.4;break;
	}
	*B += 0.1;
	*G += 0.1;
	*R += 0.1;
}

//set_colormap: Sets three different types of colormaps
void set_colormap(float vy, float maxvy)
{
   float R,G,B;
   if (clamping){
	    if (vy > clamp_param) vy = clamp_param;
	    vy /= clamp_param;
	} else {
		vy = vy/maxvy;
	}
	vy = 0.9*vy + .1;
   if (scalar_col==COLOR_BLACKWHITE)
       R = G = B = vy;
   else if (scalar_col==COLOR_RAINBOW){
       summer(vy,&R,&G,&B);
   }
   else if (scalar_col==COLOR_BANDS)
       {
          const int NLEVELS = 5;
          vy *= NLEVELS; vy = (int)(vy); vy/= NLEVELS;
	      rainbow_long(vy,&R,&G,&B);
	   }
   glColor3f(R,G,B);
}


//direction_to_color: Set the current color by mapping a direction vector (x,y), using
//                    the color mapping method 'method'. If method==1, map the vector direction
//                    using a rainbow colormap. If method==0, simply use the white color
void direction_to_color(float x, float y, int method)
{
	float r,g,b,f;
	if (method)
	{
	  f = atan2(y,x) / 3.1415927 + 1;
	  r = f;
	  if(r > 1) r = 2 - r;
	  g = f + .66667;
      if(g > 2) g -= 2;
	  if(g > 1) g = 2 - g;
	  b = f + 2 * .66667;
	  if(b > 2) b -= 2;
	  if(b > 1) b = 2 - b;
	}
	else
	{ r = g = b = 1; }
	glColor3f(r,g,b);
}

//visualize: This is the main visualization function
void visualize(void)
{
	int        i, j, k, idx; double px,py;
	fftw_real  wn = (fftw_real)winWidth / (fftw_real)(DIM + 1);   // Grid cell width
	fftw_real  hn = (fftw_real)winHeight / (fftw_real)(DIM + 1);  // Grid cell heigh

	if (draw_smoke)
	{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	// Find max(rho)
	fftw_real maxrho = 0.0f;
	for (k=0;k< (DIM-1) * (DIM-1);k++){
		maxrho = (rho[k] > maxrho)? rho[k] : maxrho;
	}
	
	for (j = 0; j < DIM - 1; j++)			//draw smoke
	{
		glBegin(GL_TRIANGLE_STRIP);

		i = 0;
		px = wn + (fftw_real)i * wn;
		py = hn + (fftw_real)j * hn;
		idx = (j * DIM) + i;
		glColor3f(rho[idx],rho[idx],rho[idx]);
		glVertex2f(px,py);

		for (i = 0; i < DIM - 1; i++)
		{
			px = wn + (fftw_real)i * wn;
			py = hn + (fftw_real)(j + 1) * hn;
			idx = ((j + 1) * DIM) + i;
			set_colormap(rho[idx],maxrho);
			glVertex2f(px, py);
			px = wn + (fftw_real)(i + 1) * wn;
			py = hn + (fftw_real)j * hn;
			idx = (j * DIM) + (i + 1);
			set_colormap(rho[idx],maxrho);
			glVertex2f(px, py);
		}

		px = wn + (fftw_real)(DIM - 1) * wn;
		py = hn + (fftw_real)(j + 1) * hn;
		idx = ((j + 1) * DIM) + (DIM - 1);
		set_colormap(rho[idx],maxrho);
		glVertex2f(px, py);
		glEnd();
	}
	}

	if (draw_vecs)
	{
	  glBegin(GL_LINES);				//draw velocities
	  for (i = 0; i < DIM; i++)
	    for (j = 0; j < DIM; j++)
	    {
		  idx = (j * DIM) + i;
		  direction_to_color(vx[idx],vy[idx],color_dir);
		  glVertex2f(wn + (fftw_real)i * wn, hn + (fftw_real)j * hn);
		  glVertex2f((wn + (fftw_real)i * wn) + vec_scale * vx[idx], (hn + (fftw_real)j * hn) + vec_scale * vy[idx]);
	    }
	  glEnd();
	}
	
}
