#pragma once
#include"GameObject.h"
#include "Animation.h"

class Crow : public GameObject
{
private:
	Animation* animation = NULL;
	bool faceRight = true;
	bool loop = false;

public:
	Crow();
	~Crow();

	void setLoop(bool loop);
	void setAnimation(Animation* animation);

	//override GameObject methods
	virtual void update(float dt);
	virtual void draw();
};

