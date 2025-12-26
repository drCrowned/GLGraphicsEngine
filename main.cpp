#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Vertex shader source code (i have no idea what that is lmao)
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Fragment shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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
	GLfloat vertices[] = {
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left
		0.5, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper
	};

	// Create a windowed mode window (Res, Title, Monitor <if we want fullscreen or smt>, Share <idk what that is yet>)
	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);
	
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

	// Create the Vertext Shader Object and get the reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Attach the Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);


	// Create the Fragment Shader Object and get the reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Attach the Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// Compile the Fragment Shader into machine code
	glCompileShader(fragmentShader);

	// Create the Shader Program Object
	GLuint shaderProgram = glCreateProgram();

	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// Delete the Shaders because we dont need them anymore (and they're already in the program)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Create the refrence containers for the Vertex Array/Buffer Object
	GLuint VAO, VBO;

	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows how to use it
	glEnableVertexAttribArray(0);

	// Bind both the VAO and VBO to 0 so that we don't accidentally modify it
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Specify the color to clear the screen with (R, G, B, A)
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	// Clear the screen with the specified color
	glClear(GL_COLOR_BUFFER_BIT);

	// Swap the front and back buffers (double buffering)
	glfwSwapBuffers(window);

	// Main loop
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Tell OpenGL which Shader Program we want to use
		glUseProgram(shaderProgram);

		// Bind the VAO so that OpenGL knows to use it
		glBindVertexArray(VAO);

		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);

		// Poll for and process events (if this is not here, the window will freeze and windows will say that its not responding)
		glfwPollEvents();
	};

	// Clean up and exit
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}