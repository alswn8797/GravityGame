#include "Crow.h"

Crow::Crow()
{
	bumpIntoSolids = true;
	collisionBox.w = 50;
	collisionBox.h = 70;
}

Crow::~Crow()
{
}

void Crow::setMultipleAnimation(bool multipleAnimation){
	this->multipleAnimation = multipleAnimation;
}

void Crow::setLoop(bool loop){
	this->loop = loop;
}

void Crow::setAnimation(Animation* animation){
	this->animation = animation;
}

void Crow::setAnimation(Animation* flyingAnimation, Animation* standingAnimation){
	this->flyingAnimation = flyingAnimation;
	this->standingAnimation = standingAnimation;
}

void Crow::update(float dt){
	//if moving on the X axis, change facing
	if (velocity.x > 0)
		faceRight = true;
	else if (velocity.x < 0)
		faceRight = false;

	if (loop){
		if (pos.x >= maxpos.x + (float)animation->frameWidth){
			pos.x = 0 - (float)animation->frameWidth;
		}
	}
	//before we move our beloved hero, we'll see if they crash into anything on their predicted path
	updateCollisions(dt);
	//move character based on velocity
	updateMovement(dt);
	//update character animation
	animation->update(dt);
}
void Crow::draw(){
	//SDL_SetRenderDrawColor(renderer, 120, 46, 22, 0);
	SDL_RenderFillRect(renderer, &collisionBox);

	if (multipleAnimation){
		if (animationState == 1)
			setAnimation(flyingAnimation);
		else
			setAnimation(standingAnimation);
	}

	if (animation != NULL)
	{
		if (faceRight)
			animation->draw(pos.x, pos.y);
		else
			animation->draw(pos.x, pos.y, true);//flip image to face left
	}
}