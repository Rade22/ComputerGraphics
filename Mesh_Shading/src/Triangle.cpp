#include "Triangle.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <vector>

// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))


Triangle::Triangle(glm::vec3 &v0, glm::vec3 &v1, glm::vec3 &v2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;

	c[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[1] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[2] = glm::vec3(1.0f, 1.0f, 1.0f);
}

glm::vec3 Triangle::interpolateColor(glm::vec3 screen[3], int x, int y)
{
	glm::vec3 inter_color;
	float alpha, beta, gamma;
	float Xa, Xb, Xc, Ya, Yb, Yc;
	Xa = screen[0].x;
	Xb = screen[1].x;
	Xc = screen[2].x;
	Ya = screen[0].y;
	Yb = screen[1].y;
	Yc = screen[2].y;
	float neg = -1.0f;


	alpha = (neg*(x - Xb)*(Yc - Yb) + (y - Yb)*(Xc - Xb)) /
		(neg*(Xa - Xb)*(Yc - Yb) + (Ya - Yb)*(Xc - Xb));

	beta = (neg*(x - Xc)*(Ya - Yc) + (y - Yc)*(Xa - Xc)) /
		(neg*(Xb - Xc)*(Ya - Yc) + (Yb - Yc)*(Xa - Xc));

	gamma = 1.0f - alpha - beta;

	inter_color.r = (c[0].r * alpha) + (c[1].r * beta) + (c[2].r * gamma);
	inter_color.g = (c[0].g * alpha) + (c[1].g * beta) + (c[2].g * gamma);
	inter_color.b = (c[0].b * alpha) + (c[1].b * beta) + (c[2].b * gamma);

	return inter_color;
}

float Triangle::interpolateZ(glm::vec3 screen[3], int x, int y)
{
	float alpha, beta, gamma;
	float z_val;
	float Xa, Xb, Xc, Ya, Yb, Yc;
	Xa = screen[0].x;
	Xb = screen[1].x;
	Xc = screen[2].x;
	Ya = screen[0].y;
	Yb = screen[1].y;
	Yc = screen[2].y;
	float neg = -1.0f;

	alpha = (neg*(x - Xb)*(Yc - Yb) + (y - Yb)*(Xc - Xb)) /
		(neg*(Xa - Xb)*(Yc - Yb) + (Ya - Yb)*(Xc - Xb));

	beta = (neg*(x - Xc)*(Ya - Yc) + (y - Yc)*(Xa - Xc)) /
		(neg*(Xb - Xc)*(Ya - Yc) + (Yb - Yc)*(Xa - Xc));

	gamma = 1.0f - alpha - beta;

	z_val = (alpha * screen[0].z) + (beta * screen[1].z) + (gamma * screen[2].z);

	return z_val;
}

bool Triangle::inside(glm::vec3 screen[3], int x, int y)
{
	bool inside;
	float line1, line2, line3;


	line1 = ((-1)*(screen[1].y - screen[0].y)*x) + ((screen[1].x - screen[0].x)*y) + (screen[0].x*screen[1].y - screen[1].x*screen[0].y);
	line2 = ((-1)*(screen[2].y - screen[1].y)*x) + ((screen[2].x - screen[1].x)*y) + (screen[1].x*screen[2].y - screen[2].x*screen[1].y);
	line3 = ((-1)*(screen[0].y - screen[2].y)*x) + ((screen[0].x - screen[2].x)*y) + (screen[2].x*screen[0].y - screen[0].x*screen[2].y);

	if (line1 >= 0 && line2 >= 0 && line3 >= 0) {
		inside = true;
	}
	else {
		inside = false;
	}

	return inside;
}

// Rendering the triangle using OpenGL
void Triangle::RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix) 
{

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(modelViewMatrix));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));

	glBegin(GL_TRIANGLES);
		glColor3f(c[0].x, c[0].y, c[0].z);
		glVertex3f(v[0].x, v[0].y, v[0].z);
		glColor3f(c[1].x, c[1].y, c[1].z);
		glVertex3f(v[1].x, v[1].y, v[1].z);
		glColor3f(c[2].x, c[2].y, c[2].z);
		glVertex3f(v[2].x, v[2].y, v[2].z);
	glEnd();
}

// Render the triangle on CPU
void Triangle::RenderCPU(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, glm::mat4 &viewportMatrix, float color[WINDOW_HEIGHT][WINDOW_WIDTH][3], float depth[WINDOW_HEIGHT][WINDOW_WIDTH])
{
	glm::vec4 vert;
	glm::vec3 screen[3];
	glm::vec4 Vclip;
	bool inside_tri;
	float zVal = 0.0f;
	glm::vec3 inter_color;

	for (int i = 0; i < 3; i++) {
		vert[0] = v[i].x;
		vert[1] = v[i].y;
		vert[2] = v[i].z;
		vert[3] = 1.0f;

		Vclip = viewportMatrix * projectionMatrix * modelViewMatrix * vert;

		screen[i].x = Vclip[0] / Vclip[3];
		screen[i].y = Vclip[1] / Vclip[3];
		screen[i].z = Vclip[2] / Vclip[3];


	}

	//calculate bounding box of triangle
	int minx, maxx, miny, maxy;
	minx = screen[0].x;
	maxx = screen[0].x;
	miny = screen[0].y;
	maxy = screen[0].y;

	// Creates bounding box
	for (int i = 1; i < 3; i++) {
		if (screen[i].x < minx) minx = screen[i].x;
		if (screen[i].x > maxx) maxx = screen[i].x;
		if (screen[i].y < miny) miny = screen[i].y;
		if (screen[i].y > maxy) maxy = screen[i].y;
	}

	if (minx < 0) minx = 0;
	if (maxx > WINDOW_WIDTH - 1) maxx = WINDOW_WIDTH - 1;
	if (miny < 0) miny = 0;
	if (maxy > WINDOW_HEIGHT - 1) maxy = WINDOW_HEIGHT - 1;

	//Rasterize triangle
		for (int i = miny; i <= maxy; i++) {
			for (int j = minx; j <= maxx; j++) {
				inside_tri = inside(screen, j, i);
				if (inside_tri == true) {
					zVal = interpolateZ(screen, j, i);
					if (zVal < depth[i][j]) {
						inter_color = interpolateColor(screen, j, i);
						color[i][j][0] = inter_color.r;
						color[i][j][1] = inter_color.g;
						color[i][j][2] = inter_color.b;
						depth[i][j] = zVal;
					}
				}
			}
		}

}

void Triangle::Tri_Color(glm::vec3 color)
{
	c[0] = color;
	c[1] = color;
	c[2] = color;
}

void Triangle::Vert_Color(glm::vec3 c1, glm::vec3 c2, glm::vec3 c3)
{
	c[0] = c1;
	c[1] = c2;
	c[2] = c3;
}

glm::vec3 Triangle::get_Z()
{
	glm::vec3 z;

	z[0] = v[0].z;
	z[1] = v[1].z;
	z[2] = v[2].z;

	return z;
}


