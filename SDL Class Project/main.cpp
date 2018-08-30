#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Animation.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"

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
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, "GRAVITY GAME", textcolour);
	//convert surface to texture
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	//dont need the surface no more
	SDL_FreeSurface(textSurface);

	//setup rectangle to describe where to draw this text
	SDL_Rect textDestination;
	textDestination.x = 50;
	textDestination.y = 50;
	//to get the width and height, query the surface
	SDL_QueryTexture(textTexture, NULL, NULL, &textDestination.w, &textDestination.h);

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
	buttonDestination.x = 400;
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
	SDL_Texture* trumpTexture = IMG_LoadTexture(renderer, "assets/trump_run.png");
	//setup run Source rect to focus on first frame
	SDL_Rect trumpSrcRect = { 0, 0, 100, 100 };
	//destination rect for runner
	SDL_Rect trumpDstRect = { 0, 0, 100, 100 };

	//Load up Run Image
	SDL_Texture* runTexture = IMG_LoadTexture(renderer, "assets/run.png");
	//setup run Source rect to focus on first frame
	SDL_Rect runSrcRect = { 0, 0, 32, 32 };
	//destination rect for runner
	SDL_Rect runDstRect = { 100, 100, 128, 128 };


	//setup run Source rect to focus on first frame
	SDL_Rect noBGrunSrcRect = { 0, 0, 32, 32 };
	//destination rect for runner
	SDL_Rect noBGrunDstRect = { 300, 350, 100, 100 };

	//To make a colour transparent, needs to be a surface first
	SDL_Surface* runSurface = IMG_Load("assets/run.png");
	//Set its colour key (colour you want to be invisible)
	SDL_SetColorKey(runSurface, 1, SDL_MapRGB(runSurface->format, 128, 128, 255));
	//convert it to a texture now
	SDL_Texture* runTextureNoBG = SDL_CreateTextureFromSurface(renderer, runSurface);
	//cleanup surface
	SDL_FreeSurface(runSurface);

	Animation anim1(runTextureNoBG, renderer, 4, 32, 32, 0.04);
	Animation anim2(runTextureNoBG, renderer, 4, 32, 32, 0.1);
	Animation anim3(runTextureNoBG, renderer, 4, 50, 50, 0.2);

	//setup time stuff
	Uint32 lastUpdate = SDL_GetTicks();//milliseconds since the start of the game running

	//current frame (0-3)
	int currentFrame = 0;
	int trumpFrame = 0;
	float frameTimer = 0.083;//83 milliseconds per frame

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

		//timeDiff = 200; //milliseconds

		framesCounted++;
		oneSecondTimer -= dt;//minusing how many milliseconds since last game loop cycle

		//1 second has passed
		if (oneSecondTimer <= 0)
		{
			cout << "FPS = " << framesCounted << " dt = "<<dt<< endl;
			framesCounted = 0;
			oneSecondTimer = 1;
		}

		//set drawing colour for sdl
		//params: which renderer to set colour for, Red, Green, Ble, Alpha where each colour value is 0 - 255
		SDL_SetRenderDrawColor(renderer, 0, 84, 165, 255);
		//clear screen with current draw colour
		SDL_RenderClear(renderer);

		//DRAW text
		//render textTexture
		SDL_RenderCopy(renderer, textTexture, NULL, &textDestination);

		//DRAW button
		//render textTexture
		SDL_RenderCopy(renderer, buttonTexture, NULL, &buttonDestination);

		//DRAW OUR BACKGROUND
		SDL_RenderCopy(renderer, bgTexture, &bgSourceRect, &bgDestinationRect);


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

		//render textTexture
		SDL_RenderCopy(renderer, textTexture, NULL, &textDestination);

		//when done drawing, present all our renderings to the window
		SDL_RenderPresent(renderer);
	}


	//SDL_Delay(5000);//pause code for how many milliseconds

	//cleanup
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(buttonTexture);
	SDL_DestroyTexture(bgTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	//shut down sdl sub systems
	SDL_Quit();

	system("pause");
	return 0;
}