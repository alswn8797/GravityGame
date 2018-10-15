#include "StateGame.h"

StateGame::StateGame()
{
	string resPath = getResourcePath();
	TimeController::timeController.reset();
	GameObject::gameObjects = &gameObjects;
	//load up sounds
	SoundManager::soundManager.loadSound("coin", resPath + "coin.wav");

	setCoins();
	setCrow();
	setWalls();

	keyboardHandler.hero = crow;
	mouseHandler.hero = crow;
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
		if (go->fallen){
			//go->velocity.y = 80;
			cout << go->pos.y << endl;
			cout << go->velocity.y << endl;
		}
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
	crowStandAnimation = new Animation(crowStandTexture, Globals::renderer, 11, 70, 70, 0.3);

	crow = new Crow();
	crow->setAnimation(crowFlyAnimation, crowStandAnimation);
	crow->setMultipleAnimation(true);
	crow->setRenderer(Globals::renderer);
	crow->pos.x = 100;
	crow->pos.y = 480;
	crow->objectType = "hero";
	crow->fallen = true;

	//add them to the list of game objects
	gameObjects.push_back(crow);
}

void StateGame::setCoins(){
	string resPath = getResourcePath();

	list<Vector*> pos{ new Vector(300, 150), new Vector(400, 100), new Vector(500, 150) };
	coinTexture = loadTexture(resPath + "coin.png", Globals::renderer);
	coinAnimation = new Animation(coinTexture, Globals::renderer, 7, 40, 40, 0.1);

	for each (Vector* posVec in pos){
		Coin* coin = new Coin();
		coin->setRenderer(Globals::renderer);
		coin->setAnimation(coinAnimation);
		coin->pos.x = posVec->x;
		coin->pos.y = posVec->y;
		gameObjects.push_back(coin);
	}
}

void StateGame::setWalls(){
	int boundWidth = 50;

	list<Vector*> wallVecs{ new Vector(boundWidth, Globals::screenHeight, 0, 0),
		new Vector(boundWidth, Globals::screenHeight, Globals::screenWidth - boundWidth, 0),
		new Vector(Globals::screenWidth, boundWidth, 0, 0),
		new Vector(Globals::screenWidth, boundWidth, 0, Globals::screenHeight - boundWidth),
		new Vector(300, 150, 200, 450),
		new Vector(300, 300, 500, 300)
	};
	
	for each (Vector* vec in wallVecs){
		Wall* wall = new Wall();
		wall->setRenderer(Globals::renderer);
		wall->pos.x = vec->x;
		wall->pos.y = vec->y;
		wall->collisionBox.w = vec->w;
		wall->collisionBox.h = vec->h;
		gameObjects.push_back(wall);
	}


	//for (i = 0;)
	//add them to the list of game objects
}

void StateGame::changeLevel(int level){

}