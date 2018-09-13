#ifndef INPUTHANDLER
#define INPUTHANDLER

#include <SDL.h>
#include "Crow.h"
class InputHandler
{
public:
	Crow* hero;

	InputHandler();
	~InputHandler();

	//abstract function
	virtual void update(SDL_Event* event) = 0;//<==== abstract function has no body, in c++ we show this with =0
};

#endif