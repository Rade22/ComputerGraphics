#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>


#include "Program.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define NUM_LIGHTS 2
#define NUM_MATERIALS 3

GLFWwindow *window;

Program program1;
Program program2;
Program program3;

std::vector<float> posBuff;
std::vector<float> norBuff;
std::vector<float> texBuff;

glm::vec3 eye(0.0f, 0.0f, 4.0f);


std::vector<Program> program_arr;

int m = 0;
int shader = 0;
int l = 0;

struct materialStruct {
	glm::vec3 ka, kd, ks;
	float s;
} materials[NUM_MATERIALS] = { {{0.2f,0.2f,0.2f},{0.8f,0.7f,0.7f},{1.0f,1.0f,1.0f},10.0f},
							   {{0.0f,0.2f,0.2f},{0.5f,0.7f,0.2f},{0.1f,1.0f,0.1f},100.0f},
							   {{0.2f,0.2f,0.2f},{0.1f,0.3f,0.9f},{0.1f,0.1f,0.1f},1.0f}
};

struct lightStruct {
	glm::vec3 position;
	glm::vec3 color;
} lights[NUM_LIGHTS] = { {{0.0f,0.0f,3.0f},{0.5f,0.5f,0.5f}},
						 { {0.0f,3.0f,0.0f},{0.2f,0.2f,0.2f}} };


void Display()
{

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, -1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 invtanModel(1.0f);
	invtanModel = transpose(inverse(modelMatrix));

	/*if (shader == 1) {
		program_arr[0].Bind();
	}
	else if (shader == 2) {
		program_arr[1].Bind();
	}
	else if (shader == 3) {
		program_arr[2].Bind();
	}*/

	program_arr[shader].Bind();

	program_arr[shader].SendUniformData(modelMatrix, "model");
	program_arr[shader].SendUniformData(viewMatrix, "view");
	program_arr[shader].SendUniformData(projectionMatrix, "projection");
	program_arr[shader].SendUniformData(invtanModel, "invtanModel");
	program_arr[shader].SendUniformData(eye, "eye");

	//send light 1 elements
	program_arr[shader].SendUniformData(lights[0].position, "lights[0].position");
	program_arr[shader].SendUniformData(lights[0].color, "lights[0].color");

	//send light 2 elements
	program_arr[shader].SendUniformData(lights[1].position, "lights[1].position");
	program_arr[shader].SendUniformData(lights[1].color, "lights[1].color");

	if (m == 0) {

		//send materials 1 elements
		program_arr[shader].SendUniformData(materials[0].ka, "ka");
		program_arr[shader].SendUniformData(materials[0].kd, "kd");
		program_arr[shader].SendUniformData(materials[0].ks, "ks");
		program_arr[shader].SendUniformData(materials[0].s, "s");
	}
	else if (m == 1) {

		//send materials 2 elements
		program_arr[shader].SendUniformData(materials[1].ka, "ka");
		program_arr[shader].SendUniformData(materials[1].kd, "kd");
		program_arr[shader].SendUniformData(materials[1].ks, "ks");
		program_arr[shader].SendUniformData(materials[1].s, "s");
	}
	else if (m == 2) {

		//send materials 3 elements
		program_arr[shader].SendUniformData(materials[2].ka, "ka");
		program_arr[shader].SendUniformData(materials[2].kd, "kd");
		program_arr[shader].SendUniformData(materials[2].ks, "ks");
		program_arr[shader].SendUniformData(materials[2].s, "s");
	}

	glDrawArrays(GL_TRIANGLES, 0, posBuff.size() / 3);
	program_arr[shader].Unbind();

}

// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	switch (key) 
	{
	case 'q':
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case 'm':
		std::cout << 'm' << std::endl;
		if (m == 2) {
			m = 0;

		}
		else {
			m++;
		}
		std::cout << m  << std::endl;
		break;
	case 'M':
		std::cout << 'M' << std::endl;
		if (m == 0) {
			m = 2;
		}
		else {
			m--;
		}
		std::cout << m << std::endl;
		break;
	case 'l':
		if (l == 1) {
			l = 0;
		}
		else {
			l++;
		}
		break;
	case 'L':
		if (l == 0) {
			l = 1;
		}
		else {
			l--;
		}
		break;
	case '1':
		shader = 0;
		break;
	case '2':
		shader = 1;
		break;
	case '3':
		shader = 2;
		break;
	case 'x':
		lights[l].position.x = lights[l].position.x + 0.1f;
		break;
	case 'X':
		lights[l].position.x = lights[l].position.x - 0.1f;
		break;
	case 'y':
		lights[l].position.y = lights[l].position.y + 0.1f;
		break;
	case 'Y':
		lights[l].position.y = lights[l].position.y - 0.1f;
		break;
	case 'z':
		lights[l].position.z = lights[l].position.z + 0.1f;
		break;
	case 'Z':
		lights[l].position.z = lights[l].position.z - 0.1f;
		break;
	default:

		break;
	}
}


void FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

void LoadModel(char* name)
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
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
					if (!attrib.normals.empty()) {
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 0]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 1]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 2]);
					}
					if (!attrib.texcoords.empty()) {
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
}

void Init()
{
	program_arr.push_back(program1);
	program_arr.push_back(program2);
	program_arr.push_back(program3);

	glfwInit();
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment4 - Rylan Wade", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetCharCallback(window, CharacterCallback);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	LoadModel("../obj/bunny.obj");
	
		program_arr[0].SetShadersFileName("../shaders/shader1.vert", "../shaders/shader1.frag");
		program_arr[0].Init();
		program_arr[0].SendAttributeData(posBuff, "vPositionModel");
		program_arr[0].SendAttributeData(norBuff, "vNormalModel");

		program_arr[1].SetShadersFileName("../shaders/shader2.vert", "../shaders/shader2.frag");
		program_arr[1].Init();
		program_arr[1].SendAttributeData(posBuff, "vPositionModel");
		program_arr[1].SendAttributeData(norBuff, "vNormalModel");

		program_arr[2].SetShadersFileName("../shaders/shader3.vert", "../shaders/shader3.frag");
		program_arr[2].Init();
		program_arr[2].SendAttributeData(posBuff, "vPositionModel");
		program_arr[2].SendAttributeData(norBuff, "vNormalModel");
}


int main()
{	
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