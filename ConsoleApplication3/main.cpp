#include "game.h"

Game *game = nullptr;
int width = 900;
int height = 600;
int main(int argc, char * argv[])
{
	game = new Game();
	game->init("windowtitle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, false);
	/*game->addCard("assets/SPADES/ace.png", 0, 0);
	game->addCard("assets/SPADES/2.png", 70, 0);*/
	while (game->running()){
		game->handleEnents();
		game->update();
		game->render();
	}

	game->clean();
	return 0;
}

