#ifndef BLOCK_H
#define BLOCK_H

#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shader.h"
#include "stb_image.h"

class Block
{
public:
	Block();

	enum BlockType
	{
		AIR,
		STONE,
		DIRT
	};

	BlockType type; 
};

#endif // BLOCK_H
