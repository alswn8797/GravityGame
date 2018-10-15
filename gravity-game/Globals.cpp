#include "Globals.h"

Globals::Globals()
{
}

Globals::~Globals()
{
}

//sdl related
int Globals::screenWidth = 800, Globals::screenHeight = 600;
SDL_Renderer* Globals::renderer = NULL;
bool Globals::quitGame = false;
GameStateMachine Globals::gameStateMachine;
int Globals::score;