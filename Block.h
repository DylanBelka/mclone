#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shader.h"
#include "stb_image.h"

/*	Wrapper class for a model. */
class Block
{
public:

	friend class Chunk;

	Block();
	Block(const glm::vec3& p);

	void moveTo(const glm::vec3& d);
	void update(Shader& s);
	void draw();

	const glm::vec3 getPos() { return pos; }

	enum BlockType
	{
		AIR,
		STONE,
		DIRT
	};

	BlockType type;
	
protected:
	glm::vec3 pos;
};