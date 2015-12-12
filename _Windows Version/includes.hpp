#ifndef includes_h
#define includes_h
#include "TextureBuilder.h"
#include <ctime>
#include <iostream>

#include <math.h>
#include <glut.h>

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
#define KEYBOARD_FUNC		keys
#define SPECIAL_FUNC		special
#define MOUSE_FUNC			mouse

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

#define TILE_SIZE			1.0f
#define GROUND_LEVEL		0.0f

#define LIGHT_AMBIENT		0.3f
#define LIGHT_DIFFUSE		0.5f
#define LIGHT_SPECULAR		0.8f
#define LIGHT_SHININESS		100.0f
#define LIGHT_EMISSION		0.7f
#define LIGHT_NO_EMISSION	0.0f

// Shortening Macros
#define hi					glPushMatrix()
#define bye					glPopMatrix()
#define hey_clear_first		
#define hey_flush_last		
#define hey_redisplay_last	
#define pt					triple<float>
#define cl(x, y, z)			glColor3f(b2f(x), b2f(y), b2f(z))
#define vr					glVertex3f
#define nr					glNormal3f
#define tex					glEnable(GL_TEXTURE_2D);
#define untex				glDisable(GL_TEXTURE_2D);
#define cd					glTexCoord2f

// Type definitions
enum game_state {
	s_menu,
	s_game,
	s_over,
	s_win
};

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
game_state our_state;
float color;


// Spaceship
float distance = 0;
float v = 0, xpos = 0, ypos = 0;
int row = 0, column = 1;

// Grid
const int ROWS = 200, COLUMNS = 3;
enum type { GROUND, HOLE, FIRE };
type grid[ROWS][COLUMNS];

// Seed
long long seed = time(0);

// Helper
unsigned long long randomNumber()
{
  unsigned long long x = 2523523 * seed, y = 1288924102 * seed, z = 58327598 * seed;
  unsigned long long t;
  x ^= x << 16;
  x ^= x >> 5;
  x ^= x << 1;
  t = x;
  x = y;
  y = z;
  z = t ^ x ^ y;
  return seed = z;
}

#endif
