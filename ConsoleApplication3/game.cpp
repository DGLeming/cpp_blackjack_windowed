#include "game.h"
#include <iostream>
#include <time.h>
#include <string>

SDL_Texture* cardTex;
SDL_Rect cardSrc, cardDest;
//SDL_Rect cardSrc;

Game::Game()
{
	playdeck = Game::getShuffledDeck();
}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	Uint32 flags = 0;
	if (fullscreen){
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0){
		std::cout << "Subsystems initialized" << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window){
			std::cout << "Window created" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer){
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created" << std::endl;
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}
	/*SDL_Surface* tmpSurface = IMG_Load("assets/card.png");
	cardTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);*/

	cardSrc.x = cardSrc.y = 0;
	cardSrc.w = Game::cardWidth;
	cardSrc.h = Game::cardHeight;
	//addCard(Game::getCardAddres(playdeck[0]).c_str(), 200, 200);
	addDrawable("assets/draw.png", WINDOW_WIDTH / 2 - 122, WINDOW_HEIGHT - 200, 244, 56, DRAWCARD);
	addDrawable("assets/stand.png", WINDOW_WIDTH / 2 - 74, WINDOW_HEIGHT - 100, 148, 56, STAND);
	addDrawable("assets/you.png", 75, 90, 198, 48, DEFAULT);
	addDrawable("assets/score/21.png", 75 + (198 - 177) / 2, 20, 177, 48, DEFAULT);
	addDrawable("assets/dealer.png", WINDOW_WIDTH - 75 - 252, 90, 252, 48, DEFAULT);
	addDrawable("assets/score/21.png", WINDOW_WIDTH - 75 - 252 + (252 - 177) / 2, 20, 177, 48, DEFAULT);
	getCard(playerHand, true);
	getCard(dealerHand, false);
	getCard(playerHand, true);

}

void Game::update()
{}

void Game::render()
{
	SDL_RenderClear(renderer);
	//here add stuff to render
	for (int i = 0; i < cardsInDeck; i++){
		SDL_RenderCopy(renderer, Game::deck[i].texture, &cardSrc, &Game::deck[i].cardDest);
	}

	for (int i = 0; i < drawbls.size(); i++){
		SDL_RenderCopy(renderer, drawbls.at(i).texture, &drawbls.at(i).srcR, &drawbls.at(i).destR);
	}

	SDL_RenderPresent(renderer);
}

void Game::handleEnents()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	int x = 0;
	int y = 0;
	switch (event.type){
	case SDL_QUIT:
		isRunning = false;
		break;
	case SDL_MOUSEBUTTONDOWN:
		x = event.button.x;
		y = event.button.y;
		for (int i = 0; i < drawbls.size(); i++){
			// was mouse clicking on drawbl
			if ((x < (drawbls.at(i).destR.x + drawbls.at(i).destR.w)) && (x >= drawbls.at(i).destR.x) && (y < (drawbls.at(i).destR.y + drawbls.at(i).destR.h)) && (y >= drawbls.at(i).destR.y)){
				//your center = 75+198/2 = 174;
				//your height end = 50 + 48 = 98;
				//dealer center = 900-75-252/2 = 825 - 126 = 699
				if ((drawbls.at(i)).type == DRAWCARD){
					//addCard(Game::getCardAddres(playdeck[cardsInDeck]).c_str(), 174 - cardWidth / 2, 150);
					getCard(playerHand, true);
				}				
			}
		}
		/*addCard(Game::getCardAddres(playdeck[cardsInDeck]).c_str(), 200, 100);*/
		break;
	default:
		break;
	}
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "game cleaned" << std::endl;
}

void Game::addCard(const char* path, int posx, int posy, bool own)
{
	SDL_Surface* tmpSurface = IMG_Load(path);
	Game::deck[Game::cardsInDeck].texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	Game::deck[Game::cardsInDeck].cardDest.x = posx;
	Game::deck[Game::cardsInDeck].cardDest.y = posy;
	Game::deck[Game::cardsInDeck].cardDest.h = Game::cardHeight;
	Game::deck[Game::cardsInDeck].cardDest.w = Game::cardWidth;
	Game::deck[Game::cardsInDeck].owner = own;
	Game::cardsInDeck++;
}

void Game::addDrawable(const char* path, int posx, int posy, int width, int height, DrwblsTypes type)
{
	Drawable tmpDrawbl;
	SDL_Surface* tmpSurface = IMG_Load(path);
	tmpDrawbl.texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	tmpDrawbl.destR.x = posx;
	tmpDrawbl.destR.y = posy;
	tmpDrawbl.destR.w = width;
	tmpDrawbl.destR.h = height;
	tmpDrawbl.srcR.x = 0;
	tmpDrawbl.srcR.y = 0;
	tmpDrawbl.srcR.w = width;
	tmpDrawbl.srcR.h = height;
	tmpDrawbl.type = type;
	drawbls.push_back(tmpDrawbl);
}

void Game::getCard(std::array<PlayCard, 22> &hand, bool player)
{
	if (player)	{
		addCard(Game::getCardAddres(playdeck[cardsInDeck]).c_str(), 174 - cardWidth / 2, 150, true);
		hand[playerCards] = playdeck[cardsInDeck - 1];
		playerCards++;
	}
	else {
		addCard(Game::getCardAddres(playdeck[cardsInDeck]).c_str(), 699 - cardWidth / 2, 150, false);
		hand[dealerCards] = playdeck[cardsInDeck - 1];
		dealerCards++;
	}
	cardsRedraw();
}

void Game::cardsRedraw()
{
	int playerStart = 174 - playerCards * 35;
	int dealerStart = 700 - dealerCards * 35;
	int playerRedrawed = 0;
	int dealerRedrawed = 0;
	for (int i = 0; i < cardsInDeck; i++){
		if (deck[i].owner){
			deck[i].cardDest.x = playerStart + playerRedrawed * 70;
			playerRedrawed++;
		}
		else{
			deck[i].cardDest.x = dealerStart + dealerRedrawed * 70;
			dealerRedrawed++;
		}
	}
	std::cout << "Player have " << playerCards << " cards, and dealer have " << dealerCards << " cards" << std::endl;
}

void Game::swapCards(PlayCard &card1, PlayCard &card2){
	PlayCard temp = card1;
	card1 = card2;
	card2 = temp;
}

int Game::getRandomNumber(int min, int max)
{
	srand((unsigned)time(0) + rand());
	static const double fraction = 1.0 / (RAND_MAX + 1.0);
	return min + static_cast<int>((max - min + 1) * (std::rand() * fraction));
}

std::array<PlayCard, 52> Game::getShuffledDeck()
{
	std::array<PlayCard, 52> deck;
	int cardn = 0;
	for (int i = 0; i < MAX_SUITS; i++){
		for (int j = 0; j < MAX_RANKS; j++){
			deck[cardn].suit = static_cast<CardSuit>(i);
			deck[cardn].rank = static_cast<CardRank>(j);
			++cardn;
		}
	}
	for (int i = 0; i < 52; i++){
		int random = getRandomNumber(0, 51);
		swapCards(deck[i], deck[random]);
	}
	return deck;
}

std::string Game::getCardAddres(const PlayCard card) {
	std::string addres = "assets/";

	switch (card.suit){
	case 0:
		addres = addres + "CLUBS/";
		break;
	case 1:
		addres = addres + "DIAMONDS/";
		break;
	case 2:
		addres = addres + "HEARTS/";
		break;
	case 3:
		addres = addres + "SPADES/";
		break;
	default:
		break;
	}

	switch (card.rank){
	case 0:
		addres = addres + "2.png";
		break;
	case 1:
		addres = addres + "3.png";
		break;
	case 2:
		addres = addres + "4.png";
		break;
	case 3:
		addres = addres + "5.png";
		break;
	case 4:
		addres = addres + "6.png";
		break;
	case 5:
		addres = addres + "7.png";
		break;
	case 6:
		addres = addres + "8.png";
		break;
	case 7:
		addres = addres + "9.png";
		break;
	case 8:
		addres = addres + "10.png";
		break;
	case 9:
		addres = addres + "jack.png";
		break;
	case 10:
		addres = addres + "quin.png";
		break;
	case 11:
		addres = addres + "king.png";
		break;
	case 12:
		addres = addres + "ace.png";
		break;
	}

	return addres;
}