#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

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

	// Vertices coordinates
	// Format: x, y, z, r, g, b
	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, 0.8f, 0.3f, 0.02f, // Lower left corner
		0.5, -0.5f * float(sqrt(3)) / 3, 0.0f, 0.8f, 0.3f, 0.02f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 1.0f, 0.6f, 0.32f, // Upper corner
		-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, 0.9f, 0.45f, 0.17f, // Inner left
		0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, 0.9f, 0.45f, 0.17f, // Inner right
		0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f, 0.8f, 0.3f, 0.02f,  // Inner down
	};

	// Indices for vertices order
	GLuint indices[] = {
		0, 3, 5, // Lower left triangle
		3, 2, 4, // Upper triangle
		5, 4, 1  // Lower right triangle
	};

	// Create a windowed mode window (Res, Title, Monitor <if we want fullscreen or smt>, Share <idk what that is yet>)
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Window", NULL, NULL);

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
	int fbWidth, fbHeight;
	glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
	glViewport(0, 0, fbWidth, fbHeight); // Set viewport to match the framebuffer size (handles high-DPI displays)
	
	// Creates a SHader object using the default vertex and fragment shaders
	Shader shaderProgram("default.vert", "default.frag");

	// Generates the Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates the Vertex Buffer Object and Element Buffer Object and links them to vertices/indices
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	// Links the VBO attributes to the VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Get the uniform location for the "scale" uniform in the shader program
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		// Set the "scale" uniform to a value (changes the size of the triangle) (MUST ALWAYS BE DONE AFTER ACTIVATING THE SHADER)
		glUniform1f(uniID, 0.5f);

		// Bind the VAO so that OpenGL knows to use it
		VAO1.Bind();

		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		// Poll for and process events (if this is not here, the window will freeze and windows will say that its not responding)
		glfwPollEvents();
	};

	// Clean up and exit

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}