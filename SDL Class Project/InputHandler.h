#pragma once
#include <SDL.h>
//#include "Hero.h"
class InputHandler
{
public:
	//Hero* hero;

	InputHandler();
	~InputHandler();

	//abstract function
	virtual void update(SDL_Event* event) = 0;//<==== abstract function has no body, in c++ we show this with =0
};

