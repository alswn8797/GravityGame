#include "Wall.h"

Wall::Wall()
{
	bumpIntoSolids = false;
	solid = true;
	collisionBox.w = 32;
	collisionBox.h = 32;
}

Wall::Wall(SDL_Renderer* renderer, bool bumpIntoSolids, bool solid, int w, int h, int x, int y)
{
	this->renderer = renderer;
	this->bumpIntoSolids = bumpIntoSolids;
	this->solid = solid;
	this->collisionBox.w = w;
	this->collisionBox.h = h;
	this->pos.x = x;
	this->pos.y = y;
}

Wall::~Wall()
{
}

void Wall::update(float dt)
{
	updateCollisionBox();
}

void Wall::draw()
{
	SDL_SetRenderDrawColor(renderer, 123, 58, 0, 255);
	SDL_RenderFillRect(renderer, &collisionBox);
}
