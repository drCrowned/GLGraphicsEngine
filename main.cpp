#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {

	// Initialize GLFW
	glfwInit();

	// Set GLFW window hints (OpenGL version, profile, etc...)

	// This tells OpenGL we want to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// This tells OpenGL we want to use the core profile (modern functions only)
	// Compatibility profile would give us access to deprecated functions, as well as modern ones
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window (Res, Title, Monitor <if we want fullscreen or smt>, Share <idk what that is yet>)
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", nullptr, nullptr);
	
	// Check if the window was created successfully, if not, terminate GLFW
	if (window == nullptr) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Make the window's context current (since we can have multiple windows and glfw is a bit stupid)
	glfwMakeContextCurrent(window);

	// Load OpenGL function pointers using GLAD
	gladLoadGL();

	// Set the viewport size (the part of the window OpenGL will render to)
	glViewport(0, 0, 800, 800);

	// Specify the color to clear the screen with (R, G, B, A)
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// Clear the screen with the specified color
	glClear(GL_COLOR_BUFFER_BIT);

	// Swap the front and back buffers (double buffering)
	glfwSwapBuffers(window);

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		// Poll for and process events (if this is not here, the window will freeze and windows will say that its not responding)
		glfwPollEvents();
	}

	// Clean up and exit
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}