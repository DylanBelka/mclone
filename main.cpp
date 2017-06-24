#include <iostream>
#include <string>
#include <SDL.h>
#include <glew.h>
#include "Application.h"


int main(int argc, char **argv)
{
	Application app(640, 480);
	app.run();

	return 0;
}