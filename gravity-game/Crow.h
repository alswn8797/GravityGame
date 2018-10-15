#ifndef CROW
#define CROW

#include "GameObject.h"
#include "Animation.h"

class Crow : public GameObject
{
private:
	Animation* animation = NULL;
	Animation* flyingAnimation = NULL;
	Animation* standingAnimation = NULL;

	bool multipleAnimation = false; //if false then only one animation is needed in the state
	int animationState = 1; //1 = standing, 2 = flying
	int power = 100;

	bool faceRight = true;
	bool loop = false;

public:
	Crow();
	~Crow();

	void setMultipleAnimation(bool multipleAnimation);
	void setLoop(bool loop);
	void setAnimationState(int state);
	void setAnimation(Animation* animation);
	void setAnimation(Animation* flyingAnimation, Animation* standingAnimation);

	//override GameObject methods
	virtual void update(float dt);
	virtual void draw();
};

#endif