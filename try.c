#include <stdio.h>              //for printing the help text
#include <math.h>               //for various math functions
#include <GL/glut.h>            //the GLUT graphics library
	


//reshape: Handle window resizing (reshaping) events
void reshape(int w, int h)
{
 	glViewport(0.0f, 0.0f, (GLfloat)w, (GLfloat)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}

void draw(){
	int i, j;
	double px, py;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glutSetWindow(1);
	for (j = 0; j < 500 - 1; j++)			
	{
		glBegin(GL_TRIANGLE_STRIP);

		i = 0;
		px = i;
		py = j;
		glColor3f(.1,0.5,225);
		glVertex2f(px,py);

		for (i = 0; i < 500 - 1; i++)
		{
			px = i;
			py = (j + 1);
			glVertex2f(px, py);
			px = (i + 1);
			py = j;
			glVertex2f(px, py);
		}
		glVertex2f(px, py);
		glEnd();
	}	
	glutPostRedisplay();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glFlush();
	draw();
	glutSwapBuffers();
}
	
int main(int argc, char * argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutCreateWindow("Real-time smoke simulation and visualization");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);


	
	glutMainLoop();
}
