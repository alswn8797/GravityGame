#ifndef STATEGAME
#define STATEGAME

#include "GameState.h"
#include <list>
#include "DrawingFunctions.h"
#include "Crow.h"
#include "Wall.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "GameControllerHandler.h"
#include "Globals.h"

class StateGame :
	public GameState
{
public:
	SDL_Texture* crowFlyTexture;
	Animation* crowFlyAnimation;
	SDL_Texture* crowStandTexture;
	Animation* crowStandAnimation;
	Crow* crow;

	list<GameObject*> gameObjects;

	KeyboardHandler keyboardHandler;
	MouseHandler mouseHandler;
	GameControllerHandler controllerHandler;

	StateGame();
	~StateGame();

	void setCrow();
	void setWalls();

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