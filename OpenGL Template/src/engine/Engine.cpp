#include "Engine.h"

void loadImGUI(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460 core");
}

/**
@param width Width of the created window
@param height Height of the created window
@param title Title of the window

@returns new GLFWwindow* as an int or a negative integer corresponding to an error with the render
	Possible error return values:
		PROJ_GLFW_LOAD_ERROR
		PROJ_NO_WINDOW_ERROR
		PROJ_GLAD_LOAD_ERROR
*/
int init(GLFWwindow* window, int width, int height, const char* title) {
    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "ERROR: Failed to initialize OpenGL context" << std::endl;
        return PROJ_GLAD_LOAD_ERROR;
    }
    printf("OpenGL Version %d.%d loaded\n", GLVersion.major, GLVersion.minor);

    loadImGUI(window);

	return (int) window;
}

void programLoop(GLFWwindow* window, std::function<void()> const& loop) {
    while (!glfwWindowShouldClose(window)) {
        glClearColor(c().BG_COLOR.r, c().BG_COLOR.g, c().BG_COLOR.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        loop(); 

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}