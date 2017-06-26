#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include <memory>
#include <iostream>

#include <chrono>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/string_cast.hpp>

#include "Block.h"
#include "Shader.h"

const unsigned int chunkWidth = 16;
const unsigned int chunkHeight = 256;
const unsigned int chunkDepth = 16;

const unsigned int chunkSize = chunkWidth * chunkDepth * chunkHeight;

extern float vertices[180];

void normalizeVertices();
void normalizeFaceVertices();

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

std::vector<glm::vec3> getFace(Face::FaceIndex index);

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

	void moveBlockTo(unsigned int index, glm::vec3 where) { blocks[index] = where; }
	glm::vec3 getBlockPos(unsigned int index) { return blocks[index].getPos(); }

	void draw(Shader& s);

public:
	void sendModelDataToGL(const std::vector<glm::vec3>& model, const std::vector<glm::vec2>& uvs);
	
	std::vector<Block> blocks;

	glm::vec3 pos;

	unsigned int vbo;
	unsigned int uvvbo;
	unsigned int vao;

	unsigned int numVerts;
};

#endif // CHUNK_H