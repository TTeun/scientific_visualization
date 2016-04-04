
// Vector parameters
int   draw_vecs = 0;            //draw the vector field or not
float vec_scale = 10;			//scaling of hedgehogs
int   vec_col = 0;              //method for vector coloring
int   scaled = 1;
int   inv_vec = 0;
int   black_vec = 1;
int   disc_vector_col = 0;

float glyph_cut = 0.0;
int x_dist = 10;
int y_dist = 10;

void drawGlyph(fftw_real * valuesX, fftw_real * valuesY) {
	int i = 0, j, k = 0, idx;
	fftw_real  wn = (fftw_real)winWidth / (fftw_real)(DIM + 1);   // Grid cell width
	fftw_real  hn = (fftw_real)winHeight / (fftw_real)(DIM + 1);  // Grid cell heigh

	float values[(DIM / y_dist) * (DIM / x_dist)][3];
	float max_size = 0;
	while (i < DIM) {
		j = 0;
		while (j < DIM) {
			idx = (j * DIM) + i;
			values[k][0] = vec_scale * valuesX[idx];
			values[k][1] = vec_scale * valuesY[idx];
			values[k][2] = sqrt(values[k][0] * values[k][0] + values[k][1] * values[k][1]);
			max_size = (max_size > values[k][2]) ? max_size : values[k][2];
			j += y_dist;
			k++;
		}
		i += x_dist;
	}
	i = 0, k = 0;
	float length = 1, x_pos, y_pos;

	while (i < DIM) {
		j = 0;
		while (j < DIM) {
			if (values[k][2] >= glyph_cut) {
				length = (scaled) ? max_size : values[k][2];
				glBegin(GL_TRIANGLE_STRIP);
				if (black_vec) {
					glColor3f(0, 0, 0); // black
				} else {
					set_colormap(values[k][2], max_size, vec_col, inv_vec, disc_vector_col);
				}
				x_pos = wn + (fftw_real)i * wn;
				y_pos = hn + (fftw_real)j * hn;
				glVertex2f(x_pos - 2 * (values[k][1] / length), y_pos + 2 * (values[k][0] / length));
				glVertex2f(x_pos + 2 * (values[k][1] / length), y_pos - 2 * (values[k][0] / length));
				glVertex2f(x_pos + vec_scale * (values[k][0] / length), y_pos + vec_scale * (values[k][1] / length));
				glEnd();
			}
			j += y_dist;
			k++;
		}
		i += x_dist;
	}
}
