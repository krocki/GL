/*
* @File: glWindow.h
* @Author: kamilrocki
* @Email: kmrocki@us.ibm.com
* @Created:	2015-05-12 09:11:50
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-11-08 21:30:53
*/

#ifndef __GL_WINDOW_H__
#define __GL_WINDOW_H__

#ifdef USE_OPENGL

#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <map>
#include <mutex>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include <colormap.h>

#define framerate 50
#define DEFAULT_WIDTH 512
#define DEFAULT_HEIGHT 512
#define eps 10e-6

std::mutex display_mutex;

typedef enum { MODE_GRAY, MODE_RGB } DisplayMode;

typedef struct {

	GLint x;
	GLint y;
	GLint w;
	GLint h;

} window_properties;

std::map<std::string, window_properties> w_props;
std::map<int, Matrix<float>*> windows;
std::map<int, DisplayMode> modes;

void draw_filled_rectangle(float x, float y, float h, float w, float r, float g, float b, float a) {

	glColor4f(r, g, b, a);

	glBegin(GL_QUADS);

	//margins, top frame
	glVertex2i((int)(x), (int)(y));
	glVertex2i((int)(x + w), (int)(y));
	glVertex2i((int)(x + w), (int)(y + h));
	glVertex2i((int)(x), (int)(y + h));

	glEnd();

}

void draw_grid(float r, float g, float b, float a, float cells_h, float cells_v) {

	GLint h = glutGet(GLUT_WINDOW_HEIGHT);
	GLint w = glutGet(GLUT_WINDOW_WIDTH);

	float scale_x = (float)w / (float)(cells_h);
	float scale_y = (float)h / (float)(cells_v);

	glColor4f(r, g, b, a);
	glBegin(GL_LINES);

	for (unsigned i = 1; i < cells_h; i++) {

		glVertex2i((int)((float)i * scale_x), 0);
		glVertex2i((int)((float)i * scale_x), (GLint)h);

	}

	for (unsigned i = 1; i < cells_v; i++) {

		glVertex2i(0, (int)((float)i * scale_y));
		glVertex2i((GLint)w, (int)((float)i * scale_y));

	}

	glEnd();
}

void draw_text(float x, float y, char* string, float r, float g, float b, float a, void* font) {

	int len, i;

	glColor4f(r, g, b, a);

	glRasterPos2i(static_cast<int>(x), static_cast<int>(y));

	len = static_cast<int>(strlen(string));

	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, string[i]);

}

class GLWindowRenderer {

public:

	static void init(void) {

		char* argv[1];
		int argc = 1;
		argv [0] = "GLWindowRenderer";

		glutInit(&argc, argv);
		load_properties_from_file();

	}

	static void load_properties_from_file(void) {

		// w_props.insert({"lena", {30, 30, 200, 200}});
		// w_props.insert({"lena", {280, 30, 200, 200}});
		// w_props.insert({"y", {530, 30, 200, 200}});

		// FILE * file = fopen("prefs.dat", "rb");

		// if (file != NULL) {

		// 	fread(&w_props, sizeof(w_props), 1, file);
		// 	fclose(file);

		// }

	}

	static void save_properties_to_file(void) {

		// FILE * file = fopen("prefs.dat", "wb");

		// if (file != NULL) {

		// 	fwrite(&w_props, sizeof(w_props), 1, file);
		// 	fclose(file);

		// }

	}

	static void keyboard(unsigned char key, int x, int y) {

		switch (key) {

		case 27: // Escape key
			printf("OpenGL: ESC signal...\n");

			// don't allow other threads to do anything
			display_mutex.lock();
			for (std::map<int, Matrix<float>*>::iterator it = windows.begin(); it != windows.end(); ++it) {
				printf("-%d\n", it->first);
				glutDestroyWindow(it->first);
			}

			save_properties_to_file();
			windows.clear();
			modes.clear();
			exit(0);
			//

		}
	}

	static void display(void) {

		display_mutex.lock();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//display_image(pi, w, h, preview_show_grid, false);

		Matrix<float>* im = windows.find(glutGetWindow())->second;
		DisplayMode mode = modes.find(glutGetWindow())->second;

		if (im == nullptr)
			std::cout << "null" << std::endl;

		//display matrix

		if (im != nullptr) {

			float w = im->cols();
			float h = im->rows();

			float scale_x = (float)glutGet(GLUT_WINDOW_HEIGHT) / (float)(h);
			float scale_y = (float)glutGet(GLUT_WINDOW_WIDTH) / (float)(w);

			float l2 = 0.0f;

			for (unsigned y = 0; y < im->rows(); y++) {

				for (unsigned x = 0; x < im->cols(); x++) {

					float val = (*im)(y, x);
					l2 += val * val;

				}
			}

			l2 = sqrtf(l2);

			if (mode == MODE_GRAY) {
				for (unsigned y = 0; y < im->rows(); y++) {

					for (unsigned x = 0; x < im->cols(); x++) {

						float val = (*im)(y, x);

						//normalize
						// val /= l2;
						// val += 0.5f;

						//printf("%f\n", val);

						color_rgba mapped_color = map_value_to_color(val, 0.0f, 1.0f, COLORMAP_GRAY);

						draw_filled_rectangle(float(x) * scale_y, float(y) * scale_x, scale_x, scale_y,
						                      ((float)(mapped_color.r) / 255.0f),
						                      ((float)(mapped_color.g) / 255.0f),
						                      ((float)(mapped_color.b) / 255.0f),
						                      ((float)(mapped_color.a) / 255.0f));


					}

				}
			}

			if (mode == MODE_RGB) {

				scale_y = scale_y * 3;

				for (unsigned y = 0; y < im->rows(); y++) {

					for (unsigned x = 0; x < im->cols() / 3; x++) {

						float r = (*im)(y, x);
						float g = (*im)(y, x + w / 3);
						float b = (*im)(y, x + 2 * w / 3);

						draw_filled_rectangle(float(x) * scale_y, float(y) * scale_x, scale_x, scale_y,
						                      r, g, b, 1);


					}

					//draw_grid(0.5f, 0.3f, 0.3f, 0.3f, w, h);

				}
			}

			display_mutex.unlock();

			glutSwapBuffers();
		}

	}

	static void add(std::string name, GLint x = 0, GLint y = 0, GLint w = DEFAULT_WIDTH, GLint h = DEFAULT_HEIGHT) {

		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

		glutInitWindowPosition(x, y);
		glutInitWindowSize(w, h);

		GLint id = glutCreateWindow(name.c_str());
		glutDisplayFunc(display);
		glutKeyboardFunc(keyboard);
		glutReshapeFunc(reshape);
		// glutReshapeFunc(reshape);
		// glutMouseFunc(mouse_func);
		// glutMotionFunc(motion_func);
		// glutPassiveMotionFunc(pmotion_func);
		windows.insert ( {id, nullptr});
		modes.insert ( {id, MODE_GRAY});
		w_props.insert({name, {x, y, w, h}});
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	}

	static void set(int id, Matrix<float>* p, DisplayMode mode = MODE_GRAY) {

		windows.find(id)->second = p;
		modes.find(id)->second = mode;

	}

	static void reshape(int width, int height) {

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//set the coordinate system, with the origin in the top left
		gluOrtho2D(0, width, height, 0);
		glMatrixMode(GL_MODELVIEW);

	}

	static void idle(void) {

		//for all windows in the list

		usleep(1000000.0 / framerate);
		for (std::map<int, Matrix<float>*>::iterator it = windows.begin(); it != windows.end(); it++) {

			//printf("glutPostRedisplay_%d\n", it->first);
			if (it->second != nullptr) {

				glutSetWindow(it->first);
				glutPostRedisplay();

			}

		}

	}

	static void main_loop(void) {

		glutIdleFunc(idle);
		glutMainLoop();

	}

};

#endif

#endif
