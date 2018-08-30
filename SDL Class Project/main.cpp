#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "Animation.h"

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

	//Lets create a window to draw into
	//params: title of window
	//		  x and y of where to put this window (we are just centering it)
	//	      width and height of window in pixels
	//		  flags to help decide what type of window to use
	SDL_Window* window = SDL_CreateWindow("My SDL2 Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600, SDL_WINDOW_SHOWN); //for full screen use SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN

	//did it work?
	if (window != NULL)
		cout << "Window created!" << endl;
	else
	{
		cout << "Window failed!" << endl;
		return -1;
	}

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
	//build stuff
	//load up a surface
	SDL_Surface* knightSurface = SDL_LoadBMP("assets/knight.bmp");
	//convert it to a texture
	SDL_Texture* knightTexture = SDL_CreateTextureFromSurface(renderer, knightSurface);
	//dont need surface anymore, clean up its memory
	SDL_FreeSurface(knightSurface);

	//which region of the texture to copy from
	SDL_Rect knightSourceRect;
	//region on the screen to copy source to (can be different sizes, will stretch)
	SDL_Rect knightDestinationRect;

	//query can be used to gain information about a texture, we're using it to get width and height info
	SDL_QueryTexture(knightTexture, NULL, NULL, &knightSourceRect.w, &knightSourceRect.h);
	cout << "The knight is " << knightSourceRect.w << " by " << knightSourceRect.h << "pixels" << endl;

	knightSourceRect.x = 0;
	knightSourceRect.y = 0;
	//destination
	//make it the same size as the image but draw it 50 pixels down and across
	knightDestinationRect.x = 50;
	knightDestinationRect.y = 50;
	knightDestinationRect.w = knightSourceRect.w;
	knightDestinationRect.h = knightSourceRect.h;
	//Lets screw up a few things
	knightSourceRect.x = 0;
	knightSourceRect.y = 0;
	knightSourceRect.w = 200;
	knightSourceRect.h = 241;

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


	bool loop = true;
	while (loop)
	{
		//TIME!!!!
		//===================
		//get difference between currentTime running minus the last update time
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		//convert to deltaTime (decimal)
		float dt = timeDiff / 1000.0;//e.g 200ms is now represented as 0.2 for dt
		//update lastUpdate to currentTime
		lastUpdate = SDL_GetTicks();
		//lets see dt per frame
		//cout << "dt = " << dt << endl;

		//End Loop Condition
		if (lastUpdate > 10000)
			loop = false; //run for 10 seconds


		//set drawing colour for sdl
		//params: which renderer to set colour for, Red, Green, Ble, Alpha where each colour value is 0 - 255
		SDL_SetRenderDrawColor(renderer, 255, 0, 168, 255);
		//clear screen with current draw colour
		SDL_RenderClear(renderer);

		//LETS DRAW A RECTANGLE TO THE SCREEN
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		//SDL_Rect has a x,y,w,h
		SDL_Rect rect = { 100, 50, 300, 240 };

		//draw to screen using current draw colour and use rect for size guidance
		SDL_RenderFillRect(renderer, &rect);

		//DRAW OUR KNIGHT
		SDL_RenderCopy(renderer, knightTexture, &knightSourceRect, &knightDestinationRect);

		//update frameTimer
		frameTimer -= dt;
		if (frameTimer <= 0){
			//time to show next frame in animation
			currentFrame++;
			if (currentFrame > 3)
				currentFrame = 0;//loop frame back to beginning when it gets to the end

			//time to show next frame in animation
			trumpFrame++;
			if (trumpFrame > 5)
				trumpFrame = 0;//loop frame back to beginning when it gets to the end

			//reset frameTimer
			frameTimer = 0.083;

			//change which part of our run texture we are sourcing from
			runSrcRect.x = currentFrame*runSrcRect.w;
			noBGrunSrcRect.x = currentFrame*noBGrunSrcRect.w;

			//change which part of our run texture we are sourcing from
			trumpSrcRect.x = trumpFrame*trumpSrcRect.w;
		}
		
		//DRAW RUN ANIMATION
		SDL_RenderCopy(renderer, runTexture, &runSrcRect, &runDstRect);
		SDL_RenderCopyEx(renderer, runTextureNoBG, &noBGrunSrcRect, &noBGrunDstRect, 0, 0, SDL_FLIP_HORIZONTAL);
		SDL_RenderCopy(renderer, trumpTexture, &trumpSrcRect, &trumpDstRect);
		
		//Draw Anim1
		anim1.update(dt);//updates timers
		anim1.draw(300, 100);
		//Draw Anim2
		anim2.update(dt);
		anim2.draw(350, 100, true);
		//Draw Anim3
		anim2.update(dt);
		anim2.draw(200, 400, true);

		//when done drawing, present all our renderings to the window
		SDL_RenderPresent(renderer);
	}
	//SDL_Delay(5000);//pause code for how many milliseconds

	//cleanup
	SDL_DestroyTexture(knightTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	//shut down sdl sub systems
	SDL_Quit();

	system("pause");
	return 0;
}