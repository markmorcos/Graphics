#ifndef menu_h
#define menu_h

#include "includes.hpp"

void drawString(std::string s, int x, int y) {
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, 1280, 0.0, 800);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRasterPos2i(x, y);

	void * font;
	if (s == "Rock The Skys!")
		font = GLUT_BITMAP_HELVETICA_18;
	else
		font = GLUT_BITMAP_TIMES_ROMAN_24;
	for (std::string::iterator i = s.begin(); i != s.end(); ++i)
	{
		char c = *i;
		glutBitmapCharacter(font, c);
	}
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
void DrawMainMenu() {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPerspective(45.0f, 1280 / 800, 0.1f, 800.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	

	glColor3f(1, 1, 0);
	drawString("Skyroads", 600, 530);
	
	glColor3f(1, 0, 1);
	drawString("Rock The Skies!", 560, 450);

	glPushMatrix();
	glColor3f(1 - color, 1, 0);
	glScaled(1.2, 0.6, 1);
	glTranslated(0, 0.9, 0);
	glutSolidCube(1);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
#endif