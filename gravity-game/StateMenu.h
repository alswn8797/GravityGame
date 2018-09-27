#ifndef STATEMENU
#define STATEMENU

#include "Globals.h"
#include "DrawingFunctions.h"
#include "GameState.h"
#include "StateGame.h"

class StateMenu :
	public GameState
{
public:
	StateMenu();
	~StateMenu();

	Mix_Music* music;

	SDL_Texture* crowTexture;
	Animation* crowAnimation;
	Crow* crow;

	SDL_Texture* TitleTexture;
	SDL_Texture* ButtonTextrue;
	SDL_Texture* BgTexture;

	//override those abstract methods inherited
	virtual void update();
	virtual void render();
	virtual bool onEnter();
	virtual bool onExit();
	virtual string getStateID(){
		return "menuState";
	}
};

#endif