#include <SDL.h>

class Player
{
private:
	
public:
	
	SDL_Rect pos, rect;
	float velX,velY;
	float moveSpeed;
	
	SDL_Rect getRect() { return pos; }
	Player()
	{
		
		moveSpeed = 200.0f;
		pos.x = pos.y = 0;
		pos.w = pos.h = 60;
		velX = velY = 0;
		
	}
	void Movement(const Uint8* _keyState, float _deltaTime);
	SDL_Rect BoundaryForWindow();
	
};

