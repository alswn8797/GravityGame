#include "Vector.h"

Vector::Vector(){
	x = 0;
	y = 0;
}

Vector::Vector(float x, float y){
	this->x = x;
	this->y = y;
}

Vector::Vector(float w, float h, float x, float y){
	this->w = w;
	this->h = h;
	this->x = x;
	this->y = y;
}