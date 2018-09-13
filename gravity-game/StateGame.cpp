#include "StateGame.h"

StateGame::StateGame()
{
	string resPath = getResourcePath();
	crowTexture = loadTexture(resPath + "crow-fly.png", Globals::renderer);
	crowAnimation = new Animation(crowTexture, Globals::renderer, 6, 60, 60, 0.8);

	crow = new Crow();
	crow->setAnimation(crowAnimation);
	crow->setRenderer(Globals::renderer);
	crow->pos.x = 200;
	crow->pos.y = 200;

	//add them to the list of game objects
	gameObjects.push_back(crow);
	
	keyboardHandler.hero = crow;
	mouseHandler.hero = crow;

	lastUpdate = SDL_GetTicks();
	
}


StateGame::~StateGame()
{
	//cleanup dynamic memory
	delete crow;
	delete crowAnimation;
	cleanup(crowTexture);
}


void StateGame::update(){
	//update time management stuff
	Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
	dt = timeDiff / 1000.0;
	lastUpdate = SDL_GetTicks();

	//deal with events
	SDL_Event event;

	while (SDL_PollEvent(&event)){
		//check if user has closed window
		if (event.type == SDL_QUIT){
			//exit loop
			Globals::quitGame = true;
			return;
		}
		//check if keyboard button pressed
		if (event.type == SDL_KEYDOWN){
			//check if ESC key pressed
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
				//exit loop
				Globals::quitGame = true;
				return;
			}
			//space barrr
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && event.key.repeat == 0){
				//SoundManager::soundManager.playSound("explode");
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_P && event.key.repeat == 0){
				//SoundManager::soundManager.playSound("powerup");
			}
		}
		//pass this event into our hero input handlers
		if (controllerHandler.controller != NULL)
			controllerHandler.update(&event);
		else
			keyboardHandler.update(&event);


		mouseHandler.update(&event);
	}

	for each (GameObject* go in gameObjects)
	{
		go->update(dt);
	}
}
void StateGame::render(){
	SDL_SetRenderDrawColor(Globals::renderer, 255, 90, 0, 255);
	SDL_RenderClear(Globals::renderer);
	for each (GameObject* go in gameObjects)
	{
		go->draw();
	}

	SDL_RenderPresent(Globals::renderer);
}
bool StateGame::onEnter(){
	cout << "enter play game state" << endl;
	return true;

}
bool StateGame::onExit(){
	cout << "exit play game state" << endl;
	return true;
}