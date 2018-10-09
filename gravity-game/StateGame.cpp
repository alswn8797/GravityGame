#include "StateGame.h"

StateGame::StateGame()
{
	TimeController::timeController.reset();
	GameObject::gameObjects = &gameObjects;

	setCrow();
	
	keyboardHandler.hero = crow;
	mouseHandler.hero = crow;

	setWalls();

}

StateGame::~StateGame()
{
	//cleanup dynamic memory
	delete crow;
	delete crowStandAnimation;
	delete crowFlyAnimation;
	cleanup(crowStandTexture);
	cleanup(crowFlyTexture);
}

void StateGame::update(){

	//update time management stuff
	TimeController::timeController.updateTime();

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
		go->update(TimeController::timeController.dT);
	}
}

void StateGame::render(){
	TimeController::timeController.reset();

	SDL_SetRenderDrawColor(Globals::renderer, 0, 84, 165, 255);
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

void StateGame::setCrow(){
	string resPath = getResourcePath();
	crowFlyTexture = loadTexture(resPath + "crow-fly.png", Globals::renderer);
	crowFlyAnimation = new Animation(crowFlyTexture, Globals::renderer, 6, 44, 60, 0.04);
	crowStandTexture = loadTexture(resPath + "crow-stand.png", Globals::renderer);
	crowStandAnimation = new Animation(crowStandTexture, Globals::renderer, 11, 70, 70, 0.04);

	crow = new Crow();
	crow->setAnimation(crowFlyAnimation, crowStandAnimation);
	crow->setMultipleAnimation(true);
	crow->setRenderer(Globals::renderer);
	crow->pos.x = 0;
	crow->pos.y = Globals::ScreenHeight - 70;

	//add them to the list of game objects
	gameObjects.push_back(crow);
}

void StateGame::setWalls(){

	//Make some walls                                   //  w    h    x   y
	Wall *wall1 = new Wall(Globals::renderer, true, true, 200, 400, 200, 200);
	Wall *wall2 = new Wall(Globals::renderer, true, true, 200, 200, 400, 400);

	//for (i = 0;)
	//add them to the list of game objects
	gameObjects.push_back(wall1);
	gameObjects.push_back(wall2);
}