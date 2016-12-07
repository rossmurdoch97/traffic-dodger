/*
=================
cPlayer.cpp
- Header file for class definition - IMPLEMENTATION
=================
*/
#include "cPlayer.h"

/*
=================================================================
Defualt Constructor
=================================================================
*/
cPlayer::cPlayer() : cSprite()
{
	this->PlayerSpeed = { 0.0f, 0.0f };
}
/*
=================================================================
Update the sprite position
=================================================================
*/

void cPlayer::update(double deltaTime)
{

	FPoint direction = { 0.0f, 0.0f };
	direction.X = (sin(this->getSpriteRotAngle()*PI / 180));
	direction.Y = -(cos(this->getSpriteRotAngle()*PI / 180));

	direction.X *= this->getSpriteTranslation().x;
	direction.Y *= this->getSpriteTranslation().y;

	this->PlayerSpeed.x = this->PlayerSpeed.x + direction.X;
	this->PlayerSpeed.y = this->PlayerSpeed.y + direction.Y;

	SDL_Rect currentSpritePos = this->getSpritePos();
	currentSpritePos.x += this->PlayerSpeed.x * deltaTime;
	currentSpritePos.y += this->PlayerSpeed.y * deltaTime;

	this->PlayerSpeed.x *= 0.97;
	this->PlayerSpeed.y *= 0.97;
	
	if (currentSpritePos.y > 100 && currentSpritePos.y < 600)
	{ 
	this->setSpritePos({ currentSpritePos.x , currentSpritePos.y  });
	this->setBoundingRect(this->getSpritePos());
	}
}
/*
=================================================================
Sets the velocity for the player
=================================================================
*/
void cPlayer::setPlayerSpeed(SDL_Point playerspd)
{
	PlayerSpeed = playerspd;
}
/*
=================================================================
Gets the player velocity
=================================================================
*/
SDL_Point cPlayer::getPlayerSpeed()
{
	return PlayerSpeed;
}


/*
=================================================================
Update the sprite position
=================================================================
*/


//
//	glm::vec2 spriteVelocityAdd = glm::vec2(0.0f, 0.0f);
//	spriteVelocityAdd.x = (glm::sin(glm::radians(spriteRotation)));
//	spriteVelocityAdd.y = -(glm::cos(glm::radians(spriteRotation)));
//
//	spriteVelocityAdd *= spriteTranslation;
//
//	PlayerSpeed += spriteVelocityAdd;
//
//	spritePos2D += PlayerSpeed * deltaTime;
//
//	PlayerSpeed *= 0.95;
//
//	/*
//	==============================================================
//	| Check for collisions
//	==============================================================
//	*/
//	for (vector<cBullet*>::iterator bulletIterartor = theplayerBullets.begin(); bulletIterartor != theplayerBullets.end(); ++bulletIterartor)
//	{
//		(*bulletIterartor)->update(deltaTime);
//		for (vector<cOtherCar*>::iterator OtherCarIterator = theOtherCars.begin(); OtherCarIterator != theOtherCars.end(); ++OtherCarIterator)
//		{
//			if ((*OtherCarIterator)->collidedWith((*OtherCarIterator)->getBoundingRect(), (*bulletIterartor)->getBoundingRect()))
//			{
//				// if a collision set the bullet and OtherCar to false
//				(*OtherCarIterator)->setActive(false);
//				(*bulletIterartor)->setActive(false);
//			}
//		}
//	}
