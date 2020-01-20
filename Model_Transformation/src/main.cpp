#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include <iterator>
#include "MatrixStack.h"
#include "Program.h"
#include "math.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

/*int it = 0;
int end;*/


glm::vec4 homo_view, homo_up, new_view, new_up;

int down = 0;
int rot = 0;
int down2 = 0;
int size = 0;
int iteration = 0;

float fovy = 60.f;

//float thetax = 0.0f;
//float thetay = 0.0f;
float newx;
float newy;
float newz;
bool vert, horz;

float theta = 0.0f;
float phi = 0.0f;


int int_x;
int int_y;
int intx;
int inty;
int deltax;
int deltay;
int delta_x;
int delta_y;



GLFWwindow *window;
double currentXpos, currentYpos;
glm::vec3 eye(0.0f, 0.0f, 8.0f);
glm::vec3 center(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);

glm::vec3 view = eye - center;
glm::vec3 right = glm::cross(up, view);
glm::vec3 Up = up;
glm::vec3 new_eye = eye;

float distx = eye[0] - center[0];
float disty = eye[1] - center[1];
float distz = eye[2] - center[2];
float rad = pow(pow(distx, 2) + pow(disty, 2) + pow(distz, 2), 0.5);

Program program;
MatrixStack modelViewProjectionMatrix;

// Draw cube on screen
void DrawCube(glm::mat4& modelViewProjectionMatrix)
{
	program.SendUniformData(modelViewProjectionMatrix, "mvp");
	glDrawArrays(GL_TRIANGLES, 0, 36);
}



class RobotElements
{
private:
	//std::vector<RobotElements*> children;
	glm::vec3 scale;
	glm::vec3 parent_joint_t;
	glm::vec3 joint_t;
	glm::vec3 rot;
	glm::vec3 parent_rot;
	//RobotElements* root = NULL;
	RobotElements* next = NULL;
	RobotElements* prev = NULL;
	RobotElements* child = NULL;
	RobotElements* parent = NULL;
public:
	RobotElements() {}
	void set_parent_rot(float x, float y, float z) {
		parent_rot[0] = x;
		parent_rot[1] = y;
		parent_rot[2] = z;
	}
	glm::vec3 get_parent_rot() {
		return parent_rot;
	}
	void set_next(RobotElements* nxt) {
		next = nxt;
	}
	RobotElements* get_next() {
		return next;
	}
	void set_prev(RobotElements* prv) {
		prev = prv;
	}
	RobotElements* get_prev() {
		return prev;
	}
	void set_child(RobotElements* chd) {
		child = chd;
	}
	RobotElements* get_child() {
		return child;
	}
	void set_parent(RobotElements* prnt) {
		parent = prnt;
	}
	RobotElements* get_parent() {
		return parent;
	}
	/*void Set_children(std::vector<RobotElements*> &vec){
		children = vec;
	}*/
	void set_scale(float s) {
		scale = glm::vec3(s, s, s);
	}
	void set_rotation(float x, float y, float z) {
		rot = glm::vec3(x, y, z);
	}
	void set_Scale(float x, float y, float z) {
		scale = glm::vec3(x, y, z);
	}
	void set_joint_translate(float x, float y, float z) {
		joint_t = glm::vec3(x, y, z);
	}
	void set_parent_joint_translate(float x, float y, float z) {
		parent_joint_t = glm::vec3(x, y, z);
	}
	/*void set_root(RobotElements r) {
		root = &r;
	}
	RobotElements* get_root() {
		return root;
	}*/
	/*void set_children(std::vector<RobotElements*> child) {
		children = child;
	}*/
	/*std::vector<RobotElements*> get_children() {
		return children;
	}*/
	glm::vec3 get_scaling() {
		return scale;
	}
	glm::vec3 get_joint_translate() {
		return joint_t;
	}
	glm::vec3 get_parent_joint_translate() {
		return parent_joint_t;
	}
	glm::vec3 get_rotation() {
		return rot;
	}
	~RobotElements() {}
};

RobotElements torso;

RobotElements head;

RobotElements upper_la;
RobotElements upper_ra;
RobotElements upper_ll;
RobotElements upper_rl;

RobotElements lower_la;
RobotElements lower_ra;
RobotElements lower_ll;
RobotElements lower_rl;

RobotElements* top;
RobotElements* top2;
RobotElements* curr;
RobotElements* child;

void ConstructRobot()
{
	
	//std::vector< RobotElements*> child;
	
	torso.set_Scale(1.0f, 1.5f, 1.0f);
	torso.set_rotation(0.0f, 0.0f, 0.0f);
	size++;
	
	/*child.push_back(&head);
	child.push_back(&upper_la);
	child.push_back(&upper_ra);
	child.push_back(&upper_rl);
	child.push_back(&upper_ll);
	torso.Set_children(child);*/

	/*child.pop_back();
	child.pop_back();
	child.pop_back();
	child.pop_back();
	child.pop_back();*/


	head.set_scale(0.6f);
	head.set_joint_translate(0.0f, 0.6f, 0.0f);
	head.set_rotation(0.0f, 0.0f, 0.0f);
	head.set_parent_joint_translate(0.0f, 1.3f, 0.0f);

	size++;

	upper_la.set_Scale(0.8f, 0.25f, 0.25f);
	upper_la.set_joint_translate(0.8f, 0.0f, 0.0f);
	upper_la.set_rotation(0.0f, 0.0f, 0.0f);
	upper_la.set_parent_joint_translate(1.0f, 1.0f, 0.0f);
	/*child.push_back(&lower_la);
	upper_la.Set_children(child);
	child.pop_back();*/
	
	size++;

	lower_la.set_Scale(0.8f, 0.15f, 0.15f);
	lower_la.set_joint_translate(0.8f, 0.0f, 0.0f);
	lower_la.set_rotation(0.0f, 0.0f, 0.0f);
	lower_la.set_parent_joint_translate(1.6f, 0.0f, 0.0f);
	
	size++;

	upper_ra.set_Scale(0.8, 0.25, 0.25);
	upper_ra.set_joint_translate(-0.8f, 0.0f, 0.0f);
	upper_ra.set_rotation(0.0f, 0.0f, 0.0f);
	upper_ra.set_parent_joint_translate(-1.0f,1.0f,0.0f);
	/*child.push_back(&lower_ra);
	upper_ra.Set_children(child);
	child.pop_back();*/
	
	size++;

	lower_ra.set_Scale(0.8, 0.15, 0.15);
	lower_ra.set_joint_translate(-0.8f, 0.0f, 0.0f);
	lower_ra.set_rotation(0.0f, 0.0f, 0.0f);
	lower_ra.set_parent_joint_translate(-1.6f, 0.0f, 0.0f);
	
	size++;
	
	upper_rl.set_Scale(0.25, 0.8, 0.25);
	upper_rl.set_joint_translate(0.0f, -0.8f, 0.0f);
	upper_rl.set_rotation(0.0f, 0.0f, 0.0f);
	upper_rl.set_parent_joint_translate(-0.5f, -1.4f, 0.0f);
	/*child.push_back(&lower_rl);
	upper_rl.Set_children(child);
	child.pop_back();*/
	
	size++;

	lower_rl.set_Scale(0.15f, 0.8f, 0.15f);
	lower_rl.set_joint_translate(0.0f, -0.8f, 0.0f);
	lower_rl.set_rotation(0.0f, 0.0f, 0.0f);
	lower_rl.set_parent_joint_translate(0.0f, -1.6f, 0.0f);
	
	size++;

	upper_ll.set_Scale(0.25, 0.8, 0.25);
	upper_ll.set_joint_translate(0.0f, -0.8f, 0.0f);
	upper_ll.set_rotation(0.0f, 0.0f, 0.0f);
	upper_ll.set_parent_joint_translate(0.5f, -1.4f, 0.0f);
	/*upper_ll.Set_children(child);
	child.pop_back();*/
	
	size++;

	lower_ll.set_Scale(0.15f, 0.8f, 0.15f);
	lower_ll.set_joint_translate(0.0f, -0.8f, 0.0f);
	lower_ll.set_rotation(0.0f, 0.0f, 0.0f);
	lower_ll.set_parent_joint_translate(0.0f, -1.6f, 0.0f);
	
	size++;

	//Construct tree

	head.set_prev(&torso);
	torso.set_next(&head);

	upper_la.set_prev(&head);
	head.set_next(&upper_la);

	upper_la.set_child(&lower_la);
	lower_la.set_parent(&upper_la);

	upper_ra.set_prev(&upper_la);
	upper_la.set_next(&upper_ra);

	lower_ra.set_parent(&upper_ra);
	upper_ra.set_child(&lower_ra);

	upper_rl.set_prev(&upper_ra);
	upper_ra.set_next(&upper_rl);

	lower_rl.set_parent(&upper_rl);
	upper_rl.set_child(&lower_rl);

	upper_ll.set_prev(&upper_rl);
	upper_rl.set_next(&upper_ll);

	lower_ll.set_parent(&upper_ll);
	upper_ll.set_child(&lower_ll);

	upper_ll.set_next(&torso);
	torso.set_prev(&upper_ll);

	curr = &head;
	//end = torso.get_children().size() - 1;
}


/*void change_Zrot(RobotElements root, float new_rot) {
	float currx = root.get_children()[it]->get_rotation()[0];
	float curry = root.get_children()[it]->get_rotation()[1];
	float currz = root.get_children()[it]->get_rotation()[2];
	root.get_children()[it]->set_rotation(currx,curry,currz+new_rot);
}

void change_Yrot(RobotElements root, float new_rot) {
	float currx = root.get_children()[it]->get_rotation()[0];
	float curry = root.get_children()[it]->get_rotation()[1];
	float currz = root.get_children()[it]->get_rotation()[2];
	root.get_children()[it]->set_rotation(currx, curry + new_rot, currz);
}

void change_Xrot(RobotElements root, float new_rot) {
	float currx = root.get_children()[it]->get_rotation()[0];
	float curry = root.get_children()[it]->get_rotation()[1];
	float currz = root.get_children()[it]->get_rotation()[2];
	root.get_children()[it]->set_rotation(currx+new_rot, curry, currz);
}*/

void DrawRobot(RobotElements root) {
	iteration++;
	//std::vector<RobotElements*> child = root.get_children();
	glm::vec3 rotation = root.get_rotation();
	glm::vec3 parent_rotation = root.get_parent_rot();
	//draw current component
	modelViewProjectionMatrix.pushMatrix();
	modelViewProjectionMatrix.translate(root.get_parent_joint_translate());
	modelViewProjectionMatrix.pushMatrix();

	modelViewProjectionMatrix.rotateX(rotation[0]);
	modelViewProjectionMatrix.rotateY(rotation[1]);
	modelViewProjectionMatrix.rotateZ(rotation[2]);

	modelViewProjectionMatrix.pushMatrix();
	modelViewProjectionMatrix.translate(root.get_joint_translate());
	modelViewProjectionMatrix.scale(root.get_scaling());
	DrawCube(modelViewProjectionMatrix.topMatrix());
	modelViewProjectionMatrix.popMatrix();

	if (iteration == size) {
		modelViewProjectionMatrix.popMatrix();
		modelViewProjectionMatrix.popMatrix();
		modelViewProjectionMatrix.popMatrix();
		modelViewProjectionMatrix.popMatrix();
		iteration = 0;
		return;
	}
	else {
		if (root.get_child() != NULL) {
			if (down == 0) {
				down = 1;
				top = &root;
			}
			DrawRobot(*root.get_child());
		}
		else if (root.get_child() == NULL && root.get_next() != NULL) {
			modelViewProjectionMatrix.popMatrix();
			modelViewProjectionMatrix.popMatrix();
			DrawRobot(*root.get_next());
		}
		else if (root.get_child() == NULL && root.get_next() == NULL) {
			down = 0;
			modelViewProjectionMatrix.popMatrix();
			modelViewProjectionMatrix.popMatrix();
			modelViewProjectionMatrix.popMatrix();
			modelViewProjectionMatrix.popMatrix();
			DrawRobot(*top->get_next());
		}
		down = 0;
	}
	/*if (root.get_child() == NULL) {
		return;
	}
	else {
		for (int i = 0; i < child.size(); i++) {
			DrawRobot(*child[i]);
		}
	}*/

	/*if (iteration == size) {
		modelViewProjectionMatrix.popMatrix();
		iteration = 0;
		return;
	}
	else {
		if (root.get_child() != NULL) {
			if (down == 0) {
				down = 1;
				top = &root;
			}
			DrawRobot(*root.get_child());
		}
		else if (root.get_child() == NULL && root.get_next() != NULL) {
			modelViewProjectionMatrix.popMatrix();
			DrawRobot(*root.get_next());
		}
		else if (root.get_child() == NULL && root.get_next()== NULL) {
			modelViewProjectionMatrix.popMatrix();
			down = 0;
			DrawRobot(*top->get_next());
		}
		down = 0;
	}*/


	/*if (iteration == size) {
		iteration = 0;
		return;
	}
	else {
		if (root.get_child() != NULL) {
			if (down == 0) {
				down = 1;
				top = &root;
			}
			DrawRobot(*root.get_child());
		}
		else if (root.get_child() == NULL && root.get_next() != NULL) {
			DrawRobot(*root.get_next());
		}
		else if (root.get_child() == NULL && root.get_next() == NULL) {
			down = 0;
			DrawRobot(*top->get_next());
		}
	}
	down = 0;*/
}



void Display()
{	
	program.Bind();

	modelViewProjectionMatrix.loadIdentity();
	modelViewProjectionMatrix.pushMatrix();

	// Setting the view and Projection matrices
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	modelViewProjectionMatrix.Perspective(glm::radians(fovy), float(width) / float(height), 0.1f, 100.0f);
	modelViewProjectionMatrix.LookAt(eye = center + view, center, up = Up);

	DrawRobot(torso);
	
	// Model transformation for Cube 1
	/*modelViewProjectionMatrix.pushMatrix();
	modelViewProjectionMatrix.translate(-2.0f, 2.0f, 0.0f);
	modelViewProjectionMatrix.rotateY(glm::radians(45.0f));
	modelViewProjectionMatrix.scale(0.8);
	DrawCube(modelViewProjectionMatrix.topMatrix());
	modelViewProjectionMatrix.popMatrix();*/
	
	// Model transformation for Cube 2
	/*modelViewProjectionMatrix.pushMatrix();
	modelViewProjectionMatrix.translate(-2.0f, -2.0f, 0.0f);
	modelViewProjectionMatrix.rotateX(glm::radians(45.0f));
	modelViewProjectionMatrix.scale(0.8);
	DrawCube(modelViewProjectionMatrix.topMatrix());
	modelViewProjectionMatrix.popMatrix();
	
	// Model transformation for Cube 3
	modelViewProjectionMatrix.pushMatrix();
	modelViewProjectionMatrix.translate(2.0f, 0.0f, 0.0f);
	modelViewProjectionMatrix.rotateZ(glm::radians(45.0f));
	modelViewProjectionMatrix.scale(0.8);
	DrawCube(modelViewProjectionMatrix.topMatrix());
	modelViewProjectionMatrix.popMatrix();*/

	modelViewProjectionMatrix.popMatrix();


	program.Unbind();
	
}

// Mouse callback function
void MouseCallback(GLFWwindow* lWindow, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && GLFW_PRESS == action)
		std::cout << "Mouse left button is pressed." << std::endl;
}

// Mouse position callback function
void CursorPositionCallback(GLFWwindow* lWindow, double xpos, double ypos)
{

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int state2 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	if (state != GLFW_PRESS) {
		intx = xpos;
		inty = ypos;
		vert = false;
		horz = false;
		theta = 0.0;
		phi = 0.0;
	}

	if (state == GLFW_PRESS) {
		view = (eye - center);
		right = glm::cross(up, view);
		deltax = abs(int_x - xpos);
		deltay = abs(int_y - ypos);

		/*if (xpos > intx && deltax > deltay) {
			theta = 0.001;
		}
		else if (xpos < intx && deltax > deltay) {
			theta = -0.001;
		}
		else if (ypos > inty && deltay > deltax) {
			phi = -0.001;
		}
		else if (ypos < inty && deltay > deltax) {
			phi = 0.001;
		}

		new_eye[0] = center[0] + rad * cos(theta)*sin(phi);
		new_eye[1] = center[1] + rad * sin(theta)*sin(phi);
		new_eye[2] = center[2] + rad * cos(theta);

		eye = new_eye;*/

		if (xpos > intx && deltax > deltay && vert == false) {
			horz = true;
			phi = 0.0f;
			theta = 0.01f;
			std::cout << theta << std::endl;
		}
		else if (xpos < intx && deltax > deltay && vert == false) {
			horz = true;
			phi = 0.0f;
			theta = -0.01f;
			std::cout << theta << std::endl;
		}
		else if (ypos > inty && deltay > deltax && horz == false) {
			vert = true;
			theta = 0.0f;
			std::cout << theta << std::endl;
			phi = -0.01f;
		}
		else if (ypos < inty && deltay > deltax && horz == false) {
			vert = true;
			theta = 0.0f;
			std::cout << theta << std::endl;
			phi = 0.01f;
		}

		glm::mat4 mat1(1.0);
		glm::mat4 mat2(1.0);
		glm::mat4 res(1.0);

		homo_view[0] = view[0];
		homo_view[1] = view[1];
		homo_view[2] = view[2];
		homo_view[3] = 0.0;

		homo_up[0] = up[0];
		homo_up[1] = up[1];
		homo_up[2] = up[2];
		homo_up[3] = 0.0;

		mat1 = glm::rotate(mat1,theta, up);
		mat2 = glm::rotate(mat2,phi, right);

		new_view = mat1*mat2 * homo_view;
		new_up = mat1*mat2 * homo_up;

		view[0] = new_view[0];
		view[1] = new_view[1];
		view[2] = new_view[2];

		Up[0] = new_up[0];
		Up[1] = new_up[1];
		Up[2] = new_up[2];


		
	}

	if (state2 != GLFW_PRESS) {
		int_y = ypos;
		int_x = xpos;
		//std::cout << int_y << std::endl;
	}

	if (state2 == GLFW_PRESS) {
		delta_x = abs(int_x - xpos);
		delta_y = abs(int_y - ypos);
		//std::cout << "Mouse position is: x - " << xpos << ", y - " << ypos << std::endl;
		float ey, cy, uy, ex, cx, ux;

		if (ypos < int_y && delta_y > delta_x) {
			ey = eye[1];
			//uy = up[1];
			cy = center[1];

			eye[1] = ey + 0.02;
			center[1] = cy + 0.02;
			//up[1] = uy + 0.02;
		}

		if (ypos > int_y && delta_y > delta_x) {
			ey = eye[1];
			//uy = up[1];
			cy = center[1];

			eye[1] = ey - 0.02;
			center[1] = cy - 0.02;
			//up[1] = uy - 0.02;
		}
		
		if (xpos > int_x && delta_x > delta_y) {
			ex = eye[0];
			//ux = up[0];
			cx = center[0];

			eye[0] = ex + 0.02;
			center[0] = cx + 0.02;
			//up[0] = ux + 0.02;
		}
		
		if (xpos < int_x && delta_x > delta_y) {
			ex = eye[0];
			//ux = up[0];
			cx = center[0];

			eye[0] = ex - 0.02;
			center[0] = cx - 0.02;
			//up[0] = ux - 0.02;
		}
		
		/*float ey, cy, uy, ex, cx, ux;
		
		if (ypos < centy) {
			ey = eye[1];
			uy = up[1];
			cy = center[1];
			
			eye[1] = ey + 0.02;
			center[1] = cy + 0.02;
			up[1] = uy + 0.02;
		}
		else if (ypos > centy) {
			ey = eye[1];
			uy = up[1];
			cy = center[1];

			eye[1] = ey - 0.02;
			center[1] = cy - 0.02;
			up[1] = uy - 0.02;
		}
		else if (xpos > 400) {
			ex = eye[0];
			ux = up[0];
			cx = center[0];

			eye[0] = ex + 0.02;
			center[0] = cx + 0.02;
			up[0] = ux + 0.02;
		}*/
	}
}

void scroll_callback(GLFWwindow* lWindow, double xoffset, double yoffset) {
	if (fovy >= 1.0f && fovy <= 60.0f)
		fovy -= yoffset;
	if (fovy <= 1.0f)
		fovy = 1.0f;
	if (fovy >= 60.0f)
		fovy = 60.0f;
}


// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	std::cout << "Key " << (char)key << " is pressed." << std::endl;

	
	if ((char)key == '.') {
		//traverse forward
		float x, y, z;
		glm::vec3 s = curr->get_scaling();
		x = s[0];
		y = s[1];
		z = s[2];
		if (curr->get_child() == NULL && curr->get_next() != NULL) {
			curr->set_Scale(x - 0.1, y - 0.1, z - 0.1);
			curr = curr->get_next();
			s = curr->get_scaling();
			x = s[0];
			y = s[1];
			z = s[2];
			curr->set_Scale(x + 0.1, y + 0.1, z + 0.1);
		}
		else if (curr->get_child() != NULL) {
			curr->set_Scale(x - 0.1, y - 0.1, z - 0.1);
			if (down2 == 0) {
				down2 = 1;
				top2 = curr;
			}
			curr = curr->get_child();
			s = curr->get_scaling();
			x = s[0];
			y = s[1];
			z = s[2];
			curr->set_Scale(x + 0.1, y + 0.1, z + 0.1);
		}
		else if (curr->get_child() == NULL && curr->get_next() == NULL) {
			down2 = 0;
			curr->set_Scale(x - 0.1, y - 0.1, z - 0.1);
			curr = top2->get_next();
			s = curr->get_scaling();
			x = s[0];
			y = s[1];
			z = s[2];
			curr->set_Scale(x + 0.1, y + 0.1, z + 0.1);
		}
	}
	else if ((char)key == ',') {
		//traverse backward
		float x, y, z;
		glm::vec3 s = curr->get_scaling();
		x = s[0];
		y = s[1];
		z = s[2];
		if (curr->get_child() == NULL && curr->get_prev() != NULL) {
			curr->set_Scale(x - 0.1, y - 0.1, z - 0.1);
			curr = curr->get_prev();
			s = curr->get_scaling();
			x = s[0];
			y = s[1];
			z = s[2];
			curr->set_Scale(x + 0.1, y + 0.1, z + 0.1);
		}
		else if (curr->get_child() != NULL) {
			curr->set_Scale(x - 0.1, y - 0.1, z - 0.1);
			if (down2 == 0) {
				down2 = 1;
				top2 = curr;
			}
			curr = curr->get_child();
			s = curr->get_scaling();
			x = s[0];
			y = s[1];
			z = s[2];
			curr->set_Scale(x + 0.1, y + 0.1, z + 0.1);
		}
		else if (curr->get_child() == NULL && curr->get_prev() == NULL) {
			down2 = 0;
			curr->set_Scale(x - 0.1, y - 0.1, z - 0.1);
			curr = top2->get_prev();
			s = curr->get_scaling();
			x = s[0];
			y = s[1];
			z = s[2];
			curr->set_Scale(x + 0.1, y + 0.1, z + 0.1);
		}
	}
	else if ((char)key == 'x') {
		//increment x
		//change_Xrot(torso, 0.1);
		float x, y, z;
		glm::vec3 r = curr->get_rotation();
		x = r[0];
		y = r[1];
		z = r[2];
		if (curr == &torso) {
			//todo
		}
		else if (curr->get_child() == NULL){
			curr->set_rotation(x + 0.1, y, z);
		}
		else if (curr->get_child() != NULL) {
			curr->set_rotation(x + 0.1, y, z);
			do {
				child = curr->get_child();
				r = child->get_parent_rot();
				x = r[0];
				y = r[1];
				z = r[2];
				child->set_parent_rot(x + 0.1, y, z);
			} while (child->get_child() != NULL);
		}
	}
	else if ((char)key == 'X') {
		//decrement x
		//change_Xrot(torso, -0.1);
		float x, y, z;
		glm::vec3 r = curr->get_rotation();
		x = r[0];
		y = r[1];
		z = r[2];
		if (curr == &torso) {
			//todo
		}
		else if (curr->get_child() == NULL) {
			curr->set_rotation(x - 0.1, y, z);
		}
		else if (curr->get_child() != NULL) {
			curr->set_rotation(x - 0.1, y, z);
			do {
				child = curr->get_child();
				r = child->get_parent_rot();
				x = r[0];
				y = r[1];
				z = r[2];
				child->set_parent_rot(x - 0.1, y, z);
			} while (child->get_child() != NULL);
		}
	}
	else if ((char)key == 'y') {
		//increment y
		//change_Yrot(torso, 0.1);
		float x, y, z;
		glm::vec3 r = curr->get_rotation();
		x = r[0];
		y = r[1];
		z = r[2];
		if (curr == &torso) {
			//todo
		}
		else if (curr->get_child() == NULL) {
			curr->set_rotation(x, y + 0.1, z);
		}
		else if (curr->get_child() != NULL) {
			curr->set_rotation(x, y+0.1, z);
			do {
				child = curr->get_child();
				r = child->get_parent_rot();
				x = r[0];
				y = r[1];
				z = r[2];
				child->set_parent_rot(x, y+0.1, z);
			} while (child->get_child() != NULL);
		}
	}
	else if ((char)key == 'Y') {
		//decrement y
		//change_Yrot(torso, -0.1);
		float x, y, z;
		glm::vec3 r = curr->get_rotation();
		x = r[0];
		y = r[1];
		z = r[2];
		if (curr == &torso) {
			//todo
		}
		else if (curr->get_child() == NULL) {
			curr->set_rotation(x, y - 0.1, z);
		}
		else if (curr->get_child() != NULL) {
			curr->set_rotation(x, y - 0.1, z);
			do {
				child = curr->get_child();
				r = child->get_parent_rot();
				x = r[0];
				y = r[1];
				z = r[2];
				child->set_parent_rot(x, y - 0.1, z);
			} while (child->get_child() != NULL);
		}
	}
	else if ((char)key == 'z') {
		//increment z
		//change_Zrot(torso,0.1);
		float x, y, z;
		glm::vec3 r = curr->get_rotation();
		x = r[0];
		y = r[1];
		z = r[2];
		if (curr == &torso) {
			//todo
		}
		else if (curr->get_child() == NULL) {
			curr->set_rotation(x, y, z+0.1);
		}
		else if (curr->get_child() != NULL) {
			curr->set_rotation(x, y, z+0.1);
			do {
				child = curr->get_child();
				r = child->get_parent_rot();
				x = r[0];
				y = r[1];
				z = r[2];
				child->set_parent_rot(x, y, z+0.1);
			} while (child->get_child() != NULL);
		}
	}
	else if ((char)key == 'Z') {
		//decrement z
		//change_Zrot(torso,-0.1);
		float x, y, z;
		glm::vec3 r = curr->get_rotation();
		x = r[0];
		y = r[1];
		z = r[2];
		if (curr == &torso) {
			//todo
		}
		else if (curr->get_child() == NULL) {
			curr->set_rotation(x, y, z - 0.1);
		}
		else if (curr->get_child() != NULL) {
			curr->set_rotation(x, y, z - 0.1);
			do {
				child = curr->get_child();
				r = child->get_parent_rot();
				x = r[0];
				y = r[1];
				z = r[2];
				child->set_parent_rot(x, y, z - 0.1);
			} while (child->get_child() != NULL);
		}
	}
	else if ((char)key == 'r') {
	rot++;
	float radius = 10.0f;
	float currx = eye[0];
	float currz = eye[2];
	float rotx = sin(rot) * radius;
	float rotz = cos(rot) * radius;
	eye[0] = rotx;
	eye[2] = rotz;
	}
	else if ((char)key == 'u') {
	rot++;
	float radius = 10.0f;
	float currx = eye[1];
	float currz = eye[2];
	float rotx = sin(rot) * radius;
	float rotz = cos(rot) * radius;
	eye[1] = rotx;
	eye[2] = rotz;
	}
}

void CreateCube()
{
	// x, y, z, r, g, b, ...
	float cubeVerts[] = {
		// Face x-
		-1.0f,	+1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	+1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	-1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	-1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	+1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	-1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		// Face x+
		+1.0f,	+1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	-1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	+1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	+1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	-1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	-1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		// Face y-
		+1.0f,	-1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	-1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	-1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	-1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	-1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	-1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		// Face y+
		+1.0f,	+1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	+1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	+1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	+1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	+1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	+1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		// Face z-
		+1.0f,	+1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	-1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		// Face z+
		+1.0f,	+1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	-1.0f,	+1.0f,	0.2f,	0.2f,	0.8f
	};

	GLuint vertBufferID;
	glGenBuffers(1, &vertBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

}

void FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Init()
{
	glfwInit();
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment2 - Rylan Wade", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCharCallback(window, CharacterCallback);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	program.SetShadersFileName("../shaders/shader.vert", "../shaders/shader.frag");
	program.Init();

	ConstructRobot();
	CreateCube();
}


int main()
{	
	Init();

	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

//eye = center + view;


		//eye[2] = newz;
		//std::cout << "Mouse position is: x - " << xpos << ", y - " << ypos << std::endl;


		/*if (xpos > intx && deltax > deltay) {
			theta += 0.01f;
			newx = rad * sin(phi)*cos(theta);
			//newz = center[2] + (rad * cos(theta));

			std::cout << "right" << std::endl;
			std::cout << newz << std::endl;

			eye[0] = newx;
			//eye[2] = newz;
		}

		else if (xpos < intx && deltax > deltay) {
			theta -= 0.01f;
			newx = rad * sin(phi)*cos(theta);
			//newz = center[2] + (rad * cos(theta));

			std::cout << "left" << std::endl;
			std::cout << newz << std::endl;

			eye[0] = newx;
			//eye[2] = newz;
		}*/

		/*if (ypos < inty && deltay > deltax) {
			phi += 0.01f;
			if (phi > 3.14 || phi < 0.0) {
				phi = 0.0;
			}
			newy = rad * sin(phi)*sin(theta);
			//newz = center[2] + (rad * cos(theta));

			newy = center[1] + (rad * sin(theta));

			std::cout << "up" << std::endl;
			std::cout << newz << std::endl;

			eye[1] = newy;
			//eye[2] = newz;


		}

		if (ypos > inty && deltay > deltax) {
			phi -= 0.01f;
			if (phi > 3.14 || phi < 0.0) {
				phi = 0.0;
			}
			newy = rad * sin(phi)*sin(theta);
			//newz = center[2] + (rad * cos(theta));

			std::cout << "down" << std::endl;
			std::cout << newz << std::endl;
			eye[1] = newy;
			//eye[2] = newz;

		}
		newz = rad * cos(phi);
		eye[2] = newz;*/