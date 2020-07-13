#include "Config.h"
#include "Engine.h"
#include "ShaderManager.h"
#include <glm\glm.hpp>
#include <glm\matrix.hpp>
 
float x = 0, currX = 0;
float y = 0, currY = 0;

bool mousePressed = false;
bool menuHovered = false;
bool showMenu = false;
bool mandelbrotMode = false;
double mStartX, mStartY;

double zoomAmt = 1;
double zoomIndex;
const float WIDTH = c().VIEW_RIGHT * 2;
// VIEW_BOTTOM, not VIEW_TOP because mouse coords and graph coords are different on the y-axis
const float HEIGHT = c().VIEW_TOP * 2;

float visWidth = WIDTH / (float)zoomAmt;
float visHeight = HEIGHT / (float)zoomAmt;

// Clamps the zoom to only where we can see the square in OpenGL
void clampPanArea(float& newX, float& newY) {
	if (newX + visWidth / 2 > (float)c().VIEW_RIGHT) {
		newX = c().VIEW_RIGHT - visWidth / 2;
	}
	else if (newX - visWidth / 2 < (float)c().VIEW_LEFT) {
		newX = c().VIEW_LEFT + visWidth / 2;
	}

	if (newY + visHeight / 2 > (float)c().VIEW_TOP) {
		newY = c().VIEW_TOP - visHeight / 2;
	}
	else if (newY - visHeight / 2 < (float)c().VIEW_BOTTOM) {
		newY = c().VIEW_BOTTOM + visHeight / 2;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	zoomIndex += yoffset;
	if (zoomIndex < 0) zoomIndex = 0;
	zoomAmt = std::pow(1.2, zoomIndex);
	std::cout << 1 / zoomAmt << std::endl;
	
	visWidth = WIDTH / (float)zoomAmt;
	visHeight = HEIGHT / (float)zoomAmt;
	clampPanArea(currX, currY);

	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{

	if (mousePressed && !menuHovered) {
		
		float newX = x + c().SENSITIVITY * (1 / std::abs(zoomAmt)) * (xpos - mStartX);
		float newY = y - c().SENSITIVITY * (1 / std::abs(zoomAmt)) * (ypos - mStartY);

		printf("%f %f\n", newX, newY);
		clampPanArea(newX, newY);

		currX = newX;
		currY = newY;
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
}

void window_resize_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
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
		glm::mat4 projMat = glm::ortho((float) c().VIEW_LEFT, (float) c().VIEW_RIGHT, (float) c().VIEW_BOTTOM, (float) c().VIEW_TOP, -1.f, 1.f);
		glm::mat4 viewMat = glm::mat4(1.0f);
		viewMat = glm::scale(viewMat, glm::vec3(1.0));
		shader.setUniformMat4fv("u_ProjMat", projMat);
		shader.setUniformMat4fv("u_ViewMat", viewMat);

		glfwSetScrollCallback(window, scroll_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetCursorPosCallback(window, cursor_position_callback);
		glfwSetFramebufferSizeCallback(window, window_resize_callback);
		glfwSetKeyCallback(window, key_callback);


		const float vertices[] = {
			c().VIEW_LEFT, c().VIEW_BOTTOM,
			c().VIEW_RIGHT, c().VIEW_BOTTOM,
			c().VIEW_RIGHT, c().VIEW_TOP,
			c().VIEW_RIGHT, c().VIEW_TOP,
			c().VIEW_LEFT, c().VIEW_TOP,
			c().VIEW_LEFT, c().VIEW_BOTTOM,
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
			Fractal settings
		*/
		float realComponent = 0, imaginaryComponent = 0;
		float rFac = 0.3f, gFac = 0.3f, bFac = 0.3f;
		// Real component of the Julia set equation
		shader.setUniform1f("r_Comp", realComponent);
		// Imaginary component of the Julia set equation
		shader.setUniform1f("z_Comp", imaginaryComponent);

		programLoop(window, [&shader, &viewMat, &realComponent, &imaginaryComponent, &rFac, &gFac, &bFac]() {
			viewMat = glm::mat4(1.0);
			viewMat = glm::scale(viewMat, glm::vec3(zoomAmt));
			viewMat = glm::translate(viewMat, glm::vec3(currX, currY, 0.0));

			if (showMenu) {
				ImGui::Begin("Modify the fractal!");
				ImGui::SliderFloat("Real Component", &realComponent, -4, 4);
				ImGui::SliderFloat("Imaginary Component", &imaginaryComponent, -4, 4);

				menuHovered = ImGui::IsWindowHovered() && ImGui::IsItemHovered();

				ImGui::Text("Shading Components");
				ImGui::SliderFloat("Red Factor", &rFac, 0, 1);
				ImGui::SliderFloat("Green Factor", &gFac, 0, 1);
				ImGui::SliderFloat("Blue Factor Factor", &bFac, 0, 1);

				if (ImGui::Button(mandelbrotMode ? "Disable Mandelbrot Mode" : "Enable Mandelbrot Mode")) {
					mandelbrotMode = !mandelbrotMode;
				}
				ImGui::End();
			}

			shader.setUniformMat4fv("u_ViewMat", viewMat);
			shader.setUniform1f("r_Comp", realComponent);
			shader.setUniform1f("z_Comp", imaginaryComponent);
			shader.setUniform1f("rFac", rFac);
			shader.setUniform1f("gFac", gFac);
			shader.setUniform1f("bFac", bFac);
			shader.setUniformBool("mandelbrot", mandelbrotMode);

			glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (2 * sizeof(float)));
		});

		shader.Unbind();
	}

	glfwTerminate();
}