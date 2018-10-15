#include "Coin.h"

Coin::Coin()
{
	bumpIntoSolids = false;
	solid = false;
	collisionBox.w = 40;
	collisionBox.h = 40;
}

Coin::~Coin()
{
}

void Coin::setAnimation(Animation* animation){
	this->animation = animation;
}

void Coin::update(float dt)
{
	updateCollisionBox();
	if (this->animation != NULL){
		animation->update(dt);
	}
	
	for each (GameObject* go in *GameObject::gameObjects){
		if (go->objectType == "hero"){
			GameObject* hero = go;
			if (this->animation != NULL && GameObject::checkCollision(collisionBox, hero->collisionBox)){
				SoundManager::soundManager.playSound("coin");
				this->animation = NULL;
			}
		}
	}
}

void Coin::draw()
{
	SDL_RenderFillRect(renderer, &collisionBox);

	if (animation != NULL)
	{
		animation->draw(pos.x, pos.y);
	}
}
