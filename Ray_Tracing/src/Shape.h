#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Shape.h"
#include "Light.h"

class Shape
{
public:
	Shape(void);
	~Shape(void);

	void Shape_color(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 km, float s);
	void Sphere_elem(glm::vec3 sphere_pos, float radius, bool reflective);
	void Plane_elem(glm::vec3 center, glm::vec3 norm, bool reflective);

	glm::vec3 get_norm() {
		return norm;
	};
	glm::vec3 get_sphere_pos() {
		return sphere_pos;
	};
	float get_radius() {
		return radius;
	};
	glm::vec3 get_center() {
		return center;
	};

	virtual float Intersect(glm::vec3 origin, glm::vec3 direction, float t0, float t1)=0;

	virtual glm::vec3 get_Normal(glm::vec3 point)=0;

	friend class Record;


private:
	glm::vec3 ka, kd, ks, km, sphere_pos, center, norm;
	float s, radius;
	bool reflective;
};

class Record {
public:
	glm::vec3 get_ka();
	glm::vec3 get_kd();
	glm::vec3 get_ks();
	glm::vec3 get_km();
	glm::vec3 get_norm();
	bool get_r();
	void set_norm(glm::vec3 N);
	float get_s();
	void set_Var(Shape& shape);
private:
	glm::vec3 ka, kd, ks, km, norm;
	float s;
	bool reflective;
};
