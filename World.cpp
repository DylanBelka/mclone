#include "World.h"

World::World()
{

}

Block& World::getBlockAt(const glm::vec3& pos)
{
	Chunk* thisChunk;
	glm::vec3 chunkPos = pos;

	if (pos.x < 0)
	{
		chunkPos.x -= chunkWidth;
	}
	if (pos.z < 0)
	{
		chunkPos.z -= chunkDepth;
	}

	thisChunk = &hchunks[worldCoordToChunkCoord(chunkPos)];

	glm::ivec3 bpos(glm::abs((int)chunkPos.x % chunkWidth), glm::abs((int)chunkPos.y % chunkHeight), glm::abs((int)chunkPos.z % chunkDepth));
	return thisChunk->getBlockAt(bpos);
}

void World::updateChunk(glm::vec3 worldPos)
{
	Chunk* thisChunk;
	glm::vec3 chunkPos = worldPos;

	if (worldPos.x < 0)
	{
		chunkPos.x -= chunkWidth;
	}
	if (worldPos.z < 0)
	{
		chunkPos.z -= chunkDepth;
	}
	updateChunk(worldCoordToChunkCoord(chunkPos));
}

bool World::destroyBlockAt(const glm::vec3& startingPos, const glm::vec3& front, const float maxReach)
{
	glm::vec3 searchPos = startingPos;

	while (glm::distance(searchPos, startingPos) < maxReach)
	{
		searchPos += front;

		if (getBlockAt(searchPos).type != Block::AIR)
		{
			getBlockAt(searchPos).type = Block::AIR;
			return true;
		}
	}

	return false; // could not find a destroyable block
}

bool World::placeBlockAt(const glm::vec3& startingPos, const glm::vec3& front, const Block::BlockType type, const float maxReach)
{
	glm::vec3 searchPos = startingPos;

	while (glm::distance(searchPos, startingPos) < maxReach)
	{
		searchPos += front;

		if (getBlockAt(searchPos).type != Block::AIR)
		{
			while (getBlockAt(searchPos).type != Block::AIR) // traverse back until there is an air block to place on
			{
				searchPos -= front;
			}
			getBlockAt(searchPos).type = type;
			return true;
		}
	}

	return false; // could not place block
}

#define DEBUG

void World::generateWorld(Player& player, int numChunks, int seed)
{
#ifdef DEBUG
	using namespace std::chrono;

	steady_clock::time_point beginFrame = steady_clock::now();
	steady_clock::time_point endFrame = steady_clock::now();

	duration<double> elapsed = duration<double>(endFrame - beginFrame);

	beginFrame = steady_clock::now();
#endif // DEBUG

	int x, z;
	x = z = 0;
	float px, pz; // perlin offset coords, really just chunk coords
	float py;
 	int pxstart;

	//std::random_device r; // random_device for seeding the Mersenne Twister 19937 generator, slower than mt19937 but "more random"
	std::mt19937 eng(seed);
	std::uniform_int_distribution<int> distrInt(1, 10); // for seeding perlin noise
	std::uniform_real_distribution<float> distrFloat(-1000.f, 1000.f); // for varying increments of perlin noise crawling

	// randomize the starting position of the perlin noise
	// essentially acting as a "seed" for the perlin noise generator
	px = distrFloat(eng);
	pz = distrFloat(eng);
	py = distrFloat(eng);
	pxstart = px;
	std::cout << "px: " << px << " pz: " << pz << std::endl;

	player.moveTo(glm::vec3(px, 15.0, pz));
	std::cout << "player starting at: " << glm::to_string(player.getPos()) << std::endl;

	updateChunks(player.getPos(), numChunks * 4);

#ifdef DEBUG
	endFrame = steady_clock::now();

	elapsed = duration<double>(endFrame - beginFrame);
	std::cout << "done generating world took: " << elapsed.count() << std::endl;
#endif // DEBUG
}

bool isValidChunkPos(glm::vec2 chunkPos)
{
	glm::ivec2 cPos(chunkPos);
	return cPos.x % 16 == 0 && cPos.y % 16 == 0;
}

void World::updateChunks(const glm::vec3& playerPos, const unsigned int renderDistance)
{
	const float maxChunkDistance = (renderDistance * chunkWidth) * (renderDistance * chunkWidth);

	for (auto i = hchunks.begin(); i != hchunks.end(); i++)
	{
		Chunk* chk = &i->second;
		glm::vec2 chunkPos(chk->chunkPosxz);
		glm::vec2 playerPosXZ(playerPos.x, playerPos.z);

		glm::vec2 chunkPosCentered(chunkPos.x + chunkWidth / 2.f, chunkPos.y + chunkDepth / 2.f);

		float dst = math::sqrdDistPtToRect(playerPosXZ, chunkPosCentered, chunkWidth, chunkDepth);

		if (dst <= maxChunkDistance) // within render distance
		{
			if (!chk->isInPlayerView) // chunk has just become within view
			{
				chk->isInPlayerView = true; // make it viewable
			}
		}
		else // not within render distance
		{
			if (chk->isInPlayerView) // chunk has just become not within view
			{
				chk->isInPlayerView = false; // make it unviewable
			}
		}
	}

	glm::vec2 playerPosChunkCoord(worldCoordToChunkCoord(playerPos)); // coordinates of chunk player is in

	// positive, positive
	for (float x = playerPosChunkCoord.x; x < playerPosChunkCoord.x + ((renderDistance + 1) * 16); x += 16)
	{
		for (float y = playerPosChunkCoord.y; y < playerPosChunkCoord.y + ((renderDistance + 1) * 16); y += 16)
		{
			glm::vec2 newChunkCoord(x, y);
			if (isValidChunkPos(newChunkCoord))
			{
				glm::vec2 newChunkCoordCentered(newChunkCoord.x + chunkWidth / 2, newChunkCoord.y + chunkDepth / 2);
				if (math::sqrdDistPtToRect(playerPosChunkCoord, newChunkCoordCentered, chunkWidth, chunkDepth) <= maxChunkDistance)
				{
					if (hchunks.find(newChunkCoord) == hchunks.end()) // does not exist
					{
						createChunk(newChunkCoord, glm::vec3(newChunkCoord.x, 0.0, newChunkCoord.y));
					}
				}
			}
		}
	}

	// positive, negative
	for (float x = playerPosChunkCoord.x; x < playerPosChunkCoord.x + ((renderDistance + 1) * 16); x += 16)
	{
		for (float y = playerPosChunkCoord.y; y > playerPosChunkCoord.y - ((renderDistance + 1) * 16); y -= 16)
		{
			glm::vec2 newChunkCoord(x, y);
			if (isValidChunkPos(newChunkCoord))
			{
				glm::vec2 newChunkCoordCentered(newChunkCoord.x + chunkWidth / 2, newChunkCoord.y + chunkDepth / 2);
				if (math::sqrdDistPtToRect(playerPosChunkCoord, newChunkCoordCentered, chunkWidth, chunkDepth) <= maxChunkDistance)
				{
					if (hchunks.find(newChunkCoord) == hchunks.end()) // does not exist
					{
						createChunk(newChunkCoord, glm::vec3(newChunkCoord.x, 0.0, newChunkCoord.y));
					}
				}
			}
		}
	}

	// negative, positive
	for (float x = playerPosChunkCoord.x; x > playerPosChunkCoord.x - ((renderDistance + 1) * 16); x -= 16)
	{
		for (float y = playerPosChunkCoord.y; y < playerPosChunkCoord.y + ((renderDistance + 1) * 16); y += 16)
		{
			glm::vec2 newChunkCoord(x, y);
			if (isValidChunkPos(newChunkCoord))
			{
				glm::vec2 newChunkCoordCentered(newChunkCoord.x + chunkWidth / 2, newChunkCoord.y + chunkDepth / 2);
				if (math::sqrdDistPtToRect(playerPosChunkCoord, newChunkCoordCentered, chunkWidth, chunkDepth) <= maxChunkDistance)
				{
					if (hchunks.find(newChunkCoord) == hchunks.end()) // does not exist
					{
						createChunk(newChunkCoord, glm::vec3(newChunkCoord.x, 0.0, newChunkCoord.y));
					}
				}
			}
		}
	}

	// negative, negative
	for (float x = playerPosChunkCoord.x; x > playerPosChunkCoord.x - ((renderDistance + 1) * 16); x -= 16)
	{
		for (float y = playerPosChunkCoord.y; y > playerPosChunkCoord.y - ((renderDistance + 1) * 16); y -= 16)
		{
			glm::vec2 newChunkCoord(x, y);
			if (isValidChunkPos(newChunkCoord))
			{
				glm::vec2 newChunkCoordCentered(newChunkCoord.x + chunkWidth / 2, newChunkCoord.y + chunkDepth / 2);
				if (math::sqrdDistPtToRect(playerPosChunkCoord, newChunkCoordCentered, chunkWidth, chunkDepth) <= maxChunkDistance)
				{
					if (hchunks.find(newChunkCoord) == hchunks.end()) // does not exist
					{
						createChunk(newChunkCoord, glm::vec3(newChunkCoord.x, 0.0, newChunkCoord.y));
					}
				}
			}
		}
	}
}

void World::createChunk(glm::vec2 chunkPos, glm::vec3 perlinCoords)
{
	Chunk tmp;
	tmp.generateChunk(1, 15, perlinCoords, time(NULL));
	tmp.buildModel();
	tmp.moveChunkWorldSpace(chunkPos);
	hchunks[chunkPos] = tmp;
}

void World::draw(Shader& s)
{
	for (auto i = hchunks.begin(); i != hchunks.end(); i++)
	{
		Chunk* chk = &i->second;
		if (chk->isInPlayerView) // only draw chunks within render distance
		{
			chk->draw(s);
		}
	}
}