#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Player.h"
#include <vector>
#include "Enemy.h"
#include <time.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <sstream>



void ShieldStrength(SDL_Renderer* renderer, int collisionCount);
bool getCollision(SDL_Rect rect1, SDL_Rect rect2);
void Score(SDL_Renderer* renderer, int score, bool printScore);

#undef main

int main(int argc, char *argv[])
{bool playAgain = true;	

	do
	{
		//Time variables
		float frameTime = 0;
		int prevTime = 0;
		int currentTime = 0;

		//Change in time
		float deltaTime = 0;
		float moveSpeed = 200.0f;
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		SDL_Texture* playerImage = nullptr;
		SDL_Texture* startSelected = nullptr;
		SDL_Texture* exitSelected = nullptr;
		SDL_Texture* backgroundImage = nullptr;
		SDL_Texture* menuImage = nullptr;
		SDL_Texture* playAgainMenu = nullptr;
		SDL_Texture* playAgainSelected = nullptr;
		SDL_Texture* exitSelected2 = nullptr;
		SDL_Texture* instructions = nullptr;
		SDL_Event ev;
		SDL_Event menu;
		Player player;
		Mix_Music* bGM = Mix_LoadMUS("bgm.mp3");
		Mix_Chunk* soundEffect = Mix_LoadWAV("explo.wav");
		bool menuBool = true;
		bool printScore = false;
		int collisionCount = 4;
		int score = 0;
		int playCount = 0;

		std::vector<Enemy*> enemies;
		srand((unsigned int)time(NULL));

		const Uint8 *keyState = nullptr;
		int frameWidth, frameHeight;
		int textureWidth, textureHeight;
		bool isRunning = true;

		//Initialisation of SDL
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		{
			std::cout << "Video initialisation error : " << SDL_GetError() << std::endl;
		}
		else
		{
			//Loading of textures and creation of window and renderer
			window = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			playerImage = IMG_LoadTexture(renderer, "Pictures/Spaceship44.png");
			backgroundImage = IMG_LoadTexture(renderer, "Pictures/starbackground.jpg");
			menuImage = IMG_LoadTexture(renderer, "Pictures/MenuUnselected1.png");
			startSelected = IMG_LoadTexture(renderer, "Pictures/MenuStartSelected.png");
			exitSelected = IMG_LoadTexture(renderer, "Pictures/MenuExitSelected.png");
			playAgainMenu =IMG_LoadTexture(renderer, "Pictures/PlayAgainMenu.png");
			playAgainSelected = IMG_LoadTexture(renderer, "Pictures/PlayAgainSelected.png");
			exitSelected2 = IMG_LoadTexture(renderer, "Pictures/ExitSelected2.png");
			instructions = IMG_LoadTexture(renderer, "Pictures/Instructions.png");

			if (window == NULL || renderer == NULL)
			{
				std::cout << "window/renderer creation error: " << SDL_GetError() << std::endl;
			}
			else
			{
				//error checking
				if (playerImage == NULL || backgroundImage == NULL)
				{
					std::cout << "Image loading error: " << SDL_GetError() << std::endl;
				}
				if (Mix_OpenAudio(22100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					std::cout << "Error: " << Mix_GetError() << std::endl;
				}
				if (TTF_Init() < 0)
				{
					std::cout << "Error: " << TTF_GetError() << std::endl;
				}
				else
				{
					//Players the background music
					Mix_PlayMusic(bGM, -1);
					//Initialisation of bounding boxes
					SDL_Rect mouseBound;
					SDL_Rect startButton = { 235,280,172,50 };
					SDL_Rect exitButton = { 234, 360, 172,55 };
					SDL_Rect instructionsButton = { 235, 0, 172,55 };

					//Start of menu loop
						while (menuBool == true)
						{

							while (SDL_PollEvent(&menu) != 0)
							{
								//Prints out menu screen
								SDL_RenderClear(renderer);
								SDL_RenderCopy(renderer, menuImage, NULL, NULL);
								
								//Gets the mouse x and y positions
								if (menu.type == SDL_MOUSEMOTION);
								{
									mouseBound.x = menu.motion.x;
									mouseBound.y = menu.motion.y;
									mouseBound.w = 4;
									mouseBound.h = 4;
									
									//If the mouse collides with any of the bounding boxes then change the image
									if (getCollision(startButton, mouseBound))
									{
										SDL_RenderCopy(renderer, startSelected, NULL, NULL);
									}
									if (getCollision(exitButton, mouseBound))
									{
										SDL_RenderCopy(renderer, exitSelected, NULL, NULL);
									}
									if (getCollision(instructionsButton, mouseBound))
									{
										SDL_RenderClear(renderer);
										SDL_RenderCopy(renderer, instructions, NULL, NULL);
									}
								}
								//Checks if the mouse button has been clicked
								if (menu.type == SDL_MOUSEBUTTONDOWN)
								{
									//If the mouse button has been clicked and is colliding with a bounding box then exit the menu loop and continue with game
									if (getCollision(startButton, mouseBound))
									{
												menuBool = false;
									}
									//If the mouse button has been clicked and is colliding with a bounding box then exit program nad bypass all loops
									if (getCollision(exitButton, mouseBound))
									{
										menuBool = false;
										isRunning = false;
										playAgain = false;
									}
								}
							}

							SDL_RenderPresent(renderer);
						}
					

						
					//Sets texture width and height
					SDL_QueryTexture(playerImage, NULL, NULL, &textureWidth, &textureHeight);

					
					frameWidth = textureWidth / 4;
					frameHeight = textureHeight / 4;

					//Sets the players texture box
					player.rect.x = 1;
					player.rect.y = 0;
					player.rect.w = frameWidth;
					player.rect.h = frameHeight;
					SDL_Rect position = { 0,0,4000,480 };

					//Creation of the enemy texture and applies the texture to all enemies(asteroids)
					SDL_Texture* enemyTexture = IMG_LoadTexture(renderer, "Pictures/Asteroid1.png");
					//This loop creates the asteroids and adds them to a vector storing position and area
					for (size_t i = 0; i < 20; i++)
					{
						Enemy* temp = new Enemy();
						temp->texture = enemyTexture;
						temp->pos.y = i * 100 + ((rand() % 100));
						temp->pos.x = 2 * 450 + ((rand() % 450) - 75);
						temp->pos.w = 40;
						temp->pos.h = 40;

						enemies.push_back(temp);
					}
					
					//Main game loop start
					while (isRunning == true)
					{
						//Screen scrolling
						position.x -= 2;
						if (position.x < -3360)
						{
							position.x = 0;
						}

						prevTime = currentTime;
						currentTime = SDL_GetTicks();
						deltaTime = (currentTime - prevTime) / 1000.0f;
						//If the x is clicked at the top of the screen make isRunning false and close window
						while (SDL_PollEvent(&ev))
						{

							if (ev.type == SDL_QUIT)
							{
								isRunning = false;
							}

							//Checks if a key is down
							else if (ev.type == SDL_KEYDOWN)
							{
								switch (ev.key.keysym.sym)
								{
									//If the key is p then mute the music if music is not playing then unmute it
								case SDLK_p:
								{
									if (!Mix_PlayingMusic())
									{
										Mix_PlayMusic(bGM, -1);
									}
									else if (Mix_PausedMusic())
									{
										Mix_ResumeMusic();
									}
									else Mix_PauseMusic();
									break;
								}
								//Attempt at sound effects
								case SDLK_m:
								{
									Mix_PlayChannel(-1, soundEffect, 0);
									break;
								}
								}
							}
						}
						score++;
						//Intialises the keyboard input
						keyState = SDL_GetKeyboardState(NULL);

						//The function for how the player moves
						player.Movement(keyState, deltaTime);

						//Gravity
						player.velY += 0.1;
						player.pos.y += player.velY;

						//Boundaries for the window
						player.pos = player.BoundaryForWindow();

						frameTime += deltaTime;
						//If player is pushing down any of the selected keys run the animation
						if (keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_UP] || keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT])
						{
							if (frameTime >= 0.15f)
							{
								frameTime = 0;
								player.rect.x += frameWidth;
								if (player.rect.x >= textureWidth)
								{
									player.rect.x = 1;
								}
							}
						}
						//Else run this particular sprite animation
						else if (frameTime >= 0.15f)
						{
							frameTime = 0;
							player.rect.y = 570;
							player.rect.x += frameWidth;
							if (player.rect.x >= textureWidth)
							{
								player.rect.x = 1;
							}
						}
						//Renders the game
						SDL_RenderClear(renderer);
						SDL_RenderCopy(renderer, backgroundImage, NULL, &position);
						SDL_RenderCopy(renderer, playerImage, &player.rect, &player.pos);
						Score(renderer, score,printScore);
						ShieldStrength(renderer, collisionCount);
						//For each enemy
						for each (Enemy* var in enemies)
						{
							//If enemy is off the screen the respawn then off the otherside of the screen and randomise the position of the enemy
							var->pos.x -= 5;
							SDL_RenderCopy(renderer, var->texture, NULL, &var->pos);
							if (var->pos.x < -100)
							{
								var->pos.y = rand() % 10 * 100 + ((rand() % 100));
								var->pos.x = rand() % 10 * 150 + ((rand() % 150) - 75);
								var->pos.x += 800;
							}
							//If there is a collision between an enemy and a player then move the enemy off screen and randomise 
							if (getCollision(var->getRect(), player.getRect()))
							{
								printf("collision");
								//Take one away from collision count
								collisionCount -= 1;
								var->pos.y = rand() % 10 * 100 + ((rand() % 100));
								var->pos.x = rand() % 10 * 150 + ((rand() % 150) - 75);
								var->pos.x += 800;
							}


						}
						if (collisionCount <= 0)
						{
							isRunning = false;
							playCount++;
							SDL_RenderClear(renderer);

						}

						SDL_RenderPresent(renderer);
						SDL_Delay(1000 / 60);
					}

					//You lose screen/ player again
				}

			}
		}
		SDL_Rect mouseBound;
		SDL_Rect startButton = { 235,280,172,50 };
		SDL_Rect exitButton = { 234, 360, 172,55 };

		if (playCount > 0)
		{
			menuBool = true;
			Score(renderer, score, printScore);
			SDL_RenderCopy(renderer, playAgainMenu, NULL, NULL);
			
		}
		while (menuBool == true)
		{

			while (SDL_PollEvent(&menu) != 0)
			{
				printScore = true;
				SDL_RenderCopy(renderer, playAgainMenu, NULL, NULL);
				if (menu.type == SDL_MOUSEMOTION);
				{
					Score(renderer, score,printScore);
					mouseBound.x = menu.motion.x;
					mouseBound.y = menu.motion.y;
					mouseBound.w = 4;
					mouseBound.h = 4;

					if (getCollision(startButton, mouseBound))
					{
						SDL_RenderCopy(renderer, playAgainSelected, NULL, NULL);
						Score(renderer, score, printScore);
					}
					if (getCollision(exitButton, mouseBound))
					{
						SDL_RenderCopy(renderer, exitSelected2, NULL, NULL);
						Score(renderer, score, printScore);
					}
				}

				if (menu.type == SDL_MOUSEBUTTONDOWN)
				{
					if (getCollision(startButton, mouseBound))
					{
						menuBool = false;
					}
					if (getCollision(exitButton, mouseBound))
					{
						menuBool = false;
						isRunning = false;
						playAgain = false;
					}
				}
			}

			SDL_RenderPresent(renderer);
		}
	
	//Destroying / deleting pointers
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(playerImage);
	SDL_DestroyTexture(backgroundImage);
	SDL_DestroyTexture(menuImage);
	SDL_DestroyTexture(startSelected);
	SDL_DestroyTexture(exitSelected);
	SDL_DestroyTexture(exitSelected2);
	SDL_DestroyTexture(playAgainMenu);
	SDL_DestroyTexture(playAgainSelected);
	
	Mix_FreeMusic(bGM);
	Mix_FreeChunk(soundEffect);
	for (Enemy* e : enemies)
	{
		delete e;
	}
	bGM = nullptr;
	soundEffect = nullptr;
	window = nullptr;
	playerImage = nullptr;
	renderer = nullptr;
	keyState = nullptr;
	menuImage = nullptr;
	backgroundImage = nullptr;
	startSelected = nullptr;
	exitSelected = nullptr;
	playAgainMenu = nullptr;
	playAgainSelected = nullptr;
	exitSelected2 = nullptr;
 }while (playAgain == true);
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
	return 0;
}
void Score(SDL_Renderer* renderer, int score, bool printScore)
{
	
	SDL_Rect textRect = {100,0,400,20};
	SDL_Rect textRect4 = { 0,0,400,20 };
if (printScore == true)
	{
		textRect.x = 350;
		textRect.h = 30;
		textRect4.x = 240;
		textRect4.h = 30;
		
	}
	std::stringstream variable;
	variable << score;
	TTF_Font* font = TTF_OpenFont("AgentOrange.ttf",20);
	SDL_Color colour = { 153, 0,204,255 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, variable.str().c_str(), colour);
	SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_Surface* textSurface4 = TTF_RenderText_Solid(font, "SCORE ", colour);
	SDL_Texture* text4 = SDL_CreateTextureFromSurface(renderer, textSurface4);

	SDL_QueryTexture(text4, NULL, NULL, &textRect4.w, &textRect4.y);
	SDL_RenderCopy(renderer, text4, NULL, &textRect4);

	SDL_QueryTexture(text, NULL, NULL, &textRect.w, &textRect.y);
	SDL_RenderCopy(renderer, text, NULL ,&textRect);

	SDL_DestroyTexture(text);
	SDL_FreeSurface(textSurface);
	textSurface = nullptr;
	text = nullptr;
	font = nullptr;
}
void ShieldStrength(SDL_Renderer* renderer, int collisionCount)
{
	int shieldHealth = 100;
	SDL_Rect textRect = { 580,0,360,20 };
	SDL_Rect textRect2 = { 470,0,360,20 };
	if (collisionCount == 4)
	{
		shieldHealth = 100;
	}
	if (collisionCount == 3)
	{
		shieldHealth = 75;
	}
	if (collisionCount == 2)
	{
		shieldHealth = 50;
	}
	if (collisionCount == 1)
	{
		shieldHealth = 25;
	}
	std::stringstream vari;
	vari << shieldHealth;
	TTF_Font* font2 = TTF_OpenFont("AgentOrange.ttf", 20);
	SDL_Color colour2 = { 153, 0,204,255 };
	SDL_Surface* textSurface2 = TTF_RenderText_Solid(font2, vari.str().c_str(), colour2);
	SDL_Texture* text2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
	SDL_Surface* textSurface3 = TTF_RenderText_Solid(font2, "SHIELD", colour2);
	SDL_Texture* text3 = SDL_CreateTextureFromSurface(renderer, textSurface3);

	SDL_QueryTexture(text3, NULL, NULL, &textRect2.w, &textRect2.y);
	SDL_RenderCopy(renderer, text3, NULL, &textRect2);
	SDL_QueryTexture(text2, NULL, NULL, &textRect.w, &textRect.y);
	SDL_RenderCopy(renderer, text2, NULL, &textRect);

	SDL_DestroyTexture(text2);
	SDL_FreeSurface(textSurface2);
	textSurface2 = nullptr;
	text2 = nullptr;
	font2 = nullptr;
	textSurface3 = nullptr;
	text3 = nullptr;

}
bool getCollision(SDL_Rect rect1, SDL_Rect rect2)
{
	return SDL_HasIntersection(&rect1, &rect2);
}



