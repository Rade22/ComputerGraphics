#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "Triangle.h"


#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

GLFWwindow *window;

struct Buffer
{
	float color[WINDOW_HEIGHT][WINDOW_WIDTH][3];
	float depth[WINDOW_HEIGHT][WINDOW_WIDTH];
}frameBuffer;

std::vector<Triangle> triangleVector;
bool isOpenGL = true;
float eyeDistance = 5.0f;

char* file;

void ClearFrameBuffer()
{
	memset(frameBuffer.color, 0.0f, sizeof(float) * WINDOW_WIDTH * WINDOW_HEIGHT * 3);
}

void Display()
{	
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), float(WINDOW_WIDTH) / float(WINDOW_HEIGHT), 0.1f, 100.0f);
	glm::mat4 modelViewMatrix = glm::lookAt(eyeDistance * glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 viewportMatrix = {
		WINDOW_WIDTH/2,0,0,0,
		0,WINDOW_HEIGHT/2,0,0,
		0,0,1,0,
		WINDOW_WIDTH/2,WINDOW_HEIGHT/2,0,1
	};

	if (isOpenGL)
	{
		for (int i = 0; i < triangleVector.size(); i++)
			triangleVector[i].RenderOpenGL(modelViewMatrix, projectionMatrix);
	}
	else
	{
		// Initializing z-buffer to infinity
		for (int i = 0; i < WINDOW_HEIGHT; i++) {
			for (int j = 0; j < WINDOW_WIDTH; j++) {
				frameBuffer.depth[i][j] = pow(10,30);
				frameBuffer.color[i][j][0] = 0.0f;
				frameBuffer.color[i][j][1] = 0.0f;
				frameBuffer.color[i][j][2] = 0.0f;
			}
		}


		for (int i = 0; i < triangleVector.size(); i++)
			triangleVector[i].RenderCPU(modelViewMatrix,projectionMatrix,viewportMatrix,frameBuffer.color,frameBuffer.depth);

		glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer.color);
	}
}

void mode0() {
	std::vector<Triangle>::iterator it = triangleVector.begin();
	glm::vec3 color;
	
	for (it; it != triangleVector.end(); it++) {
		color[0] = rand() / ((float)RAND_MAX + 1);
		color[1] = rand() / ((float)RAND_MAX + 1);
		color[2] = rand() / ((float)RAND_MAX + 1);

		it->Tri_Color(color);
	}
}

void mode1() {
	std::vector<Triangle>::iterator it = triangleVector.begin();
	glm::vec3 color1;
	glm::vec3 color2;
	glm::vec3 color3;

	for (it; it != triangleVector.end(); it++) {
		color1[0] = rand() / ((float)RAND_MAX + 1);
		color1[1] = rand() / ((float)RAND_MAX + 1);
		color1[2] = rand() / ((float)RAND_MAX + 1);

		color2[0] = rand() / ((float)RAND_MAX + 1);
		color2[1] = rand() / ((float)RAND_MAX + 1);
		color2[2] = rand() / ((float)RAND_MAX + 1);

		color3[0] = rand() / ((float)RAND_MAX + 1);
		color3[1] = rand() / ((float)RAND_MAX + 1);
		color3[2] = rand() / ((float)RAND_MAX + 1);

		it->Vert_Color(color1,color2,color3);
	}
}

void mode2() {
	std::vector<Triangle>::iterator it = triangleVector.begin();
	glm::vec3 color1;
	glm::vec3 color2;
	glm::vec3 color3;
	glm::vec3 z;


	for (it; it != triangleVector.end(); it++) {

		z = it->get_Z();
		for (int i = 0; i < 3; i++) {
			if (z[i] < 0.0f) z[i] = 0.0f;
			if (z[i] > 1.0f) z[i] = 1.0f;
		}
		color1[0] = z[0];
		color1[1] = 0.0f;
		color1[2] = 0.0f;

		color2[0] = z[1];
		color2[1] = 0.0f;
		color2[2] = 0.0f;

		color3[0] = z[2];
		color3[1] = 0.0f;
		color3[2] = 0.0f;

		it->Vert_Color(color1, color2, color3);
	}
}

// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	switch (key)
	{
	case 'w':
		eyeDistance *= (1 - 0.05);
		break;
	case 's':
		eyeDistance *= (1 + 0.05);
		break;
	case ' ':
		isOpenGL = !isOpenGL;
		break;
	case 'q':
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case '0':
		mode0();
		break;
	case '1':
		mode1();
		break;
	case '2':
		mode2();
		break;
	default:
		break;
	}
}

void CreateTriangleVector(std::vector<glm::vec3> &vertices)
{
	for (int i = 0; i < vertices.size() / 3; i++)
	{
		Triangle myTriangle(vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2]);
		triangleVector.push_back(myTriangle);
	}
}

void LoadModel(char* name, std::vector<glm::vec3> &vertices)
{
	// Taken from Shinjiro Sueda with slight modification
	std::string meshName(name);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if (!rc) {
		std::cerr << errStr << std::endl;
	}
	else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					vertices.push_back(glm::vec3(attrib.vertices[3 * idx.vertex_index + 0],
												 attrib.vertices[3 * idx.vertex_index + 1],
												 attrib.vertices[3 * idx.vertex_index + 2]));
				}
				index_offset += fv;
			}
		}
	}
}

void Init()
{

	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment3 - Rylan Wade", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetCharCallback(window, CharacterCallback);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	ClearFrameBuffer();
	std::vector<glm::vec3> vertices;
	LoadModel(file, vertices); //../obj/bunny.obj
	CreateTriangleVector(vertices);
	
}

int main(int argc, char *argv[])
{
	file = argv[1];

	Init();
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}