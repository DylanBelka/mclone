#include "Chunk.h"
float vertices[] = {
	// x   y      z			u	v
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

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


void normalizeVertices()
{
	for (int i = 0; i < 180 - 2; i += 5)
	{
		vertices[i] = vertices[i] / (float)chunkWidth; // x
		vertices[i + 1] = vertices[i + 1] / (float)chunkHeight; // y
		vertices[i + 2] = vertices[i + 2] / (float)chunkDepth; // z
	}
}

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

void normalizeFaceVertices()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 18; j += 3)
		{
			faces[i][j] = faces[i][j] / (float)chunkWidth; // x
			faces[i][j + 1] = faces[i][j + 1] / (float)chunkHeight; // y
			faces[i][j + 2] = faces[i][j + 2] / (float)chunkDepth; // z
		}
	}
}

Chunk::Chunk()
{
	for (int x = 0; x < chunkWidth; x++)
	{
		for (int z = 0; z < chunkDepth; z++)
		{
			for (int y = 0; y < chunkHeight; y++)
			{
				blocks.push_back(Block(glm::vec3(x, y, z)));
			}
		}
	}
	glGenVertexArrays(1, &vao);
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

//#define ALL_CUBES

// each chunk will have its own vbo and vao 
// convert will convert the chunk's blocks into a single model that wil lbe stored using the vbo and vao
void Chunk::convert()
{
	using namespace std::chrono;

	steady_clock::time_point beginFrame = steady_clock::now();
	steady_clock::time_point endFrame = steady_clock::now();

	duration<double> elapsed = duration<double>(endFrame - beginFrame);
	
	beginFrame = steady_clock::now();

	std::vector<glm::vec3> finalModel;
	std::vector<glm::vec2> uvs;

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
#ifdef ALL_CUBES
				// this method is significantly slower in both generating the model and rendering
				if (blocks[x + chunkWidth * (y + chunkHeight * z)].type != Block::AIR) // skip this block if it is just air
				{
					for (int i = Face::FaceIndex::FRONT; i < Face::FaceIndex::NUM_FACES; i++)
					{
						std::vector<glm::vec3> face = getFace((Face::FaceIndex)i);
						std::vector<glm::vec2> faceUV = getFaceUV((Face::FaceIndex)i);
						
						for (glm::vec3& vert : face)
						{
							vert = normalizeCoord(vert + blocks[x + chunkWidth * (y + chunkHeight * z)].getPos());
							finalModel.push_back(vert);
						}
						for (glm::vec2& uv : faceUV)
						{
							uvs.push_back(uv);
						}

					}
				}				
#else
				if (blocks[x + chunkWidth * (y + chunkHeight * z)].type != Block::AIR) // skip this block if it is just air
				{
					// 3d coords to 1d indices of blocks all around current block
					int above = x + chunkWidth * ((y + 1) + chunkHeight * z);
					int below = x + chunkWidth * ((y - 1) + chunkHeight * z);
					int left = (x - 1) + chunkWidth * (y + chunkHeight * z);
					int right = (x + 1) + chunkWidth * (y + chunkHeight * z);
					int front = x + chunkWidth * (y + chunkHeight * (z + 1));
					int back = x + chunkWidth * (y + chunkHeight * (z - 1));

					glm::vec3 pos = blocks[x + chunkWidth * (y + chunkHeight * z)].getPos();

					// todo: rewrite this so its not a define
#define pushFace(faceIndex) std::vector<glm::vec3> face = getFace(faceIndex); \
							std::vector<glm::vec2> faceUV = getFaceUV(faceIndex); \
							for (glm::vec3& vert : face) { \
								vert = normalizeCoord(vert + blocks[x + chunkWidth * (y + chunkHeight * z)].getPos()); \
								finalModel.push_back(vert); } \
							for (glm::vec2& uv : faceUV) uvs.push_back(uv);

					//std::cout << glm::to_string(pos) << " " << (int)pos.x << " " << (int)pos.y << " " << (int)pos.z << " " << std::endl;

					// above
					if (((int)pos.y) >= chunkHeight - 1 || (above < blocks.size() && blocks[above].type == Block::AIR))
					{
						pushFace(Face::FaceIndex::TOP);
					}	
					// below
					if (((int)pos.y) <= 0 || (below < blocks.size() && blocks[below].type == Block::AIR))
					{
						pushFace(Face::FaceIndex::BOTTOM);
					}
					// left
					if (((int)pos.x) <= 0 || (left < blocks.size() && blocks[left].type == Block::AIR))
					{
						pushFace(Face::FaceIndex::LEFT);
					}
					// right
					if (((int)pos.x) == chunkWidth - 1 || (right < blocks.size() && blocks[right].type == Block::AIR))
					{
						pushFace(Face::FaceIndex::RIGHT);
					}
					// back 
					if (((int)pos.z) >= chunkDepth - 1 || (front < blocks.size() && blocks[front].type == Block::AIR))
					{
						pushFace(Face::FaceIndex::BACK);
					}
					// front
					if (((int)pos.z) <= 0 || (back < blocks.size() && blocks[back].type == Block::AIR))
					{
						pushFace(Face::FaceIndex::FRONT);
					}
				}
#undef pushFace
#endif
			}
		}
	}

	endFrame = steady_clock::now();

	elapsed = duration<double>(endFrame - beginFrame);
	std::cout << "done took: " << elapsed.count() << std::endl;

	numVerts = finalModel.size();
	sendModelDataToGL(finalModel, uvs);
}

void Chunk::sendModelDataToGL(const std::vector<glm::vec3>& model, const std::vector<glm::vec2>& uvs)
{
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glGenBuffers(1, &uvvbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, model.size() * sizeof(glm::vec3), &model[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, uvvbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}

void Chunk::draw(Shader& s)
{
	glBindVertexArray(vao);

	glm::mat4 m;
	m = glm::translate(m, pos);
	m = glm::scale(m, glm::vec3((float)chunkWidth, (float)chunkHeight, (float)chunkDepth));
	
	s.setUniformMat4("model", m);

	if (numVerts > 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, numVerts);
	}
	else
	{
		//std::cout << "drwaing empty chunk" << std::endl;
	}
}