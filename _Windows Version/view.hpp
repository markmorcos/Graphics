#ifndef view_h
#define view_h

#include "includes.hpp"

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
	glOrtho(-WINDOW_SIZE_X / 2.0f, WINDOW_SIZE_X / 2.0f, -WINDOW_SIZE_Y / 2.0f, WINDOW_SIZE_Y / 2.0f, 1, 3); // Define this?
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
	glBegin(GL_LINES);
	cl(255, 0, 0); vr(-20, 0, 0); vr(20, 0, 0);
	cl(0, 255, 0); vr(0, -20, 0); vr(0, 20, 0);
	cl(0, 0, 255); vr(0, 0, -20); vr(0, 0, 20);
	glEnd();
	bye;
}

void draw_background() {
	hi;
	use_tex();
	glBindTexture(GL_TEXTURE_2D, sky_tex);
	float x = WINDOW_SIZE_X / 2.0f;
	float y = WINDOW_SIZE_Y / 2.0f;
	nr(0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
	cd(0.0f, 0.0f); vr(-x, -y, 0.0f);
	cd(1.0f, 0.0f); vr(x, -y, 0.0f);
	cd(1.0f, 1.0f); vr(x, y, 0.0f);
	cd(0.0f, 1.0f); vr(-x, y, 0.0f);
	glEnd();
	kill_tex();
	bye;
}

void draw_tex_triangle(pt p1, pt p2, pt p3, pt c) {
	hi;
	glBegin(GL_TRIANGLES);

	nr(p1.x - c.x, p1.y - c.y, p1.z - c.z);
	cd(0.0f, 0.0f);
	vr(p1.x, p1.y, p1.z);

	nr(p2.x - c.x, p2.y - c.y, p2.z - c.z);
	cd(1.0f, 0.0f);
	vr(p2.x, p2.y, p2.z);

	nr(p3.x - c.x, p3.y - c.y, p3.z - c.z);
	cd(0.5f, 0.7f);
	vr(p3.x, p3.y, p3.z);

	glEnd();
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


void draw_ship() {
	hi;
	glTranslatef(xpos, ypos, 0);
	glScalef(1, 1, -1);
	pt a = { 0.25f, 0.0f, 0.1f };
	pt b = { 0.5f, 0.0f, 1.0f };
	pt c = { 0.5f, 0.5, 0.0f };
	pt d = { 0.75f, 0, 0.1f };

	draw_tex_tetrahedron(a, b, c, d);
	bye;
}

void draw_tex_rect(pt p1, pt p2, pt p3, pt p4) {
	hi;
	glBegin(GL_QUADS);
	cd(0, 0); vr(p1.x, p1.y, p1.z);
	cd(0, 1); vr(p2.x, p2.y, p2.z);
	cd(1, 1); vr(p3.x, p3.y, p3.z);
	cd(1, 0); vr(p4.x, p4.y, p4.z);
	glEnd();
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
	for (int k = 0; k < ROWS; k++) {
		for (int i = 0; i < COLUMNS; i++) {
			type tile_type = grid[k][i];
			if (tile_type == GROUND) {
				glBindTexture(GL_TEXTURE_2D, ice_tex);
				draw_tex_cuboid(i, i + TILE_SIZE, -0.2f, 0, -k + distance, -k - TILE_SIZE + distance);
			}
			if (tile_type == FIRE) {
				glBindTexture(GL_TEXTURE_2D, fire_tex);
				draw_tex_cuboid(i, i + TILE_SIZE, -0.2f, 0.5, -k + distance, -k - TILE_SIZE + distance);
			}
		}
	}
	kill_tex();
}
#endif