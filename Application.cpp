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

	glViewport(0, 0, w, h);
	glClearColor(0.0, 0.0, 0.0, 1.0);
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
				cam.move(direction::FORWARD);
				break;
			}
			case SDLK_s:
			{
				cam.move(direction::BACKWARD);
				break;
			}
			case SDLK_a:
			{
				cam.move(direction::LEFT);
				break;
			}
			case SDLK_d:
			{
				cam.move(direction::RIGHT);
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
			}
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			break;
		}
		case SDL_MOUSEMOTION:
		{
			cam.updateRot(e.motion.x, e.motion.y);
		}
		}
	}
}

void Application::run()
{
	Shader s("vertex.glsl", "fragment.glsl");
	Block::blockMeshData glBlockMeshData = Block::initBlockMesh(); // glBlockMeshData has vbo and vao as (x,y)

	/*
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	*/
	using namespace std::chrono;

	steady_clock::time_point beginFrame = steady_clock::now();
	steady_clock::time_point endFrame = steady_clock::now();

	duration<double> elapsed = duration<double>(endFrame - beginFrame);

	const double targetFPS = 10000;
	const double fpsMillisCap = 1.0 / targetFPS * 1000.0; // 1 frame per (1 / targetfps) ms

	glm::mat4 view;

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(90.f), (float)display->width / (float)display->height, .1f, 100.f);

#if 0
	const int numChunks = 1;

	std::vector<int> v;
	v.push_back(1);
	std::vector<std::array<std::array<std::array<Block, 256>, 16>, 16>> chunks; // wat

	for (int i = 0; i < numChunks; i++) {
		std::array<std::array<std::array<Block, 256>, 16>, 16> blocks;
		for (int x = 0; x < 16; x++)
		{
			for (int z = 0; z < 16; z++)
			{
				for (int y = 0; y < 256; y++)
				{
					blocks[x][z][y].moveTo(glm::vec3(x + i * 16, y, z));
				}
			}
		}
		chunks.push_back(blocks);
	}

#endif

#if 0

	std::vector<Block> blocks(100);
	for (int i = 0; i < blocks.size(); i++)
	{
		blocks[i].moveTo(glm::vec3(i * 2, 0.0, 0.0));
	}

#endif

	const unsigned int numChunks = 10;
	std::vector<Chunk> chunks;
	for (int i = 0; i < numChunks; i++)
	{
		chunks.push_back(Chunk());
		for (int j = 0; j < chunks[i].chunkSize; j++)
		{
			Block b = chunks[i].blocks[j];
			chunks[i].blocks[j].moveTo(glm::vec3(b.getPos().x + i * 16, b.getPos().y, b.getPos().z));
		}
	}

	while (running)
	{
		handleEvents(); // handle events
		view = cam.lookAt(); // update the camera

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture1);

		s.use(); // setup shaders for rendering

		glBindVertexArray(glBlockMeshData.VAO);
		display->clear();

		// all blocks use same view and projection data
		s.setUniformMat4("view", view);
		s.setUniformMat4("projection", projection);

#if 0
		for (int i = 0; i < numChunks; i++)
		{
			for (int x = 0; x < 16; x++)
			{
				for (int z = 0; z < 16; z++)
				{
					for (int y = 0; y < 256; y++)
					{
						chunks[i][x][z][y].update(s);
						chunks[i][x][z][y].draw();
					}
				}
			}
		}
#endif 


#if 0
		for (int i = 0; i < blocks.size(); i++)
		{
			blocks[i].update(s);
			blocks[i].draw();
		}

#endif

		for (Chunk c : chunks)
		{
			c.updateAndDraw(s);
		}

		display->display();

		endFrame = steady_clock::now();
		elapsed = duration<double>(endFrame - beginFrame);
		if (elapsed.count() * 1000 < fpsMillisCap)
		{
			SDL_Delay(abs(fpsMillisCap - elapsed.count() * 1000));
			//std::cout << "delaying by " << abs(fpsMillisCap - elapsed.count() * 1000) << " ms\n";
		}
	}
}