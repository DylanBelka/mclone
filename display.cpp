#include "Display.h"

Display::Display(int w, int h)
{
	width = w;
	height = h;

	win = SDL_CreateWindow("SDL WIN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (win == nullptr)
	{
		handleSDLErr("Could not create SDL window");
	}

	glContext = SDL_GL_CreateContext(win);
}

void handleSDLErr(std::string msg)
{
	std::cout << msg << ". SDL Error: " << SDL_GetError() << std::endl;
}

Display::~Display()
{
	if (win)
	{
		SDL_DestroyWindow(win);
	}
	if (glContext)
	{
		SDL_GL_DeleteContext(glContext);
	}
}

void Display::display()
{
	SDL_GL_SwapWindow(win);
}

void Display::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}