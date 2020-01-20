#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shape.h"
#include "Light.h"


class Scene
{
public:
	Scene();
	~Scene();

	std::vector<Shape*> get_shapes();
	std::vector<Light*> get_lights();
	void set_shapes(std::vector<Shape*> s);
	void set_lights(std::vector<Light*> l);
	bool Hit(glm::vec3 origin, glm::vec3 direction, float t0, float& t1, Record& rec);

private:
	std::vector<Shape*> shapes;
	std::vector<Light*> lights;

};
