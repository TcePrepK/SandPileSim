#include "./src/utils/utils.h"
#include "./src/elementRegistry.h"

void eventSDL(SDL_Event event) {
	switch (event.type) {
	case SDL_QUIT: {
		globalVariables.gameIsRunning = false;
		break;
	}
	case SDL_MOUSEBUTTONDOWN: {
		if (event.button.button == SDL_BUTTON_LEFT) {
			s32 id = testForGUIs();
			if (id != -1) globalVariables.currentMatID = id;
			else globalVariables.holdingLeft = true;
		} else if (event.button.button == SDL_BUTTON_RIGHT) {
			// Vector tile = mouseToTile(globalVariables.mouse);
			// Vector p = {tile.x, tile.y};

			// globalVariables.shower.id = globalVariables.currentMatID;
			// globalVariables.shower.pos = p;
		}
		break;
	}
	case SDL_MOUSEBUTTONUP: {
		globalVariables.holdingLeft = false;
		break;
	}
	default:
		break;
	}
}

int main(int argc, char** argv) {
	globalVariables.init();
	SDL_Renderer* renderer = globalVariables.renderer;
	SDL_Texture* texture = globalVariables.texture;
	u32* pixels = globalVariables.pixels;

	// GUIs
	generateAllGUIs();

	// Background
	clearScreen(0x000000);

	// FPS Things
	clock_t current_ticks, delta_ticks;
	s64 fps = 0;

	SDL_Event event;
	while (globalVariables.gameIsRunning) {
		while (SDL_PollEvent(&event)) {
			eventSDL(event);

			Vector pos;
			SDL_GetMouseState(&pos.x, &pos.y);

			globalVariables.oldMouse.x = globalVariables.mouse.x;
			globalVariables.oldMouse.y = globalVariables.mouse.y;

			globalVariables.mouse.x = pos.x;
			globalVariables.mouse.y = pos.y;
		}

		// Calculate FPS
		current_ticks = clock();

		// Update Frame
		globalVariables.currentFrame++;

		// Mouse Input
		if (globalVariables.holdingLeft) {
			Vector oldTile = mouseToTile(globalVariables.oldMouse);
			Vector tile = mouseToTile(globalVariables.mouse);
			s32 id = globalVariables.currentMatID;
			if (id != STONE) {
				if (globalVariables.currentFrame % 10 == 0) {
					world.setRectPixel(tile.x, tile.y, 10, create_particle(id));
				}
			} else {
				world.setCircleLine(oldTile.x, oldTile.y, tile.x, tile.y, 2, create_particle(id));
			}
		}

		/*
		if (globalVariables.shower.id != -1 && globalVariables.currentFrame % 6 == 0) {
			s32 id = globalvariables.shower.id;
			s32 x = globalvariables.shower.pos.x;
			s32 y = globalvariables.shower.pos.y;
			world.setPixel(x, y, create_particle(id));
		}
		*/

		// Clean
		// clearScreen(0x000000);

		// Update World
		world.update();
		world.render();

		SDL_UpdateTexture(texture, NULL, pixels, globalVariables.screenWidth * sizeof(u32));
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		// calculate fps
		delta_ticks = clock() - current_ticks;
		if (delta_ticks > 0) {
			fps = CLOCKS_PER_SEC / delta_ticks;
			// cout << "delta ticks: " << delta_ticks << " fps: " << fps << endl;
		} else {
			fps = (s64)INFINITY;
		}
	}

	globalVariables.destroy();
	return 0;
}