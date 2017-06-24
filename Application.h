#ifndef APPLICATION_H
#define APPLICATION_H

#include <chrono>
#include <vector>
#include <array>

#include <glew.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "stb_image.h"

#include "Display.h"
#include "Shader.h"
#include "Camera.h"
#include "Block.h"
#include "Chunk.h"

class Application
{
public:
	Application() = default;
	Application(int w, int h);
	~Application();

	void run();

private:
	void handleEvents();

	Display *display;
	Camera cam;

	SDL_Event e;
	bool running;
};

#endif // APPLICATION_H