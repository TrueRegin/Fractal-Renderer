#include "Config.h"
#include "Engine.h"
#include "ShaderManager.h"
#include <glm\glm.hpp>
#include <glm\matrix.hpp>
#include <iostream>
#include <fstream>
#include <vector> 

double x = 0, currX = 0;
double y = 0, currY = 0;
float rotation = 0, currRotation = 0;

bool mousePressed = false;
bool rightMousePressed = false;
bool menuHovered = false;
bool showMenu = false;
bool mandelbrotMode = false;

double mStartX, mStartY;
double mStartXRot, mStartYRot;
double zoomAmt = 1;
double zoomIndex;

float
VIEW_RIGHT = c().AXIS_SIZE.x * c().CELL_WIDTH / 2,
VIEW_LEFT = c().AXIS_SIZE.x * c().CELL_WIDTH / -2,
VIEW_BOTTOM = c().AXIS_SIZE.y * c().CELL_WIDTH / -2,
VIEW_TOP = c().AXIS_SIZE.y * c().CELL_WIDTH / 2;

const float WIDTH = VIEW_RIGHT * 2;
// VIEW_BOTTOM, not VIEW_TOP because mouse coords and graph coords are different on the y-axis
const float HEIGHT = VIEW_TOP * 2;
float visWidth = WIDTH / (float)zoomAmt;
float visHeight = HEIGHT / (float)zoomAmt;

int windowWidth = (int) VIEW_RIGHT * 2;
int windowHeight = (int) VIEW_TOP * 2;

void imageTest(GLFWwindow* window) {
	//int width, height;
	//printf("writing image to test.bmp. . .");
	//glfwGetFramebufferSize(window, &width, &height);
	//std::vector<GLuint> image(width * height);
	//glReadPixels(0, 0, width, height, GL_RGBA, GL_BYTE, image.data());

	//std::ofstream file;
	//// Stores in raw rgba data
	//file.open("screenshots/screenshot");
	//	
	//for (int i = 0; i < image.size(); i++) {
	//	file << (const char) image[i];
	//}
	//file.close();
}

// Clamps the zoom to only where we can see the square in OpenGL
void clampPanArea(double& newX, double& newY) {
	if (newX + visWidth / 2 > (float)VIEW_RIGHT) {
		newX = VIEW_RIGHT - visWidth / 2;
	}
	else if (newX - visWidth / 2 < (float)VIEW_LEFT) {
		newX = VIEW_LEFT + visWidth / 2;
	}

	if (newY + visHeight / 2 > (float)VIEW_TOP) {
		newY = VIEW_TOP - visHeight / 2;
	}
	else if (newY - visHeight / 2 < (float)VIEW_BOTTOM) {
		newY = VIEW_BOTTOM + visHeight / 2;
	}
}

void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	zoomIndex += yoffset;
	if (zoomIndex < 0) zoomIndex = 0;
	zoomAmt = std::pow(1.2, zoomIndex);
	
	visWidth = WIDTH / (float)zoomAmt;
	visHeight = HEIGHT / (float)zoomAmt;
	clampPanArea(currX, currY);

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
}
void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (mousePressed && !menuHovered) {
		
		double newX = x + c().SENSITIVITY * (1 / std::abs(zoomAmt)) * (xpos - mStartX);
		double newY = y - c().SENSITIVITY * (1 / std::abs(zoomAmt)) * (ypos - mStartY);

		clampPanArea(newX, newY);

		printf("%f %f\n", newX, newY);

		currX = newX;
		currY = newY;
	}

	if (rightMousePressed && !menuHovered) {
		//float directionalSlope = (ypos - mStartYRot) / (xpos - mStartXRot);
		int d_Width, d_Height;
		glfwGetFramebufferSize(window, &d_Width, &d_Height);
		currRotation = -std::atan2f(ypos - d_Height/2, xpos - d_Width/2) * 180 / (2 * acos(0.0)); // 2 * acos(0.0) is pi, C++ doesn't have a pi variable by default :/
	}
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_1) {
		glfwGetCursorPos(window, &mStartX, &mStartY);
		mousePressed = true;
	}
	else if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_1) {
		mousePressed = false;
		x = currX;
		y = currY;
	}
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_2) {
		glfwGetCursorPos(window, &mStartXRot, &mStartYRot);
		rightMousePressed = true;
	}
	else if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_2) {
		rightMousePressed = false;
		rotation = currRotation;
	}
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			showMenu = !showMenu;
			break;
		case GLFW_KEY_E:
			mandelbrotMode = !mandelbrotMode;
			break;
		case GLFW_KEY_PRINT_SCREEN:
			imageTest(window);
			break;
		case GLFW_KEY_R:
			rotation = 0;
			currRotation = 0;
			break;
		}
	}
}


int main() {
	GLFWwindow* window;

	if (!glfwInit()) return PROJ_GLFW_LOAD_ERROR;
	window = glfwCreateWindow(c().WIDTH, c().HEIGHT, c().TITLE, NULL, NULL);
	if (!window) {
		glfwTerminate();
		return PROJ_NO_WINDOW_ERROR;
	}

	init(window, c().WIDTH, c().HEIGHT, c().TITLE);
	switch ((int)window)
	{
	case PROJ_NO_WINDOW_ERROR:
		std::cout << "GLFW window was not properly created during loadup, terminating!" << std::endl;
		return -1;
	case PROJ_GLFW_LOAD_ERROR:
		std::cout << "GLFW load error, terminating!" << std::endl;
		return -1;
	case PROJ_GLAD_LOAD_ERROR:
		std::cout << "GLAD load error, terminating" << std::endl;
		return -1;
	}

	{
		ShaderProgram shader("res/shaders/vertex.shader", "res/shaders/fragment.shader");
		shader.Bind();
		// To be used for rotation
		glm::mat4 modelMat = glm::mat4(1.0f);

		glfwSetScrollCallback(window, mouse_scroll_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetCursorPosCallback(window, mouse_position_callback);
		glfwSetKeyCallback(window, key_callback);


		const float vertices[] = {
			-1, -1,
			1, -1,
			1, 1,
			1, 1,
			-1, 1,
			-1, -1
		};

		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
		glEnableVertexAttribArray(0);

		/**
			Static Uniforms
		*/
		float realComponent = 0, imaginaryComponent = 0;
		float rFac = 0.3f, gFac = 0.3f, bFac = 0.3f;
		// Real & Imaginary components of the Julia set equation
		shader.setUniform1f("r_Comp", realComponent);
		shader.setUniform1f("z_Comp", imaginaryComponent);

		// Program Loop
		programLoop(window, [&shader, &modelMat, &realComponent, &imaginaryComponent, &rFac, &gFac, &bFac, &window]() {
			if (showMenu) {
				ImGui::Begin("Modify the fractal!");
				ImGui::SliderFloat("Real Component", &realComponent, -4, 4);
				ImGui::SliderFloat("Imaginary Component", &imaginaryComponent, -4, 4);

				ImGui::Text("Shading Components");
				ImGui::SliderFloat("Red Factor", &rFac, 0, 1);
				ImGui::SliderFloat("Green Factor", &gFac, 0, 1);
				ImGui::SliderFloat("Blue Factor Factor", &bFac, 0, 1);

				if (ImGui::Button(mandelbrotMode ? "Disable Mandelbrot Mode" : "Enable Mandelbrot Mode")) {
					mandelbrotMode = !mandelbrotMode;
				}
				ImGui::End();
			}

			shader.setUniform1f("r_Comp", realComponent);
			shader.setUniform1f("z_Comp", imaginaryComponent);
			shader.setUniform1f("rFac", rFac);
			shader.setUniform1f("gFac", gFac);
			shader.setUniform1f("bFac", bFac);
			shader.setUniformBool("mandelbrot", mandelbrotMode);
			
			int display_w, display_h;
			glfwGetFramebufferSize(window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);

			// Set new uniforms
			glm::vec2 visibleDims((1/zoomAmt) * (display_w / c().CELL_SIZE), (1 / zoomAmt) * (display_h / c().CELL_SIZE));
			glm::dvec2 translation(currX, currY);
			glm::dvec2 cornerPos(-translation.x - visibleDims.x / 2, -translation.y - visibleDims.y / 2);
			//printf("%f, %f\n", cornerPos.x, cornerPos.y);
			shader.setUniformDVec2("corner", cornerPos);
			shader.setUniformVec2("visible", visibleDims);
			shader.setUniform1d("scale", zoomAmt);

			glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (2 * sizeof(float)));
		});

		shader.Unbind();
	}

	glfwTerminate();
}