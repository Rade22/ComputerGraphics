#pragma once
#include "Shape.h"

class Plane :
	public Shape
{
public:
	Plane();
	~Plane();

	float Intersect(glm::vec3 origin, glm::vec3 direction, float t0, float t1) {
		float t;
		glm::vec3 newO = origin + (t0)*direction;
		glm::vec3 cent = Shape::get_center();
		glm::vec3 Norm = Shape::get_norm();

		

		t = (glm::dot((cent - newO), Norm)) / (glm::dot(direction, Norm));

		if (t < 0.0f) {
			t = INFINITY;
		}

		return t;
	}

	glm::vec3 get_Normal(glm::vec3 point) {
		glm::vec3 N = Shape::get_norm();
		return N;
	};


private:
};
