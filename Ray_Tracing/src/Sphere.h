#pragma once
#include "Shape.h"


class Sphere :
	public Shape
{
public:
	Sphere();
	~Sphere();

	float Intersect(glm::vec3 origin, glm::vec3 direction, float t0, float t1)
	{

		float t;
		glm::vec3 pos = Shape::get_sphere_pos();
		float rad = Shape::get_radius();
		glm::vec3 inter_point;

		float a = glm::dot(direction, direction);
		float b = glm::dot(2.0f * (origin - pos), direction);
		float c = glm::dot(origin - pos, origin - pos) - pow(rad, 2.0f);
		float D = pow(b, 2.0f) - 4.0f * a * c;

		if (D > 0.0f) { //two solutions
			t = (-b - sqrt(D)) / (2.0f * a);
			if (t < 0.0f) {
				t = (-b + sqrt(D)) / (2.0f * a);
			}
			if (t < 0.0f) { //sphere was behind eye
				t = t1;
			}
		}
		else if (D == 0.0f) { //one solution
			t = -b / (2.0f * a);
		}
		else if (D < 0.0f) { //no solution
			t = t1;
		}

		return t;
	}

	glm::vec3 get_Normal(glm::vec3 point) {
		glm::vec3 pos = Shape::get_sphere_pos();
		glm::vec3 N;

		N = glm::normalize(point - pos);

		return N;
	};

private:
	
};
