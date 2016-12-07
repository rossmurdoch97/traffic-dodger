/*
=================
cOtherCar.h
- Header file for class definition - SPECIFICATION
- Header file for the player class which is a child of cSprite class
=================
*/
#ifndef _COtherCar_H
#define _COtherCar_H
#include "cSprite.h"

class cOtherCars : public cSprite
{
private:
	SDL_Point OtherCarVelocity;

public:
	cOtherCars();
	void update(double deltaTime);		// player update method
	void setOtherCarVelocity(SDL_Point OtherCarVel);   // Sets the velocity for the OtherCar
	SDL_Point getOtherCarVelocity();				 // Gets the OtherCar velocity
};
#endif