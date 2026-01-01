#include "ShaderClass.h"
#include <stdexcept>

// Reads a text file and returns its contents as a string
std::string get_file_contents(const char *filename)
{

	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return (contents);
	}

	std::cerr << "Failed to open shader: " << filename << std::endl;
	return "";
}

// Constructor that builds the Shader Program from 2 different shaders
Shader::Shader(const char *vertexFile, const char *fragmentFile)
{
	// Read the Vertex/Fragment Shader code from the file and store it as a string
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Convert the shader source strings into character arrays
	const char *vertexSource = vertexCode.c_str();
	const char *fragmentSource = fragmentCode.c_str();

	// Create the Vertext Shader Object and get the reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Attach the Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);

	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	// Create the Fragment Shader Object and get the reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// Attach the Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

	// Compile the Fragment Shader into machine code
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	// Create the Shader Program Object
	ID = glCreateProgram();

	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	// Link all the shaders together into the Shader Program
	glLinkProgram(ID);
	compileErrors(ID, "PROGRAM");

	// Delete the Shaders because we dont need them anymore (and they're already in the program)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

// Activate the shader program
void Shader::Activate()
{
	glUseProgram(ID);
}

// Delete the shader program
void Shader::Delete()
{
	glDeleteProgram(ID);
}

// Checks for compilation errors
void Shader::compileErrors(unsigned int shader, const char *type)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (std::string(type) != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n"
					  << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n"
					  << infoLog << std::endl;
		}
	}
}