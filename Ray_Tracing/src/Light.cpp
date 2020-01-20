#include "Light.h"

Light::Light(void)
{
}

Light::~Light(void)
{
}

void Light::Set_elem(glm::vec3 light_pos, glm::vec3 light_color)
{
	this->light_pos = light_pos;
	this->light_color = light_color;
}

glm::vec3 Light::get_pos()
{
	return light_pos;
}

glm::vec3 Light::get_col()
{
	return light_color;
}

