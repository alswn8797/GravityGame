#ifndef STATEGAME
#define STATEGAME

#include "GameState.h"
#include <list>
#include "DrawingFunctions.h"
#include "Crow.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "GameControllerHandler.h"
#include "Globals.h"

class StateGame :
	public GameState
{
public:
	SDL_Texture* crowTexture;
	Animation* crowAnimation;
	Crow* crow;

	list<GameObject*> gameObjects;

	KeyboardHandler keyboardHandler;
	MouseHandler mouseHandler;
	GameControllerHandler controllerHandler;

	Uint32 lastUpdate; //last sdl_getTicks();
	float dt = 0;

	StateGame();
	~StateGame();

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