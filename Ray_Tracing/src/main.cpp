#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Camera.h"
#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"


#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1200

float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
GLFWwindow *window;

std::vector<Shape*> shapes;
std::vector<Light*> lights;

void ClearFrameBuffer()
{
	memset(&frameBuffer, 0, WINDOW_HEIGHT * WINDOW_WIDTH * 3 * sizeof(float));
}

void Display()
{	
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

void Init()
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment5 - Rylan Wade", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();

	Scene scene = Scene();

	//build scene
	//Sphere sphere1;
	Shape* shape1 = new Sphere;
	shape1->Shape_color(glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.2f, 1.0f, 0.2f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		100.0f);
	shape1->Sphere_elem(glm::vec3(-1.0f, -0.7f, 3.0f),
		0.3f,
		false);

	//Sphere sphere2;
	Shape* shape2 = new Sphere;
	shape2->Shape_color(glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		10.0f);
	shape2->Sphere_elem(glm::vec3(1.0f, -0.5f, 3.0f),
		0.5f,
		false);

	//Plane plane1;
	Shape* shape3 = new Plane;
	shape3->Shape_color(glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		0.0f);
	shape3->Plane_elem(glm::vec3(0.0f, 0.0f, -3.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		false);

	//Plane 
	Shape* shape4 = new Plane;
	shape4->Shape_color(glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		0.0f);
	shape4->Plane_elem(glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		false);

	//Reflective sphere
	Shape* shape5 = new Sphere;
	shape5->Shape_color(glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		0.0f);
	shape5->Sphere_elem(glm::vec3(-1.0f, 0.0f, 0.0f),
		1.0f,
		true);

	//Reflective sphere
	Shape* shape6 = new Sphere;
	shape6->Shape_color(glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.8f, 0.8f, 0.8f),
		0.0f);
	shape6->Sphere_elem(glm::vec3(1.0f, 0.0f, -1.0f),
		1.0f,
		true);

	shapes.push_back(shape1);
	shapes.push_back(shape6);

	shapes.push_back(shape4);
	shapes.push_back(shape3);

	shapes.push_back(shape2);
	shapes.push_back(shape5);

	Light light1;
	light1.Set_elem(glm::vec3(0.0f, 3.0f, -2.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	Light light2;
	light2.Set_elem(glm::vec3(-2.0f, 1.0f, 4.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	lights.push_back(&light1);
	lights.push_back(&light2);

	Camera camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT);
	camera.Cam_elem(glm::vec3(0.0f, 0.0f, 7.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0, 1.0f, 0.0f),
		45.0f,
		1.0f);

	scene.set_shapes(shapes);
	scene.set_lights(lights);

	camera.TakePicture(&scene);
	float *renderedImage = camera.GetRenderedImage();
	memcpy(frameBuffer, renderedImage, sizeof(float) * WINDOW_HEIGHT * WINDOW_WIDTH * 3);
}


int main()
{	
	Init();
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}