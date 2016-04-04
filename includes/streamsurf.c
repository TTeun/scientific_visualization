int duration = 0;
int max_duration = 50;
int start_index = 0;
int init_surf = 0;
float Xvals[2001][50];
float Yvals[2001][50];
int draw_surf = 0;
int tilter = 5.0;
int X_center = 400;
int Y_center = 400;
int radius = 100;
float angle = 3.1415;
float angle_offset = 0;

void stream_surf() {
	duration = (duration == max_duration) ? max_duration : (duration + 1);
	fftw_real  wn = (fftw_real)winWidth / (fftw_real)(DIM + 1);   // Grid cell width
	fftw_real  hn = (fftw_real)winHeight / (fftw_real)(DIM + 1);  // Grid cell heigh

	int idx;
	float x_vel;
	float y_vel;

	float x2x;
	float x1x;
	float y2y;
	float y1y;

	float YY;
	float XX;

	int steps = 2000;

	int i = 0;
	if (!init_surf) {
			float t = 0;
			for (t = 0; t < steps; t ++) {
				Xvals[i][0] = X_center + radius * cos(angle_offset + t * angle / steps);
				Yvals[i][0] = Y_center / 2 + radius * sin(angle_offset + t * angle / steps);
				if (Xvals[i][0] < 0) Xvals[i][0] = 0;
				if (Yvals[i][0] < 0) Yvals[i][0] = 0;
				if (Xvals[i][0] > winWidth) Xvals[i][0] = winWidth;
				
				i ++;
			
		} 
		init_surf = 1;
	}

	i = 0;
	int j;
	for (int dummy = 0; dummy < steps; dummy ++) {
		XX = Xvals[dummy][start_index];
		YY = Yvals[dummy][start_index];
		if (!(XX < 0 || YY < 0 || XX > winWidth || YY > winHeight)) {
			Xvals[dummy][start_index + 1] = XX;
			Yvals[dummy][start_index + 1] = YY;

			i = XX / wn;
			j = YY / hn;

			x2x = (i + 1) * wn - XX;
			x1x = wn - x2x;
			y2y = (j + 1) * hn - YY;
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

			XX += 1 * wn * x_vel;
			YY += 1 * hn * y_vel;
			if (!(XX < 0 || YY < 0 || XX > winWidth || YY > winHeight)) {
				Xvals[dummy][start_index + 1] = XX;
				Yvals[dummy][start_index + 1] = YY;
			}	
		}
	}

	int draw_index = start_index;
	for (i = 0; i < (duration - 1); i++) {
		glBegin(GL_TRIANGLE_STRIP);
		for (j = 1; j < steps ; j ++) {
			glColor3f(1, (float)i / duration, (float)i / duration);
			glVertex3f(Xvals[j][(draw_index + 1) % 50], tilter * i + Yvals[j][(draw_index + 1) % 50], -1);
			glVertex3f(Xvals[j][draw_index], tilter * (i + 1) + Yvals[j][draw_index ], -1);
		}
		draw_index --;
		draw_index = draw_index % max_duration;

		glEnd();
	}
	start_index ++;
	start_index = (start_index % max_duration);

}
