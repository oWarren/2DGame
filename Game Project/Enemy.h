#pragma once
#include <SDL.h>
class Enemy
{
public:
	SDL_Rect pos;
	SDL_Rect getRect() { return pos; }
	SDL_Texture* texture;
	Enemy();
	~Enemy();
private:

};

