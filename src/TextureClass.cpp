#include "TextureClass.h"

Texture::Texture(const char *image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	// Set texture type
	type = texType;

	// Load image
	int widthImg, heightImg, numColCh;
	stbi_set_flip_vertically_on_load(true); // Flip image on y-axis
	unsigned char *bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, STBI_rgb_alpha);
	if (!bytes)
	{
		std::cerr << "Failed to load texture: " << image << std::endl;
	}

	// Generate texture
	glGenTextures(1, &ID);

	// Assign texture to a Texture Unit
	glActiveTexture(slot);
	glBindTexture(texType, ID);

	// Set texture parameters
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Extra lines if choosing to use GL_CLAMP_TO_BORDER
	// float flatColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	// glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	// Assigns the image to the OpenGL Texture object
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenerateMipmap(texType);

	// Free image memory
	stbi_image_free(bytes);

	// Unbind texture
	glBindTexture(texType, 0);
}

// Assigns a texture unit to a uniform sampler
void Texture::texUnit(Shader &shader, const char *uniform, GLuint unit)
{
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

// Binds the texture
void Texture::Bind()
{
	glBindTexture(type, ID);
}

// Unbinds the texture
void Texture::Unbind()
{
	glBindTexture(type, 0);
}

// Deletes the texture
void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}