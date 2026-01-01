// Vertex shader source code (i have no idea what that is lmao)
#version 330 core

// Positions
layout (location = 0) in vec3 aPos;

// Colors
layout (location = 1) in vec3 aColor;

// Texture coordinates
layout (location = 2) in vec2 aTex;

// Outputs colors to the Fragment Shader
out vec3 color;

// Outputs texture coordinates to the Fragment Shader
out vec2 texCoord;

// Imports the camera matrix from the main function
uniform mat4 cameraMatrix;

void main()
{
   gl_Position = cameraMatrix * vec4(aPos, 1.0);
   color = aColor;
   texCoord = aTex;
}