#include "Chunk.h"

static float faces[6][18] =
{
	{
		-0.5f, -0.5f, -0.5f,//  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, // 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, // 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f, // 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,//  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,//  0.0f, 0.0f,
	},						//
							//
	{						//
		-0.5f, -0.5f,  0.5f,//  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f, // 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, // 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f, // 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,//  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,//  0.0f, 0.0f,
	},						//
							//
	{						//
		-0.5f,  0.5f,  0.5f,//  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,//  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,//  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,//  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,//  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,//  1.0f, 0.0f,
	},						//
							//
	{						//
		0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f, // 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, // 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f, // 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,
	},						//
							//
	{						//
		-0.5f, -0.5f, -0.5f,//  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, // 1.0f, 1.0f,
		0.5f, -0.5f,  0.5f, // 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f, // 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,//  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,//  0.0f, 1.0f,
	},						//
							//
	{						//
		-0.5f,  0.5f, -0.5f,//  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f, // 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f, // 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,//  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,//  0.0f, 1.0f
	},
};

float faceUVs[6][12] = 
{
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
	},

	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
	},

	{
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	},

	{
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
	},

	{
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
	},

	{
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	},
};

/*	Converts a "world" space coordinate into 
	an OpenGL vertex coordinate in range (-1.0, 1.0)
*/
glm::vec3 normalizeCoord(glm::vec3 c)
{
	float x = c.x / (float)chunkWidth;
	float y = c.y / (float)chunkHeight;
	float z = c.z / (float)chunkDepth;

	return glm::vec3(x, y, z);
}

Chunk::Chunk() :
	blocksxyz(chunkWidth, std::vector<std::vector<Block>>(chunkHeight, std::vector<Block>(chunkDepth, Block()))) // creates a 16x256x16 3d vector of default blocks
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	for (int i = 0; i < NUM_VBOS; i++)
	{
		glGenBuffers(1, &VBOS[i]);
	}
}

Chunk::~Chunk()
{

}

std::vector<glm::vec3> getFace(Face::FaceIndex index)
{
	std::vector<glm::vec3> face;
	for (int i = 0; i < 18; i += 3)
	{
		glm::vec3 vert(faces[index][i], faces[index][i + 1], faces[index][i + 2]);

		face.push_back(vert);
	}
	return face;
}

std::vector<glm::vec2> getFaceUV(Face::FaceIndex index)
{
	std::vector<glm::vec2> uvs;
	for (int i = 0; i < 12; i += 2)
	{
		glm::vec2 uv(faceUVs[index][i], faceUVs[index][i + 1]);

		uvs.push_back(uv);
	}
	return uvs;
}

// each chunk will have its own vbo and vao 
// convert will convert the chunk's blocks into a single model that will be stored using the vbo and vao
void Chunk::buildModel()
{
//#define DEBUG

#ifdef DEBUG
	using namespace std::chrono;

	steady_clock::time_point beginFrame = steady_clock::now();
	steady_clock::time_point endFrame = steady_clock::now();

	duration<double> elapsed = duration<double>(endFrame - beginFrame);
	
	beginFrame = steady_clock::now();
	
#endif // DEBUG

	std::vector<glm::vec3> finalModel;
	std::vector<glm::vec2> uvs;
	std::vector<int> blockTypeIndices;

	/* ALGORITHM
	
		for each cube
		{
			if it is not air
			{

				#if drawing all cubes (much slower)
					add the cube to the mesh
				#else 
					for each direction around the cube
					{
						if the cube in that direction is not air
						{
							add the face
						}
					}
			}
		}
	*/

	for (int y = 0; y < chunkHeight; y++)
	{
		for (int z = 0; z < chunkDepth; z++)
		{
			for (int x = 0; x < chunkDepth; x++)
			{
//#define ALL_CUBES
				// leave this method for debugging purposes 
#ifdef ALL_CUBES
				// this method is significantly slower in both generating the model and rendering
				if (blocksxyz[x][y][z].type != Block::AIR) // skip this block if it is just air
				{
					for (int i = Face::FaceIndex::FRONT; i < Face::FaceIndex::NUM_FACES; i++)
					{
						std::vector<glm::vec3> face = getFace((Face::FaceIndex)i);
						std::vector<glm::vec2> faceUV = getFaceUV((Face::FaceIndex)i);

						for (glm::vec3& vert : face)
						{
							vert = normalizeCoord(vert + blocksxyz[x][y][z].getPos());
							finalModel.push_back(vert);
						}
						for (glm::vec2& uv : faceUV)
						{
							uvs.push_back(uv);
						}

					}
				}
#else
				Block b = blocksxyz[x][y][z];
				if (b.type != Block::AIR) // skip this block if it is just air
				{
					glm::ivec3 bPos(x, y, z);

					auto pFace = [&b, &finalModel, &uvs, &bPos, &blockTypeIndices](Face::FaceIndex findex) // wew a lambda function
					{
						std::vector<glm::vec3> face = getFace(findex);
						std::vector<glm::vec2> faceUV = getFaceUV(findex);
						for (glm::vec3& vert : face) // push each vertex of the face after normalizing it
						{
							vert = normalizeCoord(vert + glm::vec3(bPos)); // shift the vertex over by the position of the block
							finalModel.push_back(vert);

							blockTypeIndices.push_back(b.type + b.uniqueFaceIndexOffset(findex));
						}
						for (glm::vec2& uv : faceUV)
						{
							uvs.push_back(uv);
						}
					};
					
					// check each direction around the current block and add a face in that direction if there is nothing there, ie air
					
					// above
					if (bPos.y >= chunkHeight - 1 || (bPos.y + 1 < chunkHeight && blocksxyz[bPos.x][bPos.y + 1][bPos.z].type == Block::AIR))
					{
						pFace(Face::FaceIndex::TOP);
					}
					// below
					if (bPos.y <= 0 || (y - 1 < chunkHeight && blocksxyz[bPos.x][bPos.y - 1][bPos.z].type == Block::AIR))
					{
						pFace(Face::FaceIndex::BOTTOM);
					}
					// left
					if (bPos.x <= 0 || (bPos.x - 1 < chunkHeight && blocksxyz[bPos.x - 1][bPos.y][bPos.z].type == Block::AIR))
					{
						pFace(Face::FaceIndex::LEFT);
					}
					// right
					if (bPos.x == chunkWidth - 1 || (bPos.x + 1 < chunkHeight && blocksxyz[bPos.x + 1][bPos.y][bPos.z].type == Block::AIR))
					{
						pFace(Face::FaceIndex::RIGHT);
					}
					// back 
					if (bPos.z >= chunkDepth - 1 || (bPos.z + 1 < chunkHeight && blocksxyz[bPos.x][bPos.y][bPos.z + 1].type == Block::AIR))
					{
						pFace(Face::FaceIndex::BACK);
					}
					// front
					if (bPos.z <= 0 || (bPos.z - 1 < chunkHeight && blocksxyz[bPos.x][bPos.y][bPos.z - 1].type == Block::AIR))
					{
						pFace(Face::FaceIndex::FRONT);
					}
				}
#endif
			}
		}
	}

#ifdef DEBUG

	endFrame = steady_clock::now();

	elapsed = duration<double>(endFrame - beginFrame);
	std::cout << "done took: " << elapsed.count() << std::endl;

#endif // DEBUG

	numVerts = finalModel.size();
	static std::mutex m;
	m.lock();
	sendModelDataToGL(finalModel, uvs, blockTypeIndices);
	m.unlock();
}

void Chunk::sendModelDataToGL(const std::vector<glm::vec3>& model, const std::vector<glm::vec2>& uvs, const std::vector<int>& blockTypeIndices)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[VERTS]);
	glBufferData(GL_ARRAY_BUFFER, model.size() * sizeof(glm::vec3), &model[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOS[UVS]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, VBOS[BLOCK_TYPES]);
	glBufferData(GL_ARRAY_BUFFER, blockTypeIndices.size() * sizeof(int), &blockTypeIndices[0], GL_STATIC_DRAW);
	glVertexAttribIPointer(2, 1, GL_INT, 0, (void*)0);
	glEnableVertexAttribArray(2);
}

void Chunk::draw(Shader& s)
{
	glBindVertexArray(vao);

	glm::mat4 m;
	m = glm::translate(m, glm::vec3(chunkPosxz.x, 0.0, chunkPosxz.y));

	// by normalizing coordinates earlier when defining mesh, all coordinates are in the range (-1, 1)
	// and are extremely small as a result
	// this scale returns them to their normal size with 1x1x1 cubes
	m = glm::scale(m, glm::vec3((float)chunkWidth, (float)chunkHeight, (float)chunkDepth));
	
	if (modelLoc == -1)
	{
		modelLoc = s.getUniformLocation("model");
	}

	s.setUniformMat4(m, modelLoc);

	if (numVerts > 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, numVerts);
	}
}

void Chunk::moveBlockTo(glm::ivec3 blockPos, glm::ivec3 where)
{
	Block b = blocksxyz[blockPos.x][blockPos.y][blockPos.z];
	blocksxyz[where.x][where.y ][where.z] = b;
}

void Chunk::generateChunk(int seed)
{
	srand(seed);

	for (int x = 0; x < chunkWidth; x++)
	{
		for (int z = 0; z < chunkDepth; z++)
		{
			int height = rand() % (chunkHeight / 16);
			for (int y = 0; y < height; y++)
			{
				blocksxyz[x][y][z].type = Block::GRASS;
			}
		}
	}
}