#ifndef APPLICATION_H
#define APPLICATION_H

#include <chrono>
#include <vector>
#include <array>

#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtx/intersect.hpp>

#include "stb_image.h"

#include "Display.h"
#include "Shader.h"
#include "Camera.h"
#include "Block.h"
#include "Chunk.h"
#include "World.h"
#include "HUD.h"
#include "Texture.h"
#include "Player.h"

class Application
{
public:
	Application() = default;
	Application(int w, int h);
	~Application();

	void run();

	const unsigned int renderDistance = 1; // number of chunks to render in any direction
	const float viewDistance = (float)sqrt(renderDistance) * (float)chunkWidth;//32.0 * (float)chunkWidth;

private:
	void handleEvents();

	Display *display;
	World world;
	Player player;
	HUD hud;

	Texture blockTextureAtlas;
	Texture hudtex;

	Shader blockShader;
	Shader hudShader;

	SDL_Event e;
	bool running;
};

#endif // APPLICATION_H