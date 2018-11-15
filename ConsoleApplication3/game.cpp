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

void Game::newgame()
{
	for (int i = 0; i < cardsInDeck; i++){
		deck[i].texture = nullptr;
		deck[i].cardDest.x = deck[i].cardDest.y = deck[i].cardDest.w = deck[i].cardDest.h = 0;
		deck[i].owner = false;
	}
	cardsInDeck = 0;
	playdeck = Game::getShuffledDeck();
	std::vector<Drawable> tmpDrawbls;
	drawbls = tmpDrawbls;
	scoreDrawables = tmpDrawbls;
	std::array<PlayCard, 22> emptyHand;
	playerHand = emptyHand;
	dealerHand = emptyHand;
	playerCards = dealerCards = playerScore = dealerScore = 0;
	initForNewGame();
}

void Game::initForNewGame()
{
	/*SDL_Surface* tmpSurface = IMG_Load("assets/card.png");
	cardTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);*/
	// Adding score drawables to scoreDrawables
	Drawable tmpDrawable;
	SDL_Surface* tmpSurface = IMG_Load("assets/score/gameover.png");

	tmpDrawable.texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

	SDL_FreeSurface(tmpSurface);

	tmpDrawable.srcR.x = tmpDrawable.srcR.y = 0;
	tmpDrawable.srcR.w = tmpDrawable.destR.w = 189;
	tmpDrawable.srcR.h = tmpDrawable.destR.h = 48;
	tmpDrawable.destR.y = 20;
	tmpDrawable.type = DEFAULT;

	tmpDrawable.destR.x = 75 + 198 / 2 - 189 / 2;
	scoreDrawables.push_back(tmpDrawable);
	tmpDrawable.destR.x = WINDOW_WIDTH - 75 - 252 / 2 - 189 / 2;
	scoreDrawables.push_back(tmpDrawable);

	// Adding some buttons and text

	cardSrc.x = cardSrc.y = 0;
	cardSrc.w = Game::cardWidth;
	cardSrc.h = Game::cardHeight;

	addDrawable("assets/draw.png", WINDOW_WIDTH / 2 - 122, WINDOW_HEIGHT - 200, 244, 56, DRAWCARD);
	addDrawable("assets/stand.png", WINDOW_WIDTH / 2 - 74, WINDOW_HEIGHT - 100, 148, 56, STAND);

	addDrawable("assets/you.png", 75, 90, 198, 48, DEFAULT);
	addDrawable("assets/dealer.png", WINDOW_WIDTH - 75 - 252, 90, 252, 48, DEFAULT);
	getCard(playerHand, true);
	getCard(dealerHand, false);
	getCard(playerHand, true);
}

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
	initForNewGame();
}

void Game::update()
{}

void Game::render()
{
	SDL_RenderClear(renderer);
	//here add stuff to render
	//render of all cards
	for (int i = 0; i < cardsInDeck; i++){
		SDL_RenderCopy(renderer, Game::deck[i].texture, &cardSrc, &Game::deck[i].cardDest);
	}
	//render of all additional objects
	for (int i = 0; i < drawbls.size(); i++){
		SDL_RenderCopy(renderer, drawbls.at(i).texture, &drawbls.at(i).srcR, &drawbls.at(i).destR);
	}
	//render of scores for player and dealer
	SDL_RenderCopy(renderer, scoreDrawables.at(0).texture, &scoreDrawables.at(0).srcR, &scoreDrawables.at(0).destR);
	SDL_RenderCopy(renderer, scoreDrawables.at(1).texture, &scoreDrawables.at(1).srcR, &scoreDrawables.at(1).destR);

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
				else if ((drawbls.at(i)).type == NO){
					isRunning = false;
				}
				else if ((drawbls.at(i)).type == YES){
					newgame();
				}
				else if ((drawbls.at(i)).type == STAND){
					while (dealerScore <= 17){
						getCard(dealerHand, false);
					}
					if (dealerScore > 21){
						gameOver(true, false);
					}
					else {
						if (dealerScore < playerScore){
							gameOver(true, false);
						}
						else if (dealerScore > playerScore){
							gameOver(false, false);
						}
						else if (dealerScore == playerScore){
							gameOver(false, true);
						}
					}
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
		switch (hand[playerCards].rank){
		case RANK_2:
			playerScore += 2;
			break;
		case RANK_3:
			playerScore += 3;
			break;
		case RANK_4:
			playerScore += 4;
			break;
		case RANK_5:
			playerScore += 5;
			break;
		case RANK_6:
			playerScore += 6;
			break;
		case RANK_7:
			playerScore += 7;
			break;
		case RANK_8:
			playerScore += 8;
			break;
		case RANK_9:
			playerScore += 9;
			break;
		case RANK_10:
		case RANK_JACK:
		case RANK_QUEEN:
		case RANK_KING:
			playerScore += 10;
			break;
		case RANK_ACE:
			if (playerScore <= 10){
				playerScore += 11;
			}
			else {
				playerScore += 1;
			}
			break;
		default:
			break;
		}
		playerCards++;
	}
	else {
		addCard(Game::getCardAddres(playdeck[cardsInDeck]).c_str(), 699 - cardWidth / 2, 150, false);
		hand[dealerCards] = playdeck[cardsInDeck - 1];
		switch (hand[dealerCards].rank){
		case RANK_2:
			dealerScore += 2;
			break;
		case RANK_3:
			dealerScore += 3;
			break;
		case RANK_4:
			dealerScore += 4;
			break;
		case RANK_5:
			dealerScore += 5;
			break;
		case RANK_6:
			dealerScore += 6;
			break;
		case RANK_7:
			dealerScore += 7;
			break;
		case RANK_8:
			dealerScore += 8;
			break;
		case RANK_9:
			dealerScore += 9;
			break;
		case RANK_10:
		case RANK_JACK:
		case RANK_QUEEN:
		case RANK_KING:
			dealerScore += 10;
			break;
		case RANK_ACE:
			if (dealerScore <= 10){
				dealerScore += 11;
			}
			else {
				dealerScore += 1;
			}
			break;
		default:
			break;
		}
		dealerCards++;
	}
	cardsRedraw(player);
}

void Game::gameOver(bool winner, bool tie)
{
	drawbls.at(0).destR.w = 0;
	drawbls.at(1).destR.w = 0;
	if (tie){
		addDrawable("assets/tie.png", WINDOW_WIDTH / 2 - 109, WINDOW_HEIGHT - 300, 218, 59, DEFAULT);
	}
	else {
		if (winner){
			addDrawable("assets/won.png", WINDOW_WIDTH / 2 - 109, WINDOW_HEIGHT - 300, 218, 59, DEFAULT);
		}
		else{
			addDrawable("assets/lost.png", WINDOW_WIDTH / 2 - 115, WINDOW_HEIGHT - 300, 231, 59, DEFAULT);
		}
	}
	addDrawable("assets/restart.png", WINDOW_WIDTH / 2 - 211, WINDOW_HEIGHT - 210, 422, 52, DEFAULT);
	addDrawable("assets/yes.png", WINDOW_WIDTH / 2 - 174 - 25, WINDOW_HEIGHT - 130, 174, 86, YES);
	addDrawable("assets/no.png", WINDOW_WIDTH / 2 + 25 + (174-126)/2, WINDOW_HEIGHT - 130, 126, 86, NO);
}

void Game::cardsRedraw(bool player)
{
	int playerStart = 174 - playerCards * 35;
	int dealerStart = 700 - dealerCards * 35;
	int playerRedrawed = 0;
	int dealerRedrawed = 0;
	// drawing cards;
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
	//drawing score
	int padding = 75;
	if ((playerScore <= 21) && (dealerScore <= 21)){
		if (player){
			if (playerScore == 21){
				gameOver(true, false);
				//player got 21 and Won!
				Drawable tmpDrawable;
				SDL_Surface* tmpSurface = IMG_Load("assets/score/gameover.png");

				tmpDrawable.texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

				SDL_FreeSurface(tmpSurface);
				int width = 189;
				
				tmpDrawable.srcR.w = tmpDrawable.destR.w = width;
				tmpDrawable.srcR.x = tmpDrawable.srcR.y = 0;
				tmpDrawable.srcR.h = tmpDrawable.destR.h = 48;
				tmpDrawable.destR.y = 20;

				tmpDrawable.destR.x = padding + 198 / 2 - width / 2;
				scoreDrawables.at(0) = tmpDrawable;
				tmpDrawable.destR.x = WINDOW_WIDTH - padding - 252 / 2 - width / 2;
				scoreDrawables.at(1) = tmpDrawable;
			}
			else {
				std::cout << "2" << std::endl;
				std::string url = getUrl(playerScore);
				SDL_Surface* tmpSurface = IMG_Load(url.c_str());
				scoreDrawables.at(0).texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
				SDL_FreeSurface(tmpSurface);

				int width = getImageWidth(playerScore);

				scoreDrawables.at(0).srcR.w = scoreDrawables.at(0).destR.w = width;

				scoreDrawables.at(0).destR.x = padding + 198 / 2 - width / 2;
			}
		}
		else {
			if (dealerScore == 21){
				gameOver(false, false);
				//dealer got 21 and Won!
				Drawable tmpDrawable;
				SDL_Surface* tmpSurface = IMG_Load("assets/score/gameover.png");

				tmpDrawable.texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

				SDL_FreeSurface(tmpSurface);
				int width = 189;

				tmpDrawable.srcR.w = tmpDrawable.destR.w = width;
				tmpDrawable.srcR.x = tmpDrawable.srcR.y = 0;
				tmpDrawable.srcR.h = tmpDrawable.destR.h = 48;
				tmpDrawable.destR.y = 20;

				tmpDrawable.destR.x = padding + 198 / 2 - width / 2;
				scoreDrawables.at(0) = tmpDrawable;
				tmpDrawable.destR.x = WINDOW_WIDTH - padding - 252 / 2 - width / 2;
				scoreDrawables.at(1) = tmpDrawable;
			}
			else {
				std::cout << "3" << std::endl;
				std::string url = getUrl(dealerScore);
				SDL_Surface* tmpSurface = IMG_Load(url.c_str());
				scoreDrawables.at(1).texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
				SDL_FreeSurface(tmpSurface);
				int width = getImageWidth(dealerScore);

				scoreDrawables.at(1).srcR.w = scoreDrawables.at(1).destR.w = width;

				scoreDrawables.at(1).destR.x = WINDOW_WIDTH - padding - 252 / 2 - width / 2;
			}
		}
	}
	else {
		if (playerScore > 21){
			gameOver(false, false);
		}
		//someones score is more then 21 (busted)
		std::cout << "1" << std::endl;
		Drawable tmpDrawable;
		SDL_Surface* tmpSurface = IMG_Load("assets/score/gameover.png");

		tmpDrawable.texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

		SDL_FreeSurface(tmpSurface);
		int width = 189;

		tmpDrawable.srcR.w = tmpDrawable.destR.w = width;
		tmpDrawable.srcR.x = tmpDrawable.srcR.y = 0;
		tmpDrawable.srcR.h = tmpDrawable.destR.h = 48;
		tmpDrawable.destR.y = 20;

		tmpDrawable.destR.x = padding + 198 / 2 - width / 2;
		scoreDrawables.at(0) = tmpDrawable;
		tmpDrawable.destR.x = WINDOW_WIDTH - padding - 252 / 2 - width / 2;
		scoreDrawables.at(1) = tmpDrawable;
	}
}

std::string Game::getUrl(int score){
	std::string url;
	switch (score){
	case 1:
		url = "assets/score/1.png";
		break;
	case 2:
		url = "assets/score/2.png";
		break;
	case 3:
		url = "assets/score/3.png";
		break;
	case 4:
		url = "assets/score/4.png";
		break;
	case 5:
		url = "assets/score/5.png";
		break;
	case 6:
		url = "assets/score/6.png";
		break;
	case 7:
		url = "assets/score/7.png";
		break;
	case 8:
		url = "assets/score/8.png";
		break;
	case 9:
		url = "assets/score/9.png";
		break;
	case 10:
		url = "assets/score/10.png";
		break;
	case 11:
		url = "assets/score/11.png";
		break;
	case 12:
		url = "assets/score/12.png";
		break;
	case 13:
		url = "assets/score/13.png";
		break;
	case 14:
		url = "assets/score/14.png";
		break;
	case 15:
		url = "assets/score/15.png";
		break;
	case 16:
		url = "assets/score/16.png";
		break;
	case 17:
		url = "assets/score/17.png";
		break;
	case 18:
		url = "assets/score/18.png";
		break;
	case 19:
		url = "assets/score/19.png";
		break;
	case 20:
		url = "assets/score/20.png";
		break;
	case 21:
		url = "assets/score/21.png";
		break;
	default:
		url = "assets/score/gameover.png";
		break;
	}
	return url;
}

int Game::getImageWidth(int score)
{
	switch (score){
	case 1:
		return 159;
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 9:
		return 165;
	case 8:
		return 161;
	case 10:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
	case 21:
		return 177;
	case 11:
		return 171;
	case 20:
		return 183;
	default:
		return 189;
	}
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