#include "includes.hpp"
#include "view.hpp"
#include "controller.hpp"
#include "audio.hpp"
#include "menu.hpp"

// State Variable Initializer
void init_vars() {
	cam.h_angle = d2r(CAM_H_ANGLE);
	cam.v_angle = d2r(CAM_V_ANGLE);
	cam.radius = CAM_RADIUS;
	cam.c = { CAM_C_X, CAM_C_Y, CAM_C_Z };
	cam.o = { CAM_O_X, CAM_O_Y, CAM_O_Z };
	timestamp = 0;
	generate();
	our_state = s_menu;
	color = 0;
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (our_state) {
	case s_menu:
		use_2d_cam();
		kill_realism();
		draw_background();
		use_3d_cam();
		use_realism();
		DrawMainMenu();
		break;
	case s_game:
		use_2d_cam();
		kill_realism();
		draw_background();
		use_3d_cam();
		use_realism();
		draw_axes();
		draw_grid();
		draw_ship();
		break;
	case s_over:
		break;
	}
	

	glutSwapBuffers();
	glFlush();
}

// Animation
void anim() {
	switch (our_state) {
	case s_menu:
		break;
	case s_game:
		timestamp++;
		distance += 0.18;
		row = (int)distance;
		if (ypos)
		{
			ypos += v;
			v -= 0.075;
			if (ypos < 0) ypos = 0;
		}
		std::cout << row << '\n';
		break;
	case s_over:
		break;
	}
	
	glutPostRedisplay();
}

// Keyboard
void keys(unsigned char key, int x, int y) {
	switch (our_state) {
	case s_menu:
		break;
		// End state
	case s_game:
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
		case('x') :
			v = 0.5;
			ypos = 0.001;
			break;
		}
		break;
		// End state
	case s_over:
		break;
		// End state
	}

	// glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	switch (our_state) {
	case s_menu:
		if (x >= 542 && x <= 737 && y >= 287 && y <= 349 && state == GLUT_DOWN) {
			color = 0.5;
			//our_state = s_game;
		}
		else if (x >= 542 && x <= 737 && y >= 287 && y <= 349 && state == GLUT_UP) {
			our_state = s_game;
		} 
		else {
			color = 0;
		}
		break;
	case s_game:
		break;
	case s_over:
		break;
	}
}

// Setup and Run
void main(int argc, char** argv) {
	// Game
	init_vars();
	// Window
	glutInit(&argc, argv);
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	glutInitWindowSize(WINDOW_SIZE_X, WINDOW_SIZE_Y);
	glutCreateWindow(WINDOW_TITLE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glClearColor(CLEAR_R, CLEAR_G, CLEAR_B, CLEAR_A);
	// Funcs
	glutDisplayFunc(DISPLAY_FUNC);
	glutIdleFunc(IDLE_FUNC);
	glutKeyboardFunc(KEYBOARD_FUNC);
	glutMouseFunc(MOUSE_FUNC);
	// Palette
	set_light_and_material();
	use_realism();
	// Cam
	use_3d_cam();
	// Textures
	loadBMP(&sky_tex, "tex/sky2.bmp");
	loadBMP(&metal_tex, "tex/metal1.bmp");
	loadBMP(&ice_tex, "tex/ice0.bmp");
	loadBMP(&fire_tex, "tex/fire0.bmp");
	// Execute
	glutMainLoop();
}
