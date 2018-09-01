#include "Crow.h"


Crow::Crow()
{
}

Crow::~Crow()
{
}

void Crow::setLoop(bool loop){
	this->loop = loop;
}

void Crow::setAnimation(Animation* animation){
	this->animation = animation;
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
	//move character based on velocity
	updateMovement(dt);
	//update character animation
	animation->update(dt);
}
void Crow::draw(){
	if (animation != NULL)
	{
		if (faceRight)
			animation->draw(pos.x, pos.y);
		else
			animation->draw(pos.x, pos.y, true);//flip image to face left
	}
}