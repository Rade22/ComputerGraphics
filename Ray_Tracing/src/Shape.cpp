#include "Shape.h"

Shape::Shape(void)
{
}

Shape::~Shape(void)
{
}

void Shape::Shape_color(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, glm::vec3 km, float s)
{
	this->ka = ka;
	this->kd = kd;
	this->ks = ks;
	this->km = km;
	this->s = s;
}

void Shape::Sphere_elem(glm::vec3 sphere_pos, float radius, bool reflective)
{
	this->sphere_pos = sphere_pos;
	this->radius = radius;
	this->reflective = reflective;
}

void Shape::Plane_elem(glm::vec3 center, glm::vec3 norm, bool reflective)
{
	this->center = center;
	this->norm = norm;
	this->reflective = reflective;
}

glm::vec3 Record::get_ka()
{
	return ka;
}

glm::vec3 Record::get_kd()
{
	return kd;
}

glm::vec3 Record::get_ks()
{
	return ks;
}

glm::vec3 Record::get_km()
{
	return km;
}

glm::vec3 Record::get_norm()
{
	return norm;
}

void Record::set_norm(glm::vec3 N)
{
	this->norm = N;
}

float Record::get_s()
{
	return s;
}

bool Record::get_r() {
	return reflective;
}

void Record::set_Var(Shape& shape)
{
	this->ka = shape.ka;
	this->kd = shape.kd;
	this->ks = shape.ks;
	this->km = shape.km;
	this->s = shape.s;
	this->reflective = shape.reflective;
}
