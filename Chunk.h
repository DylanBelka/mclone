#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <memory>
#include <iostream>

#include <glm.hpp>

#include "Block.h"
#include "Shader.h"

/****

	A chunk holds 16 * 16 * 256 blocks 
	Handles converting std::vector<Block> into a single chunk mesh
	For faster drawing + fewer draw calls
	
****/

class Chunk
{
public:
	Chunk();
	~Chunk();

	void convert();

	void updateAndDraw(Shader& s);

public:
	const unsigned int chunkSize = 16 * 16 * 256;
	std::vector<Block> blocks;

	unsigned int vbo;
	unsigned int vao;
};

#endif // CHUNK_H