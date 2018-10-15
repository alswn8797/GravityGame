#include "StateMenu.h"

StateMenu::StateMenu()
{
	TimeController::timeController.reset();

	string resPath = getResourcePath();

	SDL_Color textcolor = { 255, 255, 255, 0 };//RGBA
	//Create surface using font, colour and desired output text
	TitleTexture = renderText("Gravity Game", resPath + "vermin_vibes_1989.ttf", textcolor, 100, Globals::renderer);
	ButtonTextrue = renderText("Press space to start", resPath + "Roboto-Black.ttf", textcolor, 20, Globals::renderer);
	BgTexture = loadTexture(resPath + "main-bg.png", Globals::renderer);

	music = Mix_LoadMUS(string(resPath + "MenuBGM.wav").c_str());
	if (music != NULL)
		Mix_PlayMusic(music, -1);

	crowTexture = loadTexture(resPath + "crow-fly.png", Globals::renderer);
	crowAnimation = new Animation(crowTexture, Globals::renderer, 6, 44, 60, 0.04);

	crow = new Crow();
	crow->setRenderer(Globals::renderer);
	crow->setAnimation(crowAnimation);
	crow->setLoop(true);
	crow->maxpos.x = Globals::screenWidth;
	crow->pos.x = -60;
	crow->pos.y = 230;
	crow->velocity.x = 200;
}

StateMenu::~StateMenu()
{
	Mix_PausedMusic();
	Mix_FreeMusic(music);

	delete crow;
	delete crowAnimation;

	cleanup(crowTexture);
	cleanup(BgTexture);
	cleanup(TitleTexture);
	cleanup(ButtonTextrue);
}

void StateMenu::update(){

	//update time management stuff
	TimeController::timeController.updateTime();

	SDL_Event e;

	while (SDL_PollEvent(&e)){
		
		crow->draw();

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

	crow->update(TimeController::timeController.dT);
}
void StateMenu::render(){

	SDL_SetRenderDrawColor(Globals::renderer, 0, 84, 165, 255);
	SDL_RenderClear(Globals::renderer);
	
	renderTexture(TitleTexture, Globals::renderer, 100, 80);
	renderTexture(ButtonTextrue, Globals::renderer, 300, 350);
	renderTexture(BgTexture, Globals::renderer, 0, 412);

	crow->draw();

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