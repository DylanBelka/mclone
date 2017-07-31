#include "Texture.h"

Texture::Texture(const std::string& filePath)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load image, create texture and generate mipmaps
	int w, h, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(filePath.c_str(), &w, &h, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	std::cout << "texture: " << texture << std::endl;

	stbi_image_free(data);

	static GLenum lastUnit = GL_TEXTURE0;

	unit = lastUnit;
	lastUnit++;
	if (lastUnit == GL_TEXTURE31 + 1)
	{
		std::cout << "ran out of textures" << std::endl;
		system("pause");
	}

	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::bind()
{
	glActiveTexture(unit);
	glBindTexture(GL_TEXTURE_2D, texture);
}
