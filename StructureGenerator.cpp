#include "StructureGenerator.h"

StructureGenerator::StructureGenerator(const std::string& structureFile)
{
	if (!loadStructure(structureFile))
	{
		structureData.push_back(sgBlock(glm::vec3(0.0, 0.0, 0.0), Block::AIR));
	}
}

bool StructureGenerator::placeStructure(const glm::vec3& where, Chunk* chunk)
{
	for (int i = 0; i < structureData.size(); i++)
	{
		sgBlock b = structureData[i];
		glm::vec3 coord = b.coords + where;
		if (isValidCoord(coord, chunk))
		{
			chunk->getBlockAt(coord).type = b.blockType;
		}
	}
	return true;
}

bool StructureGenerator::loadStructure(const std::string& structureFile)
{
	std::ifstream file;
	std::string line;
	file.open(structureFile);
	if (file.bad() || file.fail())
	{
		std::cout << "StructureGenerator unable to load structure file: " << structureFile << std::endl;
		return false;
	}

	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			if (line.size() > 0 && line[0] != '#') // # are comments
			{
				// get coords
				int x = std::atoi(line.substr(0, 2).c_str());
				int y = std::atoi(line.substr(3, 2).c_str());
				int z = std::atoi(line.substr(6, 2).c_str());
				glm::vec3 coords(x, y, z);

				// get the type
				std::string typeStr = line.substr(9);
				Block::BlockType type = Block::strToBlockType(typeStr);

				sgBlock data(coords, type);
				structureData.push_back(data);
			}
		}
	}
	file.close();
	return true;

}

bool StructureGenerator::canPlaceAt(const glm::vec3& where, Chunk* chunk)
{
	for (int i = 0; i < structureData.size(); i++)
	{
		glm::vec3 pos = structureData[i].coords + where;
		if (!isValidCoord(pos, chunk))
		{
			return false;
		}
	}
	return true;
}

bool StructureGenerator::isInBounds(const glm::vec3& blockCoords)
{
	if (blockCoords.x >= chunkWidth || blockCoords.x < 0)
	{
		return false;
	}
	if (blockCoords.y >= chunkHeight || blockCoords.y < 0)
	{
		return false;
	}
	if (blockCoords.z >= chunkDepth || blockCoords.z < 0)
	{
		return false;
	}
	return true;
}

bool StructureGenerator::isValidCoord(const glm::vec3& blockCoords, Chunk* chunk)
{
	if (!isInBounds(blockCoords))
	{
		return false;
	}
	if (chunk->getBlockAt(blockCoords).type != Block::AIR)
	{
		return false;
	}
	return true;
}