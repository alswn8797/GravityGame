#include "GameObject.h"

void GameObject::setRenderer(SDL_Renderer* renderer){
	this->renderer = renderer;
}

Vector GameObject::getPosition(){
	return pos;
}
void GameObject::setPosition(Vector pos){
	this->pos = pos;
}
Vector GameObject::getVelocity(){
	return velocity;
}
void GameObject::setVelocity(Vector velocity){
	this->velocity = velocity;
}

void GameObject::update(float dt){
	//nuthin
}

void GameObject::updateMovement(float dt){
	pos.x = pos.x + (velocity.x*dt);
	pos.y = pos.y + (velocity.y*dt);
}

void GameObject::updateCollisionBox()
{
	collisionBox.x = pos.x;
	collisionBox.y = pos.y;
}
void GameObject::updateCollisions(float dt)
{
	//make sure collisionBox is set to the right position before working out all sorts of maths
	updateCollisionBox();

	if (gameObjects != NULL && bumpIntoSolids && (velocity.x != 0 || velocity.y != 0))
	{
		//collision time, 0 is immediately, 1 is not at all. We want to find out where on this boxes movement path where it crashes
		float collisionTime = 1;
		//these are to help work out which side we are hitting an object on
		float normalx, normaly;
		//we only want to compare velocity based on this times movement
		Vector velocityXdt(velocity.x*dt, velocity.y*dt);

		SDL_Rect broadphasebox = GetSweptBroadphaseBox(collisionBox, velocity);

		//loop through entire list to see if we hit anyone
		for (list<GameObject*>::iterator e = gameObjects->begin(); e != gameObjects->end(); e++) {

			if ((*e) == this || !(*e)->solid || !AABBCheck(broadphasebox, (*e)->collisionBox))
				continue;

			//check to see if we actually collide. If we do, then tmpCollisionTime will be < 1
			float tmpNormalx, tmpNormaly;
			float tmpCollisionTime = SweptAABB(collisionBox, velocityXdt, (*e)->collisionBox, tmpNormalx, tmpNormaly);

			//is this collision closer then the last?
			if (tmpCollisionTime < collisionTime)
			{
				collisionTime = tmpCollisionTime;
				normalx = tmpNormalx;
				normaly = tmpNormaly;
			}
		}
		//after the loop

		//if there was a collision, lets setup sliding to the new position instead
		if (collisionTime < 1.0f) {
			//make velocity proportion to where we bump into someone
			velocity.x = velocity.x*collisionTime;
			velocity.y = velocity.y*collisionTime;

			//this alone is good enough, it means we run up to an object, but more likely you'll wanna slide against it if possible
			float remainingtime = 1.0f - collisionTime;
			/*if slide*/
			//work out dotProduct using remaining time
			float dotprod = (velocity.x * normaly + velocity.y * normalx) * remainingtime;
			//now change the velocity based on the dotproduct and normals
			velocity.x = dotprod * normaly;
			velocity.y = dotprod * normalx;

			//technically we could slide into danger again, so we'll run this method again
			updateCollisions(dt);
		}
	}
}

void GameObject::draw(){
	//nada
}

//returns 0-1 of how far it got from startingPos to destinationPos
//params: movingBox is the one we are trying to move through space using xMove and yMove(vx and yx), otherBox is a potential collision, normalx and y gives us which side we've crashed into if we hit
float GameObject::SweptAABB(SDL_Rect movingBox, Vector vec, SDL_Rect otherBox, float &normalX, float &normalY) {
https://www.gamedev.net/articles/programming/general-and-gameplay-programming/swept-aabb-collision-detection-and-response-r3084/?page=2&sort=newest&tab=reviews#review-12559

	float xInvEntry, yInvEntry;
	float xInvExit, yInvExit;

	// find the distance between the objects on the near and far sides for both x and y
	if (vec.x > 0.0f)
	{
		xInvEntry = otherBox.x - (movingBox.x + movingBox.w);
		xInvExit = (otherBox.x + otherBox.w) - movingBox.x;
	}
	else
	{
		xInvEntry = (otherBox.x + otherBox.w) - movingBox.x;
		xInvExit = otherBox.x - (movingBox.x + movingBox.w);
	}

	if (vec.y > 0.0f)
	{
		yInvEntry = otherBox.y - (movingBox.y + movingBox.h);
		yInvExit = (otherBox.y + otherBox.h) - movingBox.y;
	}
	else
	{
		yInvEntry = (otherBox.y + otherBox.h) - movingBox.y;
		yInvExit = otherBox.y - (movingBox.y + movingBox.h);
	}

	// find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
	float xEntry, yEntry;
	float xExit, yExit;

	if (vec.x == 0.0f)
	{
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		xEntry = xInvEntry / vec.x;
		xExit = xInvExit / vec.x;
	}

	if (vec.y == 0.0f)
	{

		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		yEntry = yInvEntry / vec.y;
		yExit = yInvExit / vec.y;
	}

	// find the earliest/latest times of collision
	float entryTime = std::max(xEntry, yEntry);
	float exitTime = std::min(xExit, yExit);

	// if there was no collision
	if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f)
	{
		normalX = 0.0f;
		normalY = 0.0f;
		return 1.0f;
	}
	else // if there was a collision
	{
		system("cls");
		cout << "collision" << endl;
		if (fallen){
			fallen = false;
		}
		// calculate normal of collided surface
		if (xEntry > yEntry)
		{
			if (xInvEntry < 0.0f)
			{
				normalX = 1.0f;
				normalY = 0.0f;
			}
			else
			{
				normalX = -1.0f;
				normalY = 0.0f;
			}
		}
		else
		{
			if (yInvEntry < 0.0f)
			{
				normalX = 0.0f;
				normalY = 1.0f;
			}
			else
			{
				normalX = 0.0f;
				normalY = -1.0f;
			}
		}

		// return the time of collision
		return entryTime;
	}
}

SDL_Rect GameObject::GetSweptBroadphaseBox(SDL_Rect b, Vector vec)
{
	SDL_Rect broadphasebox;
	broadphasebox.x = vec.x > 0 ? b.x : b.x + vec.x;
	broadphasebox.y = vec.y > 0 ? b.y : b.y + vec.y;
	broadphasebox.w = vec.x > 0 ? vec.x + b.w : b.w - vec.x;
	broadphasebox.h = vec.y > 0 ? vec.y + b.h : b.h - vec.y;
	return broadphasebox;
}

bool GameObject::AABBCheck(SDL_Rect b1, SDL_Rect b2)
{
	return !(b1.x + b1.w < b2.x || b1.x > b2.x + b2.w || b1.y + b1.h < b2.y || b1.y > b2.y + b2.h);
}


list<GameObject*> *GameObject::gameObjects = NULL;

bool GameObject::checkCollision(SDL_Rect cbox1, SDL_Rect cbox2){
	SDL_Rect intersection;
	if (SDL_IntersectRect(&cbox1, &cbox2, &intersection))
	{
		return true;
	}

	//if a rectangle is in another rectangle
	//do it here

	return false;
}