#include "includes.hpp"

#include "audio.hpp"
#include "controller.hpp"
#include "model.hpp"
#include "view.hpp"

// include any fine in another using this format

void keyboard(unsigned char key, int x, int y)
{
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glutPostRedisplay();
	glutSwapBuffers();
	glFlush();
}

void idle()
{
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Skyroads");
	gluPerspective(30.0f, WINDOW_WIDTH / WINDOW_HEIGHT, 0.1, 10000);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glutMainLoop();

}
