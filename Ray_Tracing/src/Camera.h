#pragma once
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
#include "Scene.h"



class Camera
{
public:
	Camera();
	~Camera();

	Camera(int widthRes, int heightRes);

	//my code
	void Cam_elem(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up, float fovy, float F);
	glm::vec3 ComputerRayColor(Scene *scene, glm::vec3 origin, glm::vec3 direction, float t0, float t1);

	void TakePicture(Scene *scene);
	float* GetRenderedImage() { return renderedImage; };

private:

	int widthRes;
	int heightRes;
	int count, test;
	bool reflect;

	float *renderedImage;

	glm::vec3 eye, lookat, up;
	float fovy, F;



};
