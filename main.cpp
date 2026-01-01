#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "TextureClass.h"
#include "CameraClass.h"

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
	// Format: x, y, z, r, g, b, u, v
	// Layout: 3 position floats, 3 color floats, 2 UV floats (Texture Coordinates) (total 8 floats per vertex)
	GLfloat vertices[] = {
		-0.5f,  0.0f,  0.5f,  0.83f, 0.70f, 0.44f,  0.0f, 0.0f,
		-0.5f,  0.0f, -0.5f,  0.83f, 0.70f, 0.44f,  5.0f, 0.0f,

		 0.5f,  0.0f, -0.5f, 0.83f, 0.70f, 0.44f,  0.0f, 0.0f,
		 0.5f,  0.0f,  0.5f,  0.83f, 0.70f, 0.44f,  5.0f, 0.0f,
		
		 0.0f,  0.8f,  0.0f,  0.92f, 0.86f, 0.76f,  2.5f, 5.0f,
	};

	// Indices for vertices order
	// Using an element buffer avoids duplicating shared vertices
	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 1, 4,
		1, 2, 4,
		2, 3, 4,
		3, 0, 4
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
	// The Shader class compiles and links the given shader files and exposes the program ID
	Shader shaderProgram("default.vert", "default.frag");

	// Generates the Vertex Array Object and binds it
	// VAO encapsulates vertex attribute state (bindings, formats)
	VAO VAO1;
	VAO1.Bind();

	// Generates the Vertex Buffer Object and Element Buffer Object and links them to vertices/indices
	// VBO contains the interleaved vertex attributes; EBO contains triangle indices
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	// Links the VBO attributes to the VAO
	// Attribute 0: position (3 floats) at offset 0
	// Attribute 1: color    (3 floats) at offset 3*sizeof(float)
	// Attribute 2: UV       (2 floats) at offset 6*sizeof(float)
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Get the uniform location for the "scale" uniform in the shader program
	// Uniforms must be set on the active program; location is queried once for efficiency
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	// Texture
	Texture temptexture("tao.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	temptexture.texUnit(shaderProgram, "tex0", 0);

	// Enables the Depth Buffer
	glEnable(GL_DEPTH_TEST);

	// Creates the camera object
	Camera camera(fbWidth, fbHeight, glm::vec3(0.0f, 0.0f, 2.0f));

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

		// Cleans the back buffer and depth
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		camera.Inputs(window);
		
		// Updates and exports the camera matrix to the Vertex Shader
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "cameraMatrix");

		// Bind the texture so that OpenGL knows to use it
		temptexture.Bind();

		// Bind the VAO so that OpenGL knows to use it
		VAO1.Bind();

		// Draw the triangle using the GL_TRIANGLES primitive
		// Using glDrawElements leverages the EBO to reuse vertices
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);

		// Poll for and process events (if this is not here, the window will freeze and windows will say that its not responding)
		glfwPollEvents();
	};

	// Clean up and exit

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	temptexture.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}