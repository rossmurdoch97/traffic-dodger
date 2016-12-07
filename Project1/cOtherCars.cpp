/*
=================
cOtherCars.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cOtherCars.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cOtherCars::cOtherCars() : cSprite()
{
	this->OtherCarVelocity = { 3.0, 3.0 };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cOtherCars::update(double deltaTime)
{

	/*this->setSpriteRotAngle(this->getSpriteRotAngle() +(5.0f * deltaTime)); 
	if (this->getSpriteRotAngle() > 360)
	{
		this->setSpriteRotAngle(this->getSpriteRotAngle() -360);
	}*/

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->getSpriteTranslation().x * deltaTime *-5;
	currentSpritePos.y += this->getSpriteTranslation().y * deltaTime;
	if (currentSpritePos.x <= 0)
	{ 
		currentSpritePos.x = 1000;
		currentSpritePos.y = rand () % 500 + 100;
	}
	this->setSpritePos({ currentSpritePos.x, currentSpritePos.y });
	//cout << "OtherCar position - x: " << this->getSpritePos().x << " y: " << this->getSpritePos().y << " deltaTime: " << deltaTime << endl;
	this->setBoundingRect(this->getSpritePos());
}
/*
=================================================================
  Sets the velocity for the OtherCar
=================================================================
*/
void cOtherCars::setOtherCarVelocity(SDL_Point OtherCarVel)
{
	OtherCarVelocity = OtherCarVel;
}
/*
=================================================================
  Gets the OtherCar velocity
=================================================================
*/
SDL_Point cOtherCars::getOtherCarVelocity()
{
	return OtherCarVelocity;
}
