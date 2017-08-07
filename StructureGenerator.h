#ifndef STRUCTURE_GENERATOR_H
#define STRUCTURE_GENERATOR_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include <glm.hpp>
#include <gtx/string_cast.hpp>

#include "Block.h"
#include "math.h"
#include "Chunk.h"

class Chunk;

class StructureGenerator
{
public:
	StructureGenerator(const std::string& structureFile);

	bool StructureGenerator::placeStructure(const glm::vec3& where, Chunk* chunk);

	bool canPlaceAt(const glm::vec3& where, Chunk* chunk);

private:
	bool loadStructure(const std::string& structureFile);

	static bool isInBounds(const glm::vec3& blockCoords);
	static bool isValidCoord(const glm::vec3& blockCoords, Chunk* chunk);

	struct sgBlock
	{
		sgBlock(const glm::vec3& coords, Block::BlockType type)
		{
			this->coords = coords;
			this->blockType = type;
		}
		glm::vec3 coords;
		Block::BlockType blockType;
	};

	std::vector<sgBlock> structureData;
};

#endif // STRUCTURE_GENERATOR_H