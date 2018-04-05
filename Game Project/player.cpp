#include "Player.h"

void Player::Movement(const Uint8* _keyState, float _deltaTime)
{

	if (_keyState[SDL_SCANCODE_RIGHT])
	{
		pos.x += moveSpeed * _deltaTime;
		rect.y = 570;
	}
	else if (_keyState[SDL_SCANCODE_LEFT])
	{
		pos.x -= moveSpeed * _deltaTime;
		rect.y = 1190;
	}
	if (_keyState[SDL_SCANCODE_UP])
	{
		rect.y = -50;
		velY = 0;
		//moveSpeed = 20;
		pos.y -= moveSpeed * _deltaTime;
		moveSpeed = 200.0f;
	}
	if (_keyState[SDL_SCANCODE_DOWN])
	{
		pos.y += moveSpeed * _deltaTime;	
		
	}
}


SDL_Rect Player::BoundaryForWindow()
{
	if (pos.x < 0)
	{
		pos.x = 0;
	}
	if (pos.x > 590)
	{
		pos.x = 590;
	}
	if (pos.y < 0)
	{
		pos.y = 0;
	}
	if (pos.y > 430)
	{
		pos.y = 430;
		velY = 0;
	}
	return pos;
}

