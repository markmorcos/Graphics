#include "TextureBuilder.h"

#include <math.h>
#include <glut.h>

#include "includes.hpp"
#include "controller.hpp"
#include "audio.hpp"

// Numeric Constants
#define PI					3.14159265358979f
#define TAU					(2.0f * PI)

// Conversion Macros
#define b2f(b)				(b / 255.0f)
#define d2r(d)				((d / 360.0f) * TAU)
#define r2d(d)				((d / TAU) * 360.0f)

// Game Constants
#define WINDOW_SIZE_X		1280.0f
#define WINDOW_SIZE_Y		800.0f
#define WINDOW_POS_X		0.0f
#define WINDOW_POS_Y		0.0f
#define WINDOW_TITLE		"Skyroads"
#define CLEAR_R				b2f(255)
#define CLEAR_G				b2f(255)
#define CLEAR_B				b2f(255)
#define CLEAR_A				b2f(0)

#define DISPLAY_FUNC		display
#define IDLE_FUNC			anim

#define VIEW_ANGLE			45.0f
#define NEAR_PLANE			0.1f
#define FAR_PLANE			200.0f
#define CAM_H_ANGLE			90.0f
#define CAM_V_ANGLE			30.0f
#define CAM_RADIUS			10.0f
#define CAM_C_X				0.0f
#define CAM_C_Y				0.0f
#define CAM_C_Z				0.0f
#define CAM_O_X				0.0f
#define CAM_O_Y				1.0f
#define CAM_O_Z				0.0f

#define LIGHT_AMBIENT		0.3f
#define LIGHT_DIFFUSE		0.5f
#define LIGHT_SPECULAR		0.8f
#define LIGHT_SHININESS		100.0f
#define LIGHT_EMISSION		0.7f
#define LIGHT_NO_EMISSION	0.0f

#define TILE_SIZE			1.0f
#define GROUND_LEVEL		0.0f

// Shortening Macros
#define hi					glPushMatrix()
#define bye					glPopMatrix()
#define bgn					glBegin
#define end					glEnd()
#define hey_clear_first		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
#define hey_flush_last		glFlush()
#define hey_redisplay_last	glutPostRedisplay()
#define pt					triple<float>
#define cl(x, y, z)			glColor3f(b2f(x), b2f(y), b2f(z))
#define vr					glVertex3f
#define nr					glNormal3f
#define tex					glEnable(GL_TEXTURE_2D);
#define untex				glDisable(GL_TEXTURE_2D);
#define cd					glTexCoord2f

// Type definitions
template <typename Type>
struct triple {
	Type x;
	Type y;
	Type z;
};

struct cam {
	float h_angle;
	float v_angle;
	float radius;
	pt c;
	pt o;

	pt get_eye() {
		float y = radius * sinf(v_angle);
		float z = radius * cosf(v_angle) * sinf(h_angle);
		float x = radius * cosf(v_angle) * cosf(h_angle);
		return{ x + c.x, y + c.y, z + c.z };
	}
};

// Resource Variables
GLuint sky_tex;
GLuint metal_tex;
GLuint ice_tex;
GLuint fire_tex;

// Constant Variables
const GLfloat position[] = { 0.0f, 50.0f, 0.0f, 0.0f }; // Define this
const GLfloat ambient[] = { LIGHT_AMBIENT, LIGHT_AMBIENT, LIGHT_AMBIENT, 1.0f };
const GLfloat diffuse[] = { LIGHT_DIFFUSE, LIGHT_DIFFUSE, LIGHT_DIFFUSE, 1.0f };
const GLfloat specular[] = { LIGHT_SPECULAR, LIGHT_SPECULAR, LIGHT_SPECULAR, 1.0f };
const GLfloat shininess[] = { LIGHT_SHININESS };
const GLfloat emission[] = { LIGHT_EMISSION, LIGHT_EMISSION, LIGHT_EMISSION, 1.0f };
const GLfloat no_emission[] = { LIGHT_NO_EMISSION, LIGHT_NO_EMISSION, LIGHT_NO_EMISSION, 1.0f };

// State Variables
struct cam cam;
long long timestamp;

// State Variable Initializer
void init_vars() {
	cam.h_angle = d2r(CAM_H_ANGLE);
	cam.v_angle = d2r(CAM_V_ANGLE);
	cam.radius = CAM_RADIUS;
	cam.c = { CAM_C_X, CAM_C_Y, CAM_C_Z };
	cam.o = { CAM_O_X, CAM_O_Y, CAM_O_Z };
	timestamp = 0;

	generate();
}

// Camera, Lights
void set_light_and_material() {
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
	glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);
	glShadeModel(GL_SMOOTH);
}

void use_realism() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
}

void kill_realism() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_NORMALIZE);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_COLOR_MATERIAL);
}

void use_emission() {
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
}

void kill_emission() {
	glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
}

void use_tex() {
	glEnable(GL_TEXTURE_2D);
	cl(255, 255, 255);
}

void kill_tex() {
	glDisable(GL_TEXTURE_2D);
}

void use_2d_cam() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-WINDOW_SIZE_X/2.0f, WINDOW_SIZE_X/2.0f, -WINDOW_SIZE_Y/2.0f, WINDOW_SIZE_Y/2.0f, 1, 3); // Define this?
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f); // Define this?
}

void use_3d_cam() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(VIEW_ANGLE, WINDOW_SIZE_X / WINDOW_SIZE_Y, NEAR_PLANE, FAR_PLANE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	pt e = cam.get_eye();
	gluLookAt(e.x, e.y, e.z, cam.c.x, cam.c.y, cam.c.z, cam.o.x, cam.o.y, cam.o.z);
}

// Drawing
void draw_axes() {
	hi;
	glLineWidth(3);
	bgn(GL_LINES);
	cl(255, 0, 0); vr(-20, 0, 0); vr(20, 0, 0);
	cl(0, 255, 0); vr(0, -20, 0); vr(0, 20, 0);
	cl(0, 0, 255); vr(0, 0, -20); vr(0, 0, 20);
	end;
	bye;
}

void draw_background() {
	hi;
	use_tex();
	glBindTexture(GL_TEXTURE_2D, sky_tex);
	float x = WINDOW_SIZE_X / 2.0f;
	float y = WINDOW_SIZE_Y / 2.0f;
	nr(0.0f, 0.0f, 1.0f);
	bgn(GL_QUADS);
	cd(0.0f, 0.0f); vr(-x, -y, 0.0f);
	cd(1.0f, 0.0f); vr(x, -y, 0.0f);
	cd(1.0f, 1.0f); vr(x, y, 0.0f);
	cd(0.0f, 1.0f); vr(-x, y, 0.0f);
	end;
	kill_tex();
	bye;
}

void draw_tex_triangle(pt p1, pt p2, pt p3, pt c) {
	hi;
	bgn(GL_TRIANGLES);

	nr(p1.x - c.x, p1.y - c.y, p1.z - c.z);
	cd(0.0f, 0.0f);
	vr(p1.x, p1.y, p1.z);

	nr(p2.x - c.x, p2.y - c.y, p2.z - c.z);
	cd(1.0f, 0.0f);
	vr(p2.x, p2.y, p2.z);

	nr(p3.x - c.x, p3.y - c.y, p3.z - c.z);
	cd(0.5f, 0.7f);
	vr(p3.x, p3.y, p3.z);

	end;
	bye;
}

void draw_tex_tetrahedron(pt p1, pt p2, pt p3, pt p4) {
	hi;
	use_tex();
	glBindTexture(GL_TEXTURE_2D, metal_tex);
	pt c = { (p1.x + p2.x + p3.x + p4.x) / 4.0f, (p1.y + p2.y + p3.y + p4.y) / 4.0f, (p1.z + p2.z + p3.z + p4.z) / 4.0f };
	draw_tex_triangle(p1, p2, p3, c);
	draw_tex_triangle(p3, p2, p4, c);
	draw_tex_triangle(p4, p2, p1, c);
	draw_tex_triangle(p4, p1, p3, c);
	kill_tex();
	bye;
}

void draw_triangle(pt p1, pt p2, pt p3, pt c) {
	hi;
	bgn(GL_TRIANGLES);
	nr(p1.x - c.x, p1.y - c.y, p1.z - c.z);
	vr(p1.x, p1.y, p1.z);
	nr(p2.x - c.x, p2.y - c.y, p2.z - c.z);
	vr(p2.x, p2.y, p2.z);
	nr(p3.x - c.x, p3.y - c.y, p3.z - c.z);
	vr(p3.x, p3.y, p3.z);
	end;
	bye;
}

void draw_tetrahedron(pt p1, pt p2, pt p3, pt p4) {
	hi;

	pt c = { (p1.x + p2.x + p3.x + p4.x) / 4.0f, (p1.y + p2.y + p3.y + p4.y) / 4.0f, (p1.z + p2.z + p3.z + p4.z) / 4.0f };
	draw_triangle(p1, p2, p3, c);
	draw_triangle(p3, p2, p4, c);
	draw_triangle(p4, p2, p1, c);
	draw_triangle(p4, p1, p3, c);

	bye;
}

void draw_ship() {
	hi;
	glTranslatef(xpos, ypos, 0);
	glScalef(1, 1, -1);
	pt a = { 0.25f, 0.0f, 0.1f };
	pt b = { 0.5f, 0.0f, 1.0f };
	pt c = { 0.5f, 0.5, 0.0f };
	pt d = { 0.75f, 0, 0.1f };
	// glRotatef(time / 2.0, 1.0f, 0.0f, 0.0f);
	// glRotatef(-time / 7.0, 0.0f, 1.0f, 0.0f);
	// glRotatef(time / 15.0, 0.0f, 0.0f, 1.0f);
	draw_tex_tetrahedron(a, b, c, d);
	bye;
}

void draw_tex_rect(pt p1, pt p2, pt p3, pt p4) {
	hi;
	bgn(GL_QUADS);
	cd(0, 0); vr(p1.x, p1.y, p1.z);
	cd(0, 1); vr(p2.x, p2.y, p2.z);
	cd(1, 1); vr(p3.x, p3.y, p3.z);
	cd(1, 0); vr(p4.x, p4.y, p4.z);
	end;
	bye;
}

void draw_tex_cuboid(float l, float r, float b, float t, float k, float f) {
	hi;
	pt p000 = { l, b, k };
	pt p001 = { l, b, f };
	pt p010 = { l, t, k };
	pt p011 = { l, t, f };
	pt p100 = { r, b, k };
	pt p101 = { r, b, f };
	pt p110 = { r, t, k };
	pt p111 = { r, t, f };
	draw_tex_rect(p000, p001, p011, p010);
	draw_tex_rect(p100, p101, p111, p110);
	draw_tex_rect(p000, p001, p101, p100);
	draw_tex_rect(p010, p011, p111, p110);
	draw_tex_rect(p000, p010, p110, p100);
	draw_tex_rect(p001, p011, p111, p101);
	bye;
}

void draw_grid() {
	use_tex();
	for (int i = 0; i < ROWS; i++) {
		for (int k = 0; k < COLUMNS; k++) {
			type tile_type = grid[i][k];
			if (tile_type == GROUND) {
				glBindTexture(GL_TEXTURE_2D, ice_tex);
				draw_tex_cuboid(k, k + TILE_SIZE, -0.2f, 0, -i + distance, -i - TILE_SIZE + distance);
			}
			if (tile_type == FIRE) {
				glBindTexture(GL_TEXTURE_2D, fire_tex);
				draw_tex_cuboid(k, k + TILE_SIZE, -0.2f, 0.5, -i, -i - TILE_SIZE);
			}
		}
	}
	kill_tex();
}

void display(void) {
	hey_clear_first;
	use_2d_cam();
	kill_realism();
	draw_background();
	use_3d_cam();
	use_realism();
	draw_axes();
	draw_grid();
	draw_ship();
	hey_flush_last;
}

// Animation
void anim() {
	timestamp++;
	hey_redisplay_last;
}

void test_view() {

}

// Keyboard
void keys(unsigned char key, int x, int y) {
	switch (key) {
	case('j') :
		cam.h_angle += 0.1;
		break;
	case('l') :
		cam.h_angle -= 0.1;
		break;
	case('i') :
		cam.v_angle += 0.1;
		break;
	case('k') :
		cam.v_angle -= 0.1;
		break;
	case('u') :
		cam.radius += 0.1;
		break;
	case('o') :
		cam.radius -= 0.1;
		break;
	case('w') :
		ypos += 0.1;
		break;
	case('s') :
		ypos -= 0.1;
		break;
	case('a') :
		xpos -= 0.1;
		break;
	case('d') :
		xpos += 0.1;
		break;
	}
}

// Setup and Run
void main(int argc, char** argv) {
	// Game
	init_vars();
	test_view();
	// Window
	glutInit(&argc, argv);
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	glutCreateWindow(WINDOW_TITLE);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(CLEAR_R, CLEAR_G, CLEAR_B, CLEAR_A);
	// Funcs
	glutDisplayFunc(DISPLAY_FUNC);
	glutIdleFunc(IDLE_FUNC);
	glutKeyboardFunc(keys);
	// Palette
	set_light_and_material();
	use_realism();
	// Cam
	use_3d_cam();
	// Textures
	loadBMP(&sky_tex, "tex/sky1.bmp");
	loadBMP(&metal_tex, "tex/metal0.bmp");
	loadBMP(&ice_tex, "tex/ice0.bmp");
	loadBMP(&fire_tex, "tex/fire0.bmp");
	// Execute
	glutMainLoop();
}