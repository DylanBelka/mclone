#ifndef HUD_H
#define HUD_H

#include <iostream>

#include <glew.h>

#include "Shader.h"

class HUD
{
public:
	HUD() = default;
	void initialize();

	void draw();
private:
	enum
	{
		UVS = 0,
		VERTS,

		NUM_VBOS
	};

	unsigned int VBOS[NUM_VBOS];
	unsigned int vao;
};

#endif // HUD_H