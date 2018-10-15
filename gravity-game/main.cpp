#include "CleanUp.h"
#include "ResPath.h"
#include "DrawingFunctions.h"
#include "Globals.h"
#include "StateMenu.h"

using namespace std;

int main(int argc, char **argv){

	//initialise SDL with the subsystems you wish to use
	//ie SDL_INIT_VIDEO, but we'll launch all subsystems (input, video, etc)
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL failed to initialise" << endl;
		return -1;
	}
	else {
		cout << "SDL successfully initialised!" << endl;
	}
	
	//Initialise SDL_Image
	if (!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG))
	{
		cout << "sdl image did not load: " << IMG_GetError() << endl;
		SDL_Quit();
		system("pause");
		return -1;
	}

	//Initialise TTF
	if (TTF_Init() != 0){
		//if failed
		cout << "SDL TTF FAILED!" << endl;
		SDL_Quit();
		system("pause");
		return -1;
	}

	//INIT mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1){
		cout << "Mixer didn't initialise" << endl;
		SDL_Quit();
		system("pause");
		return -1;
	}

	//setup window
	SDL_Window* window = SDL_CreateWindow("Gravity Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Globals::screenWidth, Globals::screenHeight, SDL_WINDOW_SHOWN); //SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN

	if (window == NULL)
	{
		SDL_Quit();
		cout << "Window failed!" << endl;
		return -1;
	}

	//setup renderer
	Globals::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Globals::renderer == NULL){
		cleanup(window);
		SDL_Quit();
		cout << "renderer error" << endl;
		return 1;
	}
	//this is the size to draw things at, before we scale it to the screen size dimensions mentioned in createWindow
	SDL_RenderSetLogicalSize(Globals::renderer, Globals::screenWidth, Globals::screenHeight);

	//start on menu screen
	Globals::gameStateMachine.pushState(new StateMenu());

	bool loop = true;

	while (loop)
	{

		Globals::gameStateMachine.update();
		Globals::gameStateMachine.render();

		if (Globals::quitGame || Globals::gameStateMachine.gameStates.empty())
			loop = false;
	}

	//clean up any extra screen states
	Globals::gameStateMachine.clearAll();

	cleanup(Globals::renderer);
	cleanup(window);

	SDL_Quit();
	return 0;
}