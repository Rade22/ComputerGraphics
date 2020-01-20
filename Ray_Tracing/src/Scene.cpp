#include "Scene.h"


Scene::Scene()
{
}

Scene::~Scene()
{
}

std::vector<Shape*> Scene::get_shapes()
{
	return shapes;
}

std::vector<Light*> Scene::get_lights()
{
	return lights;
}

void Scene::set_shapes(std::vector<Shape*> s)
{
	this->shapes = s;
}

void Scene::set_lights(std::vector<Light*> l)
{
	this->lights = l;
}

bool Scene::Hit(glm::vec3 origin, glm::vec3 direction, float t0, float& t1, Record& rec)
{
	bool hit = false;
	glm::vec3 N;
	glm::vec3 point;
	float t;
	int index = 0;
	int numS = shapes.size();

	for (int i = 0; i < numS; i++) {
		t = shapes[i]->Intersect(origin, direction, t0, t1);
		if (t < t1) {
			hit = true;
			t1 = t;
			index = i;
		}
	}

	if (hit == true) {
		rec.set_Var(*shapes[index]);
		point = origin + (t1)*direction;
		N = shapes[index]->get_Normal(point);
		rec.set_norm(N);
	}

	return hit;
}


