/*
* @Author: kmrocki
* @Date:   2016-11-07 18:30:46
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-11-07 18:33:13
*/

#ifdef USE_OPENGL

#ifndef __GL_UTILS_HPP__
#define __GL_UTILS_HPP__

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string.h>

void draw_text(float x, float y, char* string, float r, float g, float b, float a, void* font) {

	int len, i;

	glColor4f(r, g, b, a);

	glRasterPos2i((int)x, (int)y);

	len = (int) strlen(string);

	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, string[i]);

}

void draw_grid(void) {

	glColor4f(0.0f, 0.9f, 0.0f, 0.17f);

	glBegin(GL_LINES);

	for (float i = -2; i <= 2; i += 0.1f) {
		glVertex3f((float)i, -2.0f, 0.0f);
		glVertex3f((float)i, 2.0f, 0.0f);
		glVertex3f(-2.0f, (float)i, 0.0f);
		glVertex3f(2.0f, (float)i, 0.0f);
	}

	glEnd();

	glColor4f(0.0f, 0.0f, 0.9f, 0.4f);

	glBegin(GL_LINES);

	for (float i = -2; i <= 2; i += 0.1f) {

		glVertex3f((float)i, -2.0f, 0.0f);
		glVertex3f((float)i, 2.0f, 0.0f);
		glVertex3f(-2.0f, (float)i, 0.0f);
		glVertex3f(2.0f, (float)i, 0.0f);
	}

	glEnd();

	glColor4f(0.9f, 0.0f, 0.0f, 0.4f);

	glBegin(GL_LINES);
	glVertex3f(0.0f, -50.0f, 0.0f);
	glVertex3f(0.0f, 50.0f, 0.0f);
	glVertex3f(-50.0f, 0.0f, 0.0f);
	glVertex3f(50.0f, 0.0f, 0.0f);
	glEnd();


}

void draw_grid_labels(void) {

	char txt[32];

	for (int i = -2; i <= 2; i += 1) {
		sprintf(txt, "(%.1f, %.1f, %.1f)", (float)i, -2.0f, 0.0f);
		draw_text((float)i, -2.0f, txt, 0.7f, 0.7f, 0.7f, 0.7f, GLUT_BITMAP_HELVETICA_10);
		//draw_plus((float)i, -2.0f, 1.0f, 2.0f, 0.9f, 0.0f, 0.0f, 0.7f);
		sprintf(txt, "(%.1f, %.1f, %.1f)", 2.0f, (float)i, 0.0f);
		draw_text((float)i, 2.0f, txt, 0.7f, 0.7f, 0.7f, 0.7f, GLUT_BITMAP_HELVETICA_10);
		//draw_plus((float)i, 2.0f, 1.0f, 2.0f, 0.9f, 0.0f, 0.0f, 0.7f);
	}

	for (int i = -2; i < 2; i += 50) {
		sprintf(txt, "(%.1f, %.1f, %.1f)", (float)i, -2.0f, 0.0f);
		draw_text(-2.0f, (float)i, txt, 0.7f, 0.7f, 0.7f, 0.7f, GLUT_BITMAP_HELVETICA_10);
		//draw_plus(-2.0f, (float)i, 1.0f, 2.0f, 0.9f, 0.0f, 0.0f, 0.7f);
		sprintf(txt, "(%.1f, %.1f, %.1f)",  2.0f, (float)i, 0.0f);
		draw_text(2.0f, (float)i, txt, 0.7f, 0.7f, 0.7f, 0.7f, GLUT_BITMAP_HELVETICA_10);
		//draw_plus(2.0f, (float)i, 1.0f, 2.0f, 0.9f, 0.0f, 0.0f, 0.7f);
	}

}

#endif /* __GL_MAIN_HPP__ */
#endif /* USE_OPENGL */
