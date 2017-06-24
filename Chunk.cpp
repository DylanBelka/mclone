#include "Chunk.h"

Chunk::Chunk()
{
	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			for (int y = 0; y < 256; y++)
			{
				blocks.push_back(Block(glm::vec3(x, y, z)));

			}
		}
	}
}

Chunk::~Chunk()
{
}

// each chunk will have its own vbo and vao 
// convert will convert the chunk's blocks into a single model that wil lbe stored using the vbo and vao

void Chunk::convert()
{
	std::vector<glm::vec3> finalModel;


}

void Chunk::updateAndDraw(Shader& s)
{
	for (int i = 0; i < chunkSize; i++)
	{
		// TODO: only update and/or draw blocks that are within the view distance
		
		blocks[i].update(s);
		blocks[i].draw();
	}
}