#ifndef STATEGAME
#define STATEGAME

#include "GameState.h"
#include <list>
#include "DrawingFunctions.h"
#include "Crow.h"
#include "Wall.h"
#include "Coin.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "GameControllerHandler.h"
#include "Globals.h"
#include "GameObject.h"

class StateGame :
	public GameState
{
public:
	SDL_Texture* crowFlyTexture;
	Animation* crowFlyAnimation;
	SDL_Texture* crowStandTexture;
	Animation* crowStandAnimation;
	Crow* crow;

	SDL_Texture* coinTexture;
	Animation* coinAnimation;

	list<GameObject*> gameObjects;
	list<GameObject*> coins;

	KeyboardHandler keyboardHandler;
	MouseHandler mouseHandler;
	GameControllerHandler controllerHandler;

	int level;

	StateGame();
	~StateGame();

	void setCrow();
	void setCoins();
	void setWalls();

	void changeLevel(int level);

	//override those abstract methods inherited
	virtual void update();
	virtual void render();
	virtual bool onEnter();
	virtual bool onExit();
	virtual string getStateID(){
		return "StatePlay";
	}
};

#endif