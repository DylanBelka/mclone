#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>

#include <SDL.h>

#include <glew.h>

class Display
{
public:
	Display() = default;
	Display(int w, int h);
	~Display();

	void display();
	void clear();

	int width, height;

private:
	SDL_Window *win;
	SDL_GLContext glContext;
};

void handleSDLErr(std::string msg);


#endif // DISPLAY_H