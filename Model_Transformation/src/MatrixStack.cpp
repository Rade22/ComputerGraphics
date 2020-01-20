#include "MatrixStack.h"

#include <stdio.h>
#include <cassert>
#include <vector>
#include <math.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

MatrixStack::MatrixStack()
{
	mstack = make_shared< stack<glm::mat4> >();
	mstack->push(glm::mat4(1.0));
}

MatrixStack::~MatrixStack()
{
}

void MatrixStack::pushMatrix()
{
	const glm::mat4 &top = mstack->top();
	mstack->push(top);
	assert(mstack->size() < 100);
}

void MatrixStack::popMatrix()
{
	assert(!mstack->empty());
	mstack->pop();
	// There should always be one matrix left.
	assert(!mstack->empty());
}

void MatrixStack::loadIdentity()
{
	glm::mat4 &top = mstack->top();
	top = glm::mat4(1.0);
}

void MatrixStack::translate(const glm::vec3 &t)
{
	float A[16] = {1,0,0,0,
				   0,1,0,0,
				   0,0,1,0,
				   t[0],t[1],t[2],1};
	
	glm::mat4 translationMatrix(1.0f);

	// Need to comment out the following line and write your own version
	//translationMatrix = glm::translate(glm::mat4(1.0f), t);

	translationMatrix = glm::make_mat4(A);


	multMatrix(translationMatrix);

}

void MatrixStack::scale(const glm::vec3 &s)
{

	float A[16] = { s[0],0,0,0,
			        0,s[1],0,0,
			        0,0,s[2],0,
			        0,0,0,1 };
					
	glm::mat4 scaleMatrix(1.0f);

	// Need to comment out the following line and write your own version
	//scaleMatrix = glm::scale(glm::mat4(1.0f), s);

	scaleMatrix = glm::make_mat4(A);

	multMatrix(scaleMatrix);
}

void MatrixStack::rotateX(float angle)
{
	float A[16] = { 1,0,0,0,
					0,cos(angle),sin(angle),0,
					0,-sin(angle),cos(angle),0,
					0,0,0,1 };

	glm::mat4 rotationMatrix(1.0f);

	// Need to comment out the following line and write your own version
	//rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1.0f, 0.0f, 0.0f));

	rotationMatrix = glm::make_mat4(A);
	multMatrix(rotationMatrix);
}

void MatrixStack::rotateY(float angle)
{

	float A[16] = { cos(angle),0,-sin(angle),0,
					0,1,0,0,
					sin(angle),0,cos(angle),0,
					0,0,0,1 };
	glm::mat4 rotationMatrix(1.0f);

	// Need to comment out the following line and write your own version
	//rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));

	rotationMatrix = glm::make_mat4(A);
	multMatrix(rotationMatrix);
}

void MatrixStack::rotateZ(float angle)
{

	float A[16] = { cos(angle),sin(angle),0,0,
					-sin(angle),cos(angle),0,0,
					0,0,1,0,
					0,0,0,1 };
	glm::mat4 rotationMatrix(1.0f);

	// Need to comment out the following line and write your own version
	//rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));

	rotationMatrix = glm::make_mat4(A);
	multMatrix(rotationMatrix);
}

void MatrixStack::multMatrix(glm::mat4 &matrix)
{
	glm::mat4 &top = mstack->top();

	glm::mat4 MMatrix(1.0f);

	// Need to comment out the following line and write your own version
	//top *= matrix;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			MMatrix[i][j] = 0;
		}
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			//MMatrix[i][j] = 0;
			for (int k = 0; k < 4; k++) {
				MMatrix[j][i] += top[k][i] * matrix[j][k];
			}
		}
	}

	top = MMatrix;

	// Getting a pointer to the glm::mat4 matrix
	float* topArray = glm::value_ptr(top);
	float* matrixArray = glm::value_ptr(matrix);
}

void MatrixStack::Perspective(float fovy, float aspect, float near, float far)
{
	float d = 1 / (tan(fovy / 2));
	float a = d / aspect;
	float b = -(far + near) / (far - near);
	float c = -(2 * far*near) / (far - near);

	float A[16] = { a, 0, 0, 0,
			 0, d, 0, 0,
			 0, 0, b, -1,
			 0, 0, c, 0 };

	glm::mat4 projectionMatrix(0.0f);

	// Need to comment out the following line and write your own version
	//projectionMatrix = glm::perspective(fovy, aspect, near, far);

	projectionMatrix = glm::make_mat4(A);

	multMatrix(projectionMatrix);
}

void MatrixStack::LookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	glm::vec3 D = glm::normalize(eye - center);
	glm::vec3 R = glm::normalize(glm::cross(up, D));
	glm::vec3 U = glm::cross(D, R);

	float A[16] = { R[0],U[0],D[0],0,
					R[1],U[1],D[1],0,
					R[2],U[2],D[2],0,
					0,0,0,1 };

	float B[16] = { 1,0,0,0,
				   0,1,0,0,
				   0,0,1,0,
				   -eye[0],-eye[1],-eye[2],1 };

	glm::mat4 viewMatrix(1.0f);

	// Need to comment out the following line and write your own version
	//viewMatrix = glm::lookAt(eye, center, up);

	viewMatrix = glm::make_mat4(A) * glm::make_mat4(B);

	multMatrix(viewMatrix);
}


void MatrixStack::translate(float x, float y, float z)
{
	translate(glm::vec3(x, y, z));
}

void MatrixStack::scale(float x, float y, float z)
{
	scale(glm::vec3(x, y, z));
}

void MatrixStack::scale(float s)
{
	scale(glm::vec3(s, s, s));
}

glm::mat4 &MatrixStack::topMatrix()
{
	return mstack->top();
}

void MatrixStack::print(const glm::mat4 &mat, const char *name)
{
	if(name) {
		printf("%s = [\n", name);
	}
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			// mat[j] returns the jth column
			printf("%- 5.2f ", mat[j][i]);
		}
		printf("\n");
	}
	if(name) {
		printf("];");
	}
	printf("\n");
}

void MatrixStack::print(const char *name) const
{
	print(mstack->top(), name);
}
