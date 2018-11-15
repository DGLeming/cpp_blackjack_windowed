#include "game.h"

Game *game = nullptr;

int main(int argc, char * argv[])
{
	const int FPS = 30;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;

	game = new Game();
	game->init("windowtitle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, false);
	/*game->addCard("assets/SPADES/ace.png", 0, 0);
	game->addCard("assets/SPADES/2.png", 70, 0);*/
	while (game->running()){
		frameStart = SDL_GetTicks();

		game->handleEnents();
		game->update();
		game->render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();
	return 0;
}

