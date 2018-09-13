#include "Globals.h"

Globals::Globals()
{
}

Globals::~Globals()
{
}

//sdl related
int Globals::ScreenWidth = 800, Globals::ScreenHeight = 600;
SDL_Renderer* Globals::renderer = NULL;
bool Globals::quitGame = false;
GameStateMachine Globals::gameStateMachine;