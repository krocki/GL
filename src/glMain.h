/*
* @Author: kmrocki
* @Date:   2016-11-07 18:30:45
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-11-09 21:24:27
*/

#ifdef USE_OPENGL

#ifndef __GL_MAIN_HPP__
#define __GL_MAIN_HPP__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <mutex>
#include <unistd.h>
#include <timer.h>
#include <rand.h>
#include <glUtils.h>
#include <vector>
#include <cell.h>

#define FRAMERATE 50
#define DEFAULT_WIDTH 512
#define DEFAULT_HEIGHT 512

#define INITIAL_THETA 270.0f
#define ANGULAR_THETA 310.0f
#define INITIAL_PHI 360.0f
#define ANGULAR_PHI 360.0f
#define INITIAL_RHO 4.5f
#define ANGULAR_RHO 450.0f
#define NEAR_PLANE 1
#define FAR_PLANE 2000

extern std::vector<Cell> cells;
std::mutex display_mutex;
GLint window_id;

//shaders
GLuint ps, vs, prog, r_mod;

//camera
GLfloat eyeX = 0.0f;
GLfloat eyeY = 0.0f;
GLfloat eyeZ = 2.0f;
GLfloat dirX = 0.0f;
GLfloat dirY = 0.0f;
GLfloat dirZ = 0.0f;
GLfloat upX = 0.0f;
GLfloat upY = 0.0f;
GLfloat upZ = 0.0f;

GLfloat	theta = INITIAL_THETA;
GLfloat	phi = INITIAL_PHI;
GLfloat	rho = INITIAL_RHO;

int mouse_x;
int mouse_y;
int window_height;
int window_width;
int previous_height;
int previous_width;

void idle();
void teardown();
void reshape(int width, int height);
void display();
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void pmotion(int x, int y);
void update_eye_position(void);
void show_camera_info(void);

void mode_2d();
void mode_3d();

bool show_grid = false;
bool fullscreen = false;

bool _pause = false;
bool _step = false;

void glinit() {

	char* argv[1];
	int argc = 1;
	argv [0] = "GLMain";

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	GLint x = (glutGet(GLUT_SCREEN_WIDTH) - DEFAULT_WIDTH) / 2;
	GLint y = (glutGet(GLUT_SCREEN_HEIGHT) - DEFAULT_HEIGHT) / 2;
	GLint w = DEFAULT_WIDTH;
	GLint h = DEFAULT_HEIGHT;

	glutInitWindowPosition(x, y);
	window_id = glutCreateWindow("GLMain");

	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(pmotion);
	glutReshapeFunc(reshape);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	// antialiasing
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	update_eye_position();

}

void display() {

	display_mutex.lock();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 3D
	mode_3d();


	if (show_grid) {
		draw_grid();
		draw_grid_labels();
	}

	glColor4f(1.0f, 1.0f, 1.0f, 0.9f);

	glPointSize(3);
	glBegin(GL_POINTS);

	for (size_t i = 0; i < cells.size(); i++) {

		if (cells[i].active && cells[i].predicted)
			glColor4f(1.0f, 0.0f, 1.0f, 0.9f);
		else if (cells[i].predicted)
			glColor4f(0.0f, 0.0f, 1.0f, 0.9f);
		else if (cells[i].active)
			glColor4f(1.0f, 0.0f, 0.0f, 0.9f);
		else
			glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

		glVertex3f(cells[i].location.x, cells[i].location.y, cells[i].location.z);

	}

	glEnd();

	// 2D
	mode_2d();
	//show overlay text
	show_camera_info();

	display_mutex.unlock();

	glutSwapBuffers();
}

void idle(void) {

	usleep(1000000.0 / FRAMERATE);
	glutSetWindow(window_id);
	glutPostRedisplay();

}

void reshape(int width, int height) {

	window_height = height;
	window_width = width;

	glViewport(0, 0, width, height);

}

void keyboard(unsigned char key, int x, int y) {

	switch (key) {

	// case ' ': // space
	// 	_pause = !_pause;
	// 	break;
	// case '\t': // tab
	// 	_step = true;
	// 	break;

	case 'g':
		show_grid = !show_grid;
		break;

	case 'w':
		theta += 10.0f;
		update_eye_position();
		break;
	case 's':
		theta -= 10.0f;
		update_eye_position();
		break;
	case 'a':
		phi += 10.0f;
		update_eye_position();
		break;
	case 'd':
		phi -= 10.0f;
		update_eye_position();
		break;
	case 'z':
		rho *= 1.2f;
		update_eye_position();
		break;
	case 'x':
		rho /= 1.2f;
		update_eye_position();
		break;

	case 'f':
		if (!fullscreen) {

			previous_height = window_height;
			previous_width = window_width;

			glutFullScreen();

			fullscreen = true;


		} else if (fullscreen) {

			window_height = previous_height;
			window_width = previous_width;

			glutReshapeWindow(window_width, window_height);
			glutPositionWindow((glutGet(GLUT_SCREEN_WIDTH) - window_width) / 2,
			                   (glutGet(GLUT_SCREEN_HEIGHT) - window_height) / 2);

			fullscreen = false;
		}

		break;

	case '[':
		for (size_t i = 0; i < cells.size(); i++)
			cells[i].move(0.1f);
		break;
	case ']':
		for (size_t i = 0; i < cells.size(); i++)
			cells[i].move(-0.1f);
		break;
	case ' ': // space
		_pause = !_pause;
		break;
	case '\t': // tab
		_step = true;
		break;

	case 27: // Escape key
		printf("OpenGL: ESC signal...\n");

		// don't allow other threads to do anything
		display_mutex.lock();
		glutDestroyWindow(window_id);
		cells.clear();

		exit(0);

	}
}

void mouse(int button, int state, int x, int y) {

	mouse_x = x;
	mouse_y = y;

	printf("click: state %d, button %d, %d, %d\n", state, button, x, y);

}

void pmotion(int x, int y) {

	mouse_x = x;
	mouse_y = y;

}

void motion(int x, int y) {

	// Mouse point to angle conversion

	int dx = x - mouse_x;
	int dy = y - mouse_y;

	mouse_x = x;
	mouse_y = y;

	theta -= (360.0f / window_height) * (dy) * 0.5f;
	phi += (360.0f / window_width) * (dx) * 0.5f;

	// Restrict the angles within 0~360 deg (optional)

	update_eye_position();

	glutPostRedisplay();

}

void update_eye_position(void) {

	if (theta > 360.0f) theta = fmodf(theta, 360.0f);
	if (phi > 360.0f) phi = fmodf(phi, 360.0f);

	// Spherical to Cartesian conversion.
	// Degrees to radians conversion factor 0.0174532f
	eyeX = rho * sinf(theta * 0.0174532f) * sinf(phi * 0.0174532f);
	eyeY = rho * cosf(theta * 0.0174532f);
	eyeZ = rho * sinf(theta * 0.0174532f) * cosf(phi * 0.0174532f);

	// Reduce theta slightly to obtain another point on the same longitude line on the sphere.
	GLfloat dt = 1.0;
	GLfloat eyeXtemp = rho * sinf(theta * 0.0174532f - dt) * sinf(phi * 0.0174532f);
	GLfloat eyeYtemp = rho * cosf(theta * 0.0174532f - dt);
	GLfloat eyeZtemp = rho * sinf(theta * 0.0174532f - dt) * cosf(phi * 0.0174532f);

	// Connect these two points to obtain the camera's up vector.
	upX = eyeXtemp - eyeX;
	upY = eyeYtemp - eyeY;
	upZ = eyeZtemp - eyeZ;

}

void show_camera_info(void) {

	char cam_info[256];

	sprintf(cam_info, "Camera Position: (%.3f, %.3f, %.3f), Looking at = (%.3f, %.3f, %.3f), Normal vector = (%.3f, %.3f, %.3f), rho = %.2f, theta = %.2f, phi = %.2f", eyeX, eyeY, eyeZ, dirX, dirY, dirZ, upX, upY, upZ, rho, theta, phi);

	draw_text(5.0f, window_height - 5.0f, cam_info, 1.0f, 1.0f, 1.0f, 0.7f, GLUT_BITMAP_HELVETICA_12);

}

void mode_2d(void) {

	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, window_width, window_height, 0);

}

void mode_3d(void) {

	// 3D
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float windowAspectRatio = (float)window_width / (float)window_height;
	float halfWindowAspectRatio = windowAspectRatio * 0.5f;
	float left, right, top, bottom;

	top = 0.5f; // remember this is the top of the texture
	left = -halfWindowAspectRatio;
	right = -left;
	bottom = -top;

	glFrustum(left, right, bottom, top, NEAR_PLANE, FAR_PLANE);

	gluLookAt(eyeX, eyeY, eyeZ, dirX, dirY, dirZ, upX, upY, upZ);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}
#endif /* __GL_MAIN_HPP__ */
#endif /* USE_OPENGL */
