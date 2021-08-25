#ifndef VARIABLES_CPP
#define VARIABLES_CPP

static struct globalVariables {
	b32 gameIsRunning = true;
	s32 currentFrame = 0;

	const u32 screenWidth = 512;
	const u32 screenHeight = 512;

	s32 currentMatID = 1;
	// particle_shower shower;

	Vector oldMouse;
	Vector mouse;
	b32 holdingLeft = false;

	u32* pixels = new u32[screenWidth * screenHeight];

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;

	void init() {
		SDL_Init(SDL_INIT_EVERYTHING);

		window = SDL_CreateWindow("sdl2 pixel drawing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, 0);

		if (window == nullptr) {
			SDL_Log("Could not create the window: %s", SDL_GetError());
			return;
		}

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

		if (renderer == nullptr) {
			SDL_Log("Could not create the renderer: %s", SDL_GetError());
			return;
		}

		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, screenWidth, screenHeight);

		if (texture == nullptr) {
			SDL_Log("Could not create the texture: %s", SDL_GetError());
			return;
		}
	}

	void destroy() {
		delete[] pixels;
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
} globalVariables;

#endif // VARIABLES_CPP