int draw_grad = 1;

void drawGrad(fftw_real * values) {
	int i = 0, j, idx;
	fftw_real  wn = (fftw_real)winWidth / (fftw_real)(DIM + 1);   // Grid cell width
	fftw_real  hn = (fftw_real)winHeight / (fftw_real)(DIM + 1);  // Grid cell heigh

	while (i < DIM) {
		j = 0;
		while (j < DIM) {
			idx = (j * DIM) + i;
			float vX = values[idx + 1] - values[idx];
			float vY = values[idx + DIM] - values[idx];
			float size = (sqrt(vX * vX + vY * vY));
			if ( (size != 0) && (!scaled) ) {
				vX /= 100 * size;
				vY /= 100 * size;
				size = 0.01;
			}
			glBegin(GL_TRIANGLE_STRIP);
			if (black_vec) {
				glColor3f(0, 0, 0); // black
			} else {
				set_colormap(values[j], size, vec_col, inv_vec, disc_vector_col);
			}
			glVertex2f(wn + (fftw_real)i * wn - 1.5 * vY / size, hn + (fftw_real)j * hn + 1.5 * vX / size);
			glVertex2f(wn + (fftw_real)i * wn + 1.5 * vY / size, hn + (fftw_real)j * hn - 1.5 * vX / size);
			glVertex2f((wn + (fftw_real)i * wn) + vec_scale * vX / size, (hn + (fftw_real)j * hn) +  vec_scale * vY / size);
			glEnd();
			j += 10;
		}
		i += 10;
	}
}
