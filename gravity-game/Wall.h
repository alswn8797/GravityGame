#ifndef WALL
#define WALL

#include "GameObject.h"

class Wall :
	public GameObject
{
public:
	Wall();
	Wall(SDL_Renderer* renderer, bool bumpIntoSolids, bool solid, int w, int h, int x, int y);
	~Wall();

	void update(float dt);
	void draw();
};

#endif