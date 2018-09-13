#ifndef STATEMENU
#define STATEMENU

#include "Globals.h"
#include "GameState.h"
#include "StateGame.h"

class StateMenu :
	public GameState
{
public:
	StateMenu();
	~StateMenu();

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