#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light
{
public:
	Light();
	~Light();

	void Set_elem(glm::vec3 light_pos, glm::vec3 light_color);
	glm::vec3 get_pos();
	glm::vec3 get_col();

private:
	glm::vec3 light_pos, light_color;
};
