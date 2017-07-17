#include "Block.h"

Block::Block()
{
	type = Block::AIR;
}

int Block::uniqueFaceIndexOffset(Face::FaceIndex face)
{
	if (face == Face::FaceIndex::TOP) // grass,
	{
		if (type == Block::GRASS)
		{
			return 1; // grass block bottom is 1 atlas coord away from base
		}
	}
	else if (face == Face::FaceIndex::BOTTOM) // grass,
	{
		if (type == Block::GRASS)
		{
			return 2; // grass block bottom is 2 atlas coords away from base
		}
	}

	return 0;
}