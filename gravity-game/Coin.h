#ifndef COIN
#define COIN

#include "GameObject.h"
#include "Animation.h"
#include "SoundManager.h"

class Coin : public GameObject
{
private:
	Animation* animation = NULL;

public:
	Coin();
	~Coin();

	void setAnimation(Animation* animation);

	//override GameObject methods
	virtual void update(float dt);
	virtual void draw();
};

#endif