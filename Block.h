#pragma once

#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Shader.h"
#include "stb_image.h"


/*	Wrapper class for a model. */
class Block
{
public:
	Block();
	Block(const glm::vec3& p);

	void moveTo(const glm::vec3& d);
	void update(Shader& s);
	void draw();

	//void setModelUnformLoc(const Shader& s) { modelUniformLoc = s.getUniformLocation("model"); }

	const glm::vec3 getPos() { return pos; }

	struct blockMeshData
	{
		blockMeshData(unsigned int vbo, unsigned int vao, unsigned int ebo) { VBO = vbo; VAO = vao; EBO; }

		unsigned int VBO;
		unsigned int VAO;
		unsigned int EBO;
	};

	enum BlockType
	{
		AIR,
		STONE,
		DIRT
	};
	
	static blockMeshData initBlockMesh();

protected:
	BlockType type;
	glm::vec3 pos;
	
	//unsigned int modelUniformLoc;
};