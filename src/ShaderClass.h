#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_contents(const char* filename);

class Shader {
public:
	// Reference ID of the Shader Program
	GLuint ID;

	// Constructor that builds the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activate the shader program
	void Activate();

	// Delete the shader program
	void Delete();

private:
	// Checks for compilation errors
	void compileErrors(unsigned int shader, const char* type);
};

#endif