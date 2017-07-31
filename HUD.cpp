#include "HUD.h"

const float vertices[] = {
	// Pos      // Tex
	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f, 0.0f
};

void HUD::initialize()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(NUM_VBOS, VBOS);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, VBOS[VERTS]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
}

void HUD::draw()
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 4);
}