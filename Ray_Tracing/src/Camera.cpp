#include "Camera.h"



Camera::Camera()
{

}

Camera::~Camera()
{
}

Camera::Camera(int widthRes, int heightRes)
{
	this->widthRes = widthRes;
	this->heightRes = heightRes;
	renderedImage = new float[widthRes * heightRes * 3];
}

void Camera::Cam_elem(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up, float fovy, float F)
{
	this->eye = eye;
	this->lookat = lookat;
	this->up = up;
	this->fovy = fovy;
	this->F = F;
}

glm::vec3 Camera::ComputerRayColor(Scene *scene, glm::vec3 origin, glm::vec3 direction, float t0, float t1)
{	
	glm::vec3 c, inter_point, Normal, shadowD, LD, epsilon;
	Record rec;
	Record Srec;
	float T1,S;
	float T0 = 0.0001f;
	glm::vec3 L, R, E, spec, diff, sum, RD, L_pos, Kd, Ks;
	std::vector<Light*> l;
	int numL;

	count++;
	if (scene->Hit(origin, direction, t0, t1, rec)) {
		//determine color
		c = rec.get_ka();
		Kd = rec.get_kd();
		Ks = rec.get_ks();
		S = rec.get_s();
		inter_point = origin + (t1)*direction;
		Normal = rec.get_norm();
		epsilon = inter_point + Normal * T0;
		l = scene->get_lights();
		numL = l.size();

		if (rec.get_r() == true) reflect = true;

		for (int i = 0; i < numL; i++) {
			L_pos = l[i]->get_pos();
			LD = L_pos - inter_point;
			T1 = glm::length(LD);
			shadowD = glm::normalize(LD);
			if (scene->Hit(epsilon, shadowD, T0, T1, Srec) == false) {
				L = glm::normalize(L_pos - inter_point);
				E = glm::normalize(eye - inter_point);
				R = glm::normalize(2.0f*glm::dot(L,Normal)*Normal-L);
				//R = glm::normalize(glm::reflect(L, Normal));

				diff = Kd * glm::max(0.0f, glm::dot(L, Normal));
				spec = Ks * pow(glm::max(0.0f, glm::dot(R, E)),S);
				sum = diff + spec;

				c += l[i]->get_col() * sum;
			}
		}
		//if (count != 4) {
		if(reflect == true && count != 4){
			t1 = INFINITY;
			RD = glm::normalize(glm::reflect(direction,Normal));
			c += rec.get_km() * ComputerRayColor(scene, epsilon, RD, T0, t1);
		}
		return c;
	}
	else {
		//return background color
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}
}

void Camera::TakePicture(Scene *scene)
{
	memset(renderedImage, 0, sizeof(float) * widthRes * heightRes * 3);

	glm::vec3 color;
	int numShape = scene->get_shapes().size();

	for (int i = 0; i < widthRes; i++) {
		for (int j = 0; j < heightRes; j++) {

			float ly = 2 * F*tan(glm::radians(fovy) / 2);
			float lx = ly * (widthRes / float(heightRes));
			glm::vec3 viewDir = normalize(lookat-eye);
			glm::vec3 I_center = eye + viewDir * F;
			glm::vec3 u = normalize(glm::cross(viewDir, up));
			glm::vec3 v = glm::cross(u, viewDir);
			float Pw = ly / heightRes;
			glm::vec3 O = I_center - (lx / 2)*u - (ly / 2)*v;

			glm::vec3 point = O + (j + 0.5f)*Pw*v + (i + 0.5f)*Pw*u;
			glm::vec3 direction = normalize(point - eye);

			float t0 = 0.0f;
			float t1 = INFINITY;

			count = 0;
			reflect = false;
			color = ComputerRayColor(scene, eye, direction, t0, t1);

			renderedImage[(j * widthRes + i) * 3 + 0] = color[0];
			renderedImage[(j * widthRes + i) * 3 + 1] = color[1];
			renderedImage[(j * widthRes + i) * 3 + 2] = color[2];
		}
	}
}
