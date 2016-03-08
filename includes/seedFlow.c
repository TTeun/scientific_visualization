
// Flow object parameters
int   draw_flow = 0;            //draw the flow objects or not
int   flow_col = 0;              //method for vector coloring
int   inv_flow = 0;
int   black_flow = 1;
int   flow_col_scale = 500;
int   seed_spacing = 30;
int   path_length = 100;

void seed(void) {
	fftw_real  wn = (fftw_real)winWidth / (fftw_real)(DIM + 1);   // Grid cell width
	fftw_real  hn = (fftw_real)winHeight / (fftw_real)(DIM + 1);  // Grid cell heigh

	float X, XX;
	float Y, YY;

	int idx;
	float x_vel;
	float y_vel;
	float size;

	float x2x;
	float x1x;
	float y2y;
	float y1y;

	int i, j, k;
	for (XX = 10; XX < winWidth; XX += seed_spacing) {
		for (YY = 10; YY < winHeight; YY += seed_spacing) {
			X = XX;
			Y = YY;
			glBegin(GL_LINE_STRIP);
			glColor3f(.2, .2, .2);
			glVertex2f(X, Y);
			k = 0;

			for (k = 0; (k < path_length) && !(X < 0 || Y < 0 || X > winWidth || Y > winHeight); k++) {
				i = X / wn;
				j = Y / hn;

				x2x = (i + 1) * wn - X;
				x1x = wn - x2x;
				y2y = (j + 1) * hn - Y;
				y1y = hn - y2y;

				idx = (j * DIM) + i;
				x_vel =  x2x * y2y * vx[idx];
				x_vel += x2x * y1y * vx[idx + DIM];
				x_vel += x1x * y1y * vx[idx + DIM + 1];
				x_vel += x1x * y2y * vx[idx + 1];

				y_vel = x2x * y2y * vy[idx];
				y_vel += x2x * y1y * vy[idx + DIM];
				y_vel += x1x * y1y * vy[idx + DIM + 1];
				y_vel += x1x * y2y * vy[idx + 1];

				size = (y_vel * y_vel + x_vel * x_vel);

				if (size > 0) {
					if (!black_flow) {
						set_colormap(flow_col_scale * size, 1, flow_col, inv_flow, 0);
					}
					// glColor3f(.1, .1, .1);
					size = sqrt(size);
					y_vel /= size;
					x_vel /= size;

					X += 0.4 * wn * x_vel;
					Y += 0.4 * hn * y_vel;
					glVertex2f(X, Y);
				}
			}
			glEnd();
			glBegin(GL_TRIANGLE_STRIP);
			glVertex2f(X + seed_spacing * x_vel / 6, Y + seed_spacing * y_vel / 6);
			glVertex2f(X - seed_spacing * y_vel / 16, Y + seed_spacing * x_vel / 16);
			glVertex2f(X + seed_spacing * y_vel / 16, Y - seed_spacing * x_vel / 16);
			glEnd();
		}
	}

}