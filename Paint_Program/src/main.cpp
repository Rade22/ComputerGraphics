#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>



#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
bool mask[WINDOW_WIDTH][WINDOW_HEIGHT];
GLFWwindow *window;


// Color structure. Can be used to define the brush and background color.
struct color { float r, g, b; };


color brush = { 1,1,1 };
color back;
color prev_back = { 0,0,0 };
int s = 2;
bool spray = false;

// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))




// Set a particular pixel of the frameBuffer to the provided color
void SetFrameBufferPixel(int x, int y, struct color lc)
{
	// Origin is upper left corner. 
	// Changes the origin from the lower-left corner to the upper-left corner
	y = WINDOW_HEIGHT - 1 - y;

	x = CLAMP(x, 0, WINDOW_WIDTH - 1);
	y = CLAMP(y, 0, WINDOW_HEIGHT - 1);

	frameBuffer[y][x][0] = lc.r;
	frameBuffer[y][x][1] = lc.g;
	frameBuffer[y][x][2] = lc.b;



}

//sets all pixels to background color and resets mask array to false
void clearWin(color lc) {
	for (int i = 0; i < WINDOW_WIDTH; i++) {
		for (int j = 0; j < WINDOW_HEIGHT; j++) {
			SetFrameBufferPixel(i, j, lc);

		}
	}

	memset(&mask[0][0], false, sizeof(mask));

}

//checks for pixels which represent user paintings
void compareFrameBuffer(int x, int y, struct color lc)
{
	// Origin is upper left corner. 
	// Changes the origin from the lower-left corner to the upper-left corner
	y = WINDOW_HEIGHT - 1 - y;

	x = CLAMP(x, 0, WINDOW_WIDTH - 1);
	y = CLAMP(y, 0, WINDOW_HEIGHT - 1);

	if (mask[x][y] != true) {
		SetFrameBufferPixel(x, y, lc);
	}

}

//changes background pixels with helper function compareFrmeBuffer()
void changeBackground() {
	std::cout << "Background color: {" << back.r << "," << back.g << "," << back.b << "}" << std::endl;
	for (int i = 0; i < WINDOW_WIDTH; i++) {
		for (int j = 0; j < WINDOW_HEIGHT; j++) {
			compareFrameBuffer(i, j, back);
		}
	}
}

void ClearFrameBuffer()
{
	memset(frameBuffer, 0.0f, sizeof(float) * WINDOW_WIDTH * WINDOW_HEIGHT * 3);
}

// Display frameBuffer on screen
void Display()
{
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

// Mouse position callback function
void CursorPositionCallback(GLFWwindow* lWindow, double xpos, double ypos)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		std::cout << "Mouse position is: x - " << xpos << ", y - " << ypos << std::endl;

		int x = xpos;
		int y = ypos;

		//normal rectangle brush
		if (spray == false) {
			for (int i = 0; i <= s; i++) {
				for (int j = 0; j <= s; j++) {
					SetFrameBufferPixel(xpos + i, ypos + j, brush);
					mask[x + i][y + j] = true;
					SetFrameBufferPixel(xpos - i, ypos + j, brush);
					mask[x - i][y + j] = true;
					SetFrameBufferPixel(xpos + i, ypos - j, brush);
					mask[x + i][y - j] = true;
					SetFrameBufferPixel(xpos - i, ypos - j, brush);
					mask[x - i][y - j] = true;
				}
			}
		}
		else {
			//spray paint rectangle brush
			int random = 0;
			for (int i = 0; i <= s; i++) {
				for (int j = 0; j <= s; j++) {
					random = (rand() % 100) + 1;
					if (random >= 95) {
						SetFrameBufferPixel(xpos + i, ypos + j, brush);
						mask[x + i][y + j] = true;
						SetFrameBufferPixel(xpos - i, ypos + j, brush);
						mask[x - i][y + j] = true;
						SetFrameBufferPixel(xpos + i, ypos - j, brush);
						mask[x + i][y - j] = true;
						SetFrameBufferPixel(xpos - i, ypos - j, brush);
						mask[x - i][y - j] = true;
					}
				}
			}
		}
	}
}

// Mouse callback function
void MouseCallback(GLFWwindow* lWindow, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		std::cout << "Mouse left button is pressed." << std::endl;

	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		std::cout << "Mouse right button is pressed." << std::endl;

		clearWin(back);
	}

}

// You can use "switch" or "if" to compare key to a specific character.
// for example,
// if (key == '0')
//     DO SOMETHING

// Keyboard callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	std::cout << "Key " << (char)key << " is pressed." << std::endl;

	if (key == '+' && s < 128) {
		s = 2 * s;
	}
	else if (key == 's') {
		//Extra: spray paint brush
		spray = !spray;
	}
	else if (key == '-' && s > 2) {
		s = s / 2;
	}
	else if (key == '0') {
		brush.r = 0;
		brush.g = 0;
		brush.b = 0;
	}
	else if (key == '1') {
		brush.r = 0;
		brush.g = 0;
		brush.b = 255;
	}
	else if (key == '2') {
		brush.r = 0;
		brush.g = 255;
		brush.b = 0;
	}
	else if (key == '3') {
		brush.r = 0;
		brush.g = 255;
		brush.b = 255;
	}
	else if (key == '4') {
		brush.r = 455;
		brush.g = 0;
		brush.b = 0;
	}
	else if (key == '5') {
		brush.r = 455;
		brush.g = 0;
		brush.b = 255;
	}
	else if (key == '6') {
		brush.r = 255;
		brush.g = 255;
		brush.b = 0;
	}
	else if (key == '7') {
		brush.r = 255;
		brush.g = 255;
		brush.b = 255;
	}
	else if (key == ')') {
		prev_back = back;

		back.r = 0;
		back.g = 0;
		back.b = 0;


		changeBackground();
	}
	else if (key == '!') {

		prev_back = back;

		back.r = 0;
		back.g = 0;
		back.b = 255;

		changeBackground();
	}
	else if (key == '@') {
		prev_back = back;

		back.r = 0;
		back.g = 255;
		back.b = 0;

		changeBackground();
	}
	else if (key == '#') {
		prev_back = back;

		back.r = 0;
		back.g = 255;
		back.b = 255;

		changeBackground();
	}
	else if (key == '$') {

		prev_back = back;

		back.r = 255;
		back.g = 0;
		back.b = 0;

		changeBackground();

	}
	else if (key == '%') {
		prev_back = back;

		back.r = 255;
		back.g = 0;
		back.b = 255;

		changeBackground();

	}
	else if (key == '^') {
		prev_back = back;

		back.r = 255;
		back.g = 255;
		back.b = 0;

		changeBackground();

	}
	else if (key == '&') {
		prev_back = back;

		back.r = 255;
		back.g = 255;
		back.b = 255;

		changeBackground();

	}
}

void Init()
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment 1 - Rylan Wade", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetCharCallback(window, CharacterCallback);
	glewExperimental = GL_TRUE;
	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();
}




int main()
{

	Init();
	while (glfwWindowShouldClose(window) == 0)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}