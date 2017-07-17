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

	//SDL_ShowCursor(SDL_DISABLE);
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	display = new Display(w, h);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew failed to initialize" << std::endl;
	}

	shader = Shader("vertex.glsl", "fragment.glsl");

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
			case SDLK_SPACE: // spacebar
			{
				std::cout << "Camera pos: " << glm::to_string(cam.getPos()) << std::endl;
				break;
			}
			case SDLK_r:
			{
				for (int i = 0; i < world.getNumChunks(); i++)
				{
					world.updateChunk(i);
				}
				break;
			}
			case SDLK_e:
			{
				//shader = Shader("vertex.glsl", "fragment.glsl");
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
				std::cout << "left button pressed\n";
				std::cout << "Camera front: " << glm::to_string(cam.getFront()) << std::endl;
				//std::cout << world.getBlockAt(glm::vec3(cam.getPos())).type << std::endl;
				//world.getBlockAt(glm::vec3(cam.getPos())).type = Block::AIR;

				std::cout << "Updating chunk: " << world.getChunkIndex(cam.getPos()) << std::endl;
				world.updateChunk(world.getChunkIndex(cam.getPos()));

				// start here

				break;
			}
			}

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
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load image, create texture and generate mipmaps
	int w, h, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load("textures.png", &w, &h, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	using namespace std::chrono;

	steady_clock::time_point beginFrame = steady_clock::now();
	steady_clock::time_point endFrame = steady_clock::now();

	duration<double> elapsed = duration<double>(endFrame - beginFrame);

	const double targetFPS = 2500;
	const double fpsMillisCap = 1.0 / targetFPS * 1000.0; // 1 frame per (1 / targetfps) ms

	glm::mat4 view;

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(90.f), (float)display->width / (float)display->height, .1f, 1000.f);

	const unsigned int numChunks = 1024;
	
	world.generateWorld(numChunks);

	while (running) 
	{
		beginFrame = steady_clock::now();

		handleEvents(); // handle events
		view = cam.lookAt(); // update the camera

		shader.use(); // setup shaders for rendering

		display->clear();

		// all blocks use same view and projection data
		shader.setUniformMat4("view", view);
		shader.setUniformMat4("projection", projection);
		
		world.draw(shader);

		display->display();

		endFrame = steady_clock::now();
		elapsed = duration<double>(endFrame - beginFrame);
		if (elapsed.count() * 1000 < fpsMillisCap)
		{
			SDL_Delay(abs(fpsMillisCap - elapsed.count() * 1000));
		}
	}
}