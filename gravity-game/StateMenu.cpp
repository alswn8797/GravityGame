#include "StateMenu.h"

StateMenu::StateMenu()
{
}

StateMenu::~StateMenu()
{
}

void StateMenu::update(){

	SDL_Event e;
	while (SDL_PollEvent(&e)){
		//event fired when pressing window close button
		if (e.type == SDL_QUIT){
			Globals::quitGame = true;
			Globals::gameStateMachine.popState();
			return;
		}
		if (e.type == SDL_KEYDOWN){
			if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
				Globals::quitGame = true;
				Globals::gameStateMachine.popState();
				return;
			}
			if (e.key.keysym.scancode == SDL_SCANCODE_SPACE){
				//start game
				Globals::gameStateMachine.pushState(new StateGame());
			}
		}
	}
}
void StateMenu::render(){
	SDL_SetRenderDrawColor(Globals::renderer, 173, 0, 0, 255);
	SDL_RenderClear(Globals::renderer);
	SDL_RenderPresent(Globals::renderer);
}
bool StateMenu::onEnter(){
	cout << "Entering menu state" << endl;
	return true;
}
bool StateMenu::onExit(){
	cout << "Exiting menu state" << endl;
	return true;
}