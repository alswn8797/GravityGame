#ifndef KEYBOARDHANDLER
#define KEYBOARDHANDLER

#include "InputHandler.h"
class KeyboardHandler :
	public InputHandler
{
public:
	KeyboardHandler();
	~KeyboardHandler();

	void update(SDL_Event* event);
};

#endif