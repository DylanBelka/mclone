#ifndef BLOCK_H
#define BLOCK_H

#include <glm.hpp>

#include "Shader.h"
#include "stb_image.h"

namespace Face {
	enum FaceIndex
	{
		FRONT,
		BACK,
		LEFT,
		RIGHT,
		BOTTOM,
		TOP,

		NUM_FACES
	};
}

class Block
{
public:
	Block();
	/*
		returns the offset from the block type's base texture to the unique face texture
		or
		returns 0 if the face is not unique
	*/
	int uniqueFaceIndexOffset(Face::FaceIndex face); 

	enum BlockType
	{
		AIR,
		GRASS,
		DIRT = 3,
		STONE
	};

	BlockType type;
};

#endif // BLOCK_H
