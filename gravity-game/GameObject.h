#ifndef GAMEOBJECT
#define GAMEOBJECT

#include <SDL.h>
#include <list>
#include "Vector.h"
#include "timeController.h"
#include <limits>
#include <algorithm>
#include <iostream>

using namespace std;

class GameObject{
protected:
	SDL_Renderer* renderer;
	
public:
	Vector pos; //position
	Vector velocity; //how fast to move in any direction
	Vector maxpos;

	string objectType;

	//FOR COLLISIONS
	bool bumpIntoSolids = false; //do I run into stuff?
	bool solid = false; //do things run into me?
	SDL_Rect collisionBox;

	bool fallen = false; //crow has to fallen when user not press key up

	void setRenderer(SDL_Renderer* renderer);

	Vector getPosition();
	void setPosition(Vector pos);
	Vector getVelocity();
	void setVelocity(Vector velocity);

	virtual void update(float dt);
	virtual void updateMovement(float dt);
	virtual void updateCollisionBox();
	virtual void updateCollisions(float dt);
	virtual void draw();

	float SweptAABB(SDL_Rect movingBox, Vector vec, SDL_Rect otherBox, float &normalX, float &normalY);
	SDL_Rect GetSweptBroadphaseBox(SDL_Rect b, Vector vec);
	bool AABBCheck(SDL_Rect b1, SDL_Rect b2);

	static list<GameObject*> *gameObjects;

	bool checkCollision(SDL_Rect cbox1, SDL_Rect cbox2);
};

#endif