#include "Block.h"

Block::Block()
{
	type = Block::AIR;
}

int Block::uniqueFaceIndexOffset(Face::FaceIndex face)
{
	if (face == Face::FaceIndex::TOP) // grass, wood
	{
		if (type == Block::GRASS)
		{
			return 1; // grass block bottom is 1 atlas coord away from base
		}
		if (type == Block::WOOD)
		{
			return 1;
		}
	}
	else if (face == Face::FaceIndex::BOTTOM) // grass, wood
	{
		if (type == Block::GRASS)
		{
			return 2; // grass block bottom is 2 atlas coords away from base
		}
		if (type == Block::WOOD)
		{
			return 1;
		}
	}

	return 0;
}

Block::BlockType Block::strToBlockType(const std::string& str)
{
	if (str == "AIR")
	{
		return Block::AIR;
	}
	if (str == "GRASS")
	{
		return Block::GRASS;
	}
	if (str == "DIRT")
	{
		return Block::DIRT;
	}
	if (str == "STONE")
	{
		return Block::STONE;
	}
	if (str == "WOOD")
	{
		return Block::WOOD;
	}
	if (str == "LEAVES")
	{
		return Block::LEAVES;
	}
	return Block::AIR;
}