#ifndef GLOBALS
#define GLOBALS

#include <string>
#include <iostream>
#include <SDL.h>
#include "SoundManager.h"
#include "RandomNumber.h"
#include "GameStateMachine.h"

using namespace std;

class Globals{
public:
	Globals();
	~Globals();
	//sdl related
	static int screenWidth, screenHeight;
	static SDL_Renderer* renderer;
	static bool quitGame;
	static GameStateMachine gameStateMachine;
	static int score;
};

#endif