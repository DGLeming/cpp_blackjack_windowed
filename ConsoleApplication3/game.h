#include "SDL.h"
#include "SDL_image.h"
#include <array>
#include <vector>
#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

enum DrwblsTypes
{
	DEFAULT,
	DRAWCARD,
	STAND,
	YES,
	NO
};

struct Drawable
{
	SDL_Rect srcR,destR;
	SDL_Texture* texture;
	DrwblsTypes type;
};

enum CardSuit
{
	SUIT_CLUB,
	SUIT_DIAMOND,
	SUIT_HEART,
	SUIT_SPADE,
	MAX_SUITS
};
enum CardRank
{
	RANK_2,
	RANK_3,
	RANK_4,
	RANK_5,
	RANK_6,
	RANK_7,
	RANK_8,
	RANK_9,
	RANK_10,
	RANK_JACK,
	RANK_QUEEN,
	RANK_KING,
	RANK_ACE,
	MAX_RANKS
};

struct Card{
	SDL_Texture* texture;
	SDL_Rect cardDest;
	bool owner;
};

struct PlayCard{
	CardSuit suit;
	CardRank rank;
	bool dealed = false;
};

class Game {
public:
	Game();
	~Game();
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEnents();
	bool inline running(){ return isRunning; };
	void update();
	void render();
	void clean();
	void addCard(const char* path, int posx, int posy, bool own);
	std::array<PlayCard, 52> getShuffledDeck();
	int getRandomNumber(int min, int max);
	void swapCards(PlayCard &card1, PlayCard &card2);
	std::string getCardAddres(const PlayCard card);
	void addDrawable(const char* path, int posx, int posy, int width, int height, DrwblsTypes type);
	void getCard(std::array<PlayCard, 22> &hand, bool player);
	void cardsRedraw(bool player);
	int getImageWidth(int score);
	std::string getUrl(int score);
	void gameOver(bool winner, bool tie);
	void newgame();
	void initForNewGame();

private:
	bool isRunning;
	SDL_Window *window;
	SDL_Renderer *renderer;
	Card deck[52];
	int cardsInDeck = 0;
	SDL_Rect cardSrc;
	int cardWidth = 70;
	int cardHeight = 101;
	std::array<PlayCard, 52> playdeck;
	std::vector<Drawable> drawbls;
	std::vector<Drawable> scoreDrawables;
	std::array<PlayCard, 22> playerHand, dealerHand;
	int playerCards = 0;
	int dealerCards = 0;
	int playerScore = 0;
	int dealerScore = 0;
	int playerAces = 0;
	int dealerAces = 0;
};