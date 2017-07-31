#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <iostream>

#include <glew.h>

#include "stb_image.h"

class Texture
{
public:
	Texture() = default;
	Texture(const std::string& filePath);

	void bind();

private:
	GLenum unit;
	GLuint texture;
};

#endif // TEXTURE_H
