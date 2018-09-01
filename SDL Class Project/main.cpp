#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Animation.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "GameObject.h"
#include <list>
#include "Crow.h"

using namespace std;

int main(int argc, char **argv){
	//initialise SDL with the subsystems you wish to use
	//ie SDL_INIT_VIDEO, but we'll launch all subsystems (input, video, etc)
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "SDL failed to initialise" << endl;
		return -1;
	}
	cout << "SDL successfully initialised!" << endl;

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
		system("pause");
		SDL_Quit();
		return -1;
	}

	//Initialse SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1){
		cout << "Mixer did't initialise" << endl;
		SDL_Quit();
		system("pause");
		return -1;
	}

	//Lets create a window to draw into
	//params: title of window
	//		  x and y of where to put this window (we are just centering it)
	//	      width and height of window in pixels
	//		  flags to help decide what type of window to use
	SDL_Window* window = SDL_CreateWindow("Gravity Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600, SDL_WINDOW_SHOWN); //for full screen use SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN

	//did it work?
	if (window != NULL)
		cout << "Window created!" << endl;
	else
	{
		cout << "Window failed!" << endl;
		return -1;
	}

	//Add event hendler
	KeyboardHandler keyboardHandler;
	MouseHandler mouseHandler;

	//Lets build renderer next, its used to help draw stuff to the screen
	//params: window to create renderer for, render driver index(-1, get first best match), flags for what renderer can handle
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//did it work?
	if (renderer != NULL){
		cout << "renderer created!" << endl;
	}
	else
	{
		cout << "renderer failed!" << endl;
		return -1;
	}

	//Load up our font
	//								font file path                font size
	TTF_Font* font = TTF_OpenFont("assets/vermin_vibes_1989.ttf", 100);
	//create a colour for our text
	SDL_Color textcolour = { 255, 255, 255, 0 };//RGBA
	//Create surface using font, colour and desired output text
	SDL_Surface* titleSurface = TTF_RenderText_Blended(font, "GRAVITY GAME", textcolour);
	//convert surface to texture
	SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
	//dont need the surface no more
	SDL_FreeSurface(titleSurface);

	//setup rectangle to describe where to draw this text
	SDL_Rect titleDestination;
	titleDestination.x = 100;
	titleDestination.y = 80;
	//to get the width and height, query the surface
	SDL_QueryTexture(titleTexture, NULL, NULL, &titleDestination.w, &titleDestination.h);

	//load up button text
	//								font file path                font size
	TTF_Font* buttonfont = TTF_OpenFont("assets/Roboto-Black.ttf", 25);
	//create a colour for our text
	//Create surface using font, colour and desired output text
	SDL_Surface* buttonSurface = TTF_RenderText_Blended(buttonfont, "START GAME", textcolour);
	//convert surface to texture
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
	//dont need the surface no more
	SDL_FreeSurface(buttonSurface);

	//setup rectangle to describe where to draw this text
	SDL_Rect buttonDestination;
	buttonDestination.x = 300;
	buttonDestination.y = 350;
	//to get the width and height, query the surface
	SDL_QueryTexture(buttonTexture, NULL, NULL, &buttonDestination.w, &buttonDestination.h);

	//load up background
	SDL_Texture* bgTexture = IMG_LoadTexture(renderer, "assets/main-bg.png");

	//which region of the texture to copy from
	SDL_Rect bgSourceRect;
	//region on the screen to copy source to (can be different sizes, will stretch)
	SDL_Rect bgDestinationRect;

	bgSourceRect.x = 0;
	bgSourceRect.y = 0;
	bgSourceRect.w = 800;
	bgSourceRect.h = 188;
	//destination
	//make it the same size as the image but draw it 50 pixels down and across
	bgDestinationRect.x = 0;
	bgDestinationRect.y = 412;
	bgDestinationRect.w = bgSourceRect.w;
	bgDestinationRect.h = bgSourceRect.h;

	//Load up Run Image
	SDL_Texture* flyCrowTexture = IMG_LoadTexture(renderer, "assets/crow-fly.png");
	Animation flyCrowAnim(flyCrowTexture, renderer, 6, 60, 60, 0.04);

	//LIST OF ALL GAME OBJECTS
	list<GameObject*> gameObjects;

	//Store window width and height
	int screenWidth = SDL_GetWindowSurface(window)->w;
	int screenHeight = SDL_GetWindowSurface(window)->h;

	//BUILD A CROW
	Crow* flyCrow = new Crow();
	flyCrow->setRenderer(renderer);
	flyCrow->setAnimation(&flyCrowAnim);
	flyCrow->setLoop(true);
	flyCrow->maxpos.x = SDL_GetWindowSurface(window)->w;
	flyCrow->pos.x = -60;
	flyCrow->pos.y = 230;
	flyCrow->velocity.x = 200;
	gameObjects.push_back(flyCrow);

	//LOAD UP MUSIC FILE
	Mix_Music* music = Mix_LoadMUS("assets/MenuBGM.wav");
	if (music == NULL){
		cout << "Music failed to load" << endl;
		SDL_Quit();
		system("pause");
		return -1;
	}

	//play song
	//params: music, how many times to play the song (-1 means infinite times)
	Mix_PlayMusic(music, -1);

	//setup time stuff
	Uint32 lastUpdate = SDL_GetTicks();//milliseconds since the start of the game running

	int framesCounted = 0;
	float oneSecondTimer = 1;

	float millisecondsInASecond = 1000;

	bool loop = true;
	while (loop)
	{
		//TIME!!!!
		//===================
		//get difference between currentTime running minus the last update time
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		//convert to deltaTime (decimal)
		float dt = timeDiff / millisecondsInASecond;//e.g 200ms is now represented as 0.2 for dt
		//update lastUpdate to currentTime
		lastUpdate = SDL_GetTicks();
		//lets see dt per frame
		//cout << "dt = " << dt << endl;

		//set drawing colour for sdl
		//params: which renderer to set colour for, Red, Green, Ble, Alpha where each colour value is 0 - 255
		SDL_SetRenderDrawColor(renderer, 0, 84, 165, 255);
		//clear screen with current draw colour
		SDL_RenderClear(renderer);

		//DRAW title
		SDL_RenderCopy(renderer, titleTexture, NULL, &titleDestination);

		//DRAW button
		SDL_RenderCopy(renderer, buttonTexture, NULL, &buttonDestination);

		//DRAW OUR BACKGROUND
		SDL_RenderCopy(renderer, bgTexture, &bgSourceRect, &bgDestinationRect);

		framesCounted++;
		oneSecondTimer -= dt;//minusing how many milliseconds since last game loop cycle

		//1 second has passed
		if (oneSecondTimer <= 0)
		{
			cout << "FPS = " << framesCounted << " dt = " << dt << endl;
			framesCounted = 0;
			oneSecondTimer = 1;
		}

		//CHECK FOR INPUTS
		SDL_Event event;
		//loop through all input events (reuse event object each time, just change values in it)
		while (SDL_PollEvent(&event)){
			//check if user has closed window
			if (event.type == SDL_QUIT){
				//exit loop
				loop = false;
			}
			//check if keyboard button pressed
			if (event.type == SDL_KEYDOWN){
				//check if ESC key pressed
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
					//exit loop
					loop = false;
				}
				//space barrr
				/*if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && event.key.repeat == 0){
				SoundManager::soundManager.playSound("explode");
				}
				*/
			}
			mouseHandler.update(&event);
		}

		//update all game objects in list
		
		for each (GameObject* go in gameObjects)
		{
			go->update(dt);
			go->draw();
		}

		//when done drawing, present all our renderings to the window
		SDL_RenderPresent(renderer);
	}
	
	//stop music from playing
	Mix_PauseMusic();
	//delete song from memory
	Mix_FreeMusic(music); //make sure this bit of music is not playing

	//cleanup
	SDL_DestroyTexture(flyCrowTexture);
	SDL_DestroyTexture(titleTexture);
	SDL_DestroyTexture(buttonTexture);
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	//shut down sdl sub systems
	SDL_Quit();

	system("pause");
	return 0;
}