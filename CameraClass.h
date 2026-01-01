#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H
#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "ShaderClass.h"

class Camera {
public:
	// Camera attributes
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 UpVector = glm::vec3(0.0f, 1.0f, 0.0f);

	// Prevents camera from jumping on the first click
	bool firstClick = true;

	// Window dimensions
	int width;
	int height;

	// Camera settings
	float cameraSpeed = 0.1f;
	float cameraSensitivity = 70.0f;

	// Constructor
	Camera(int width, int height, glm::vec3 position);

	// Exports the camera matrix to the Vertex Shader
	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);

	// Handles camera inputs
	void Inputs(GLFWwindow* window);
};

#endif