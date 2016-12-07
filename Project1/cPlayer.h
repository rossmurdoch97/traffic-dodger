/*
=================
cPlayer.h
- Header file for class definition - SPECIFICATION
- Header file for the player class which is a child of cSprite class
=================
*/
#ifndef _Cplayer_H
#define _Cplayer_H
#include "cSprite.h"

class cPlayer : public cSprite
{
private:
	SDL_Point PlayerSpeed;


public:
	cPlayer();
	void update(double deltaTime);		// player update method
	void setPlayerSpeed(SDL_Point playerspd);   // Sets the velocity for the player
	SDL_Point getPlayerSpeed();				 // Gets the player velocity
	int playerScore;	
	bool PlayerState;
};
#endif