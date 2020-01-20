#pragma once

#include <stdlib.h>
#include <math.h>
#include <glm/glm.hpp>

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

class Triangle {
	private:
		glm::vec3 v[3];		// Triangle vertices
		glm::vec3 c[3];		// Vertex color

	public:
		// Constructor
		Triangle(glm::vec3 &v0, glm::vec3 &v1, glm::vec3 &v2);

		// Point color by Barycentric coordinates
		glm::vec3 interpolateColor(glm::vec3 screen[3], int x, int y);

		// Calculate pixel z-value
		float interpolateZ(glm::vec3 screen[3], int x, int y);

		// Check if point is inside triangle
		bool inside(glm::vec3 screen[3], int x, int y);

		// Rendering the triangle using OpenGL
		void RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix);

		// Rendering the triangle using CPU
		void RenderCPU(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, glm::mat4 &viewportMatrix, float color[WINDOW_HEIGHT][WINDOW_WIDTH][3], float depth[WINDOW_HEIGHT][WINDOW_WIDTH]);

		// Change triangle color
		void Tri_Color(glm::vec3 color);

		// Change vertex color
		void Vert_Color(glm::vec3 c1, glm::vec3 c2,glm::vec3 c3);

		glm::vec3 get_Z();

};
