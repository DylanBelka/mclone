#include "Application.h"

Application::Application(int w, int h)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		handleSDLErr("Could not initialize SDL");
	}

	// initialize opengl attributes
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, GL_TRUE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, GL_TRUE);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	display = new Display(w, h);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew failed to initialize" << std::endl;
	}

	blockShader = Shader("blockVertex.glsl", "blockFragment.glsl");
	//hudShader = Shader("hudVertex.glsl", "hudFragment.glsl");

	blockTextureAtlas = Texture("textures.png");
	hudtex = Texture("crosshair.png");

	//hud.initialize();
	
	glViewport(0, 0, w, h);

	glClearColor(.5, .67, 1, 1.0);
	glEnable(GL_DEPTH_TEST);
	running = true;
}

Application::~Application()
{
	delete display;
	SDL_Quit();
}

void Application::handleEvents()
{
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
		{
			SDL_Keycode key = e.key.keysym.sym;
			switch (key)
			{
			case SDLK_ESCAPE:
			{
				running = false;
				break;
			}
			case SDLK_w:
			{
				player.move(direction::FORWARD);
				world.updateChunks(player.getPos(), chunkRenderDistance);
				break;
			}
			case SDLK_s:
			{
				player.move(direction::BACKWARD);
				world.updateChunks(player.getPos(), chunkRenderDistance);
				break;
			}
			case SDLK_a:
			{
				player.move(direction::LEFT);
				world.updateChunks(player.getPos(), chunkRenderDistance);
				break;
			}
			case SDLK_d:
			{
				player.move(direction::RIGHT);
				world.updateChunks(player.getPos(), chunkRenderDistance);
				break;
			}
			case SDLK_r:
			{
				world.getBlockAt(player.getPos());
				break;
			}
			case SDLK_RETURN: // enter key
			{
				static int glpMode = GL_FILL;
				if (glpMode == GL_POINT)
				{
					glpMode = GL_LINE;
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				}
				else if (glpMode == GL_LINE)
				{
					glpMode = GL_FILL;
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
				else if (glpMode == GL_FILL)
				{
					glpMode = GL_POINT;
					glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				}
				break;
			}
			case SDLK_SPACE: // spacebar
			{
				std::cout << "Camera pos: " << glm::to_string(player.getPos()) << std::endl;
				break;
			}
			}
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			switch (e.button.button)
			{
			case SDL_BUTTON_LEFT:
			{
				if (world.destroyBlockAt(player.getPos(), player.getFront()))
				{
					//world.updateChunk(world.getChunkIndex(player.getPos()));
					world.updateChunk(player.getPos());
				}

				break;
			}
			case SDL_BUTTON_RIGHT:
			{
				if (world.placeBlockAt(player.getPos(), player.getFront(), Block::GRASS))
				{
					//world.updateChunk(world.getChunkIndex(player.getPos()));
					world.updateChunk(player.getPos());
				}

				break;
			}
			}

			break;
		}
		case SDL_MOUSEMOTION:
		{
			player.updateRot(e.motion.x, e.motion.y);
			break;
		}
		}
	}
}

void Application::run()
{
	using namespace std::chrono;

	steady_clock::time_point beginFrame = steady_clock::now();
	steady_clock::time_point endFrame = steady_clock::now();

	duration<double> elapsed = duration<double>(endFrame - beginFrame);

	const double targetFPS = 2500;
	const double fpsMillisCap = 1.0 / targetFPS * 1000.0; // 1 frame per (1 / targetfps) ms

	glm::mat4 view;

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(90.f), (float)display->width / (float)display->height, .1f, 1000.f);

	unsigned int viewUniformLoc = blockShader.getUniformLocation("view");
	unsigned int projUniformLoc = blockShader.getUniformLocation("projection");

	std::cout << "view distance: " << viewDistance << std::endl;
	std::cout << "chunk render distance: " << chunkRenderDistance << std::endl;

	const unsigned int numChunks = 4 * chunkRenderDistance * chunkRenderDistance;
	std::cout << "number of chunks rendering: " << numChunks << std::endl;

	world.generateWorld(player, chunkRenderDistance, time(NULL));

	while (running)
	{
		beginFrame = steady_clock::now();

		handleEvents(); // handle events
		view = player.lookAt(); // update the camera

		display->clear();

		// draw blocks
		blockTextureAtlas.bind();
		blockShader.use(); // setup shaders for rendering

		// all blocks use same view and projection data
		blockShader.setUniformMat4(viewUniformLoc, view);
		blockShader.setUniformMat4(projUniformLoc, projection);
		
		world.draw(blockShader);

		// draw hud
		//hudtex.bind();
		//hudShader.use();

		//hud.draw();

		display->display();

		endFrame = steady_clock::now();
		elapsed = duration<double>(endFrame - beginFrame);
	}
}