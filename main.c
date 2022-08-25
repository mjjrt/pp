#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

typedef struct Player{
	int xpos;
	int ypos;
}Player;

typedef struct Ball{
	int xpos;
	int ypos;
	int vx;
	int vy;
}Ball;

typedef enum{
	UP,
	DOWN
}DIRECTION;


#define PLAYER1_INIT_X	50
#define PLAYER1_INIT_Y	SCREEN_HEIGHT/2
#define PLAYER2_INIT_X	500
#define PLAYER2_INIT_Y	SCREEN_HEIGHT/2
#define PLAYER_WIDTH 40
#define PLAYER_HEIGHT 100
#define BALL_SIZE 30

void UpdatePlayerPos(Player* p, int Yamount, DIRECTION d)
{
	if(p->ypos == 0) d = DOWN;
	if(p->ypos == SCREEN_HEIGHT-PLAYER_HEIGHT) d = UP;
	
	switch(d){
		case UP:
			p->ypos -= Yamount;
			break;
		case DOWN:
			p->ypos += Yamount;
			break;
		default:
			break;
	}
}

void UpdateBallPos(Ball* b, Player* p1, Player* p2)
{
	if(b->xpos == 0 || b->xpos == SCREEN_WIDTH-BALL_SIZE)
	{
		b->vx = -b->vx;
	}
	if(b->ypos == 0 || b->ypos == SCREEN_HEIGHT-BALL_SIZE)
	{
		b->vy = -b->vy;
	}

	if(b->xpos == p1->xpos+PLAYER_WIDTH && b->ypos >= p1->ypos && b->ypos <= p1->ypos+PLAYER_HEIGHT)
	{
		b->vx = -b->vx;
		b->vy = -b->vy;
	}
	if(b->xpos == p2->xpos && b->ypos >= p2->ypos && b->ypos <= p2->ypos+PLAYER_HEIGHT)
	{
		b->vx = -b->vx;
		b->vy = -b->vy;
	}
	b->xpos += b->vx;
	b->ypos += b->vy;
}

bool MainLoop(SDL_Window* win, SDL_Surface* surf)
{
	bool quit = false;

	SDL_Event e;

	SDL_Renderer* gRenderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	Player player1;
	player1.xpos = PLAYER1_INIT_X;
	player1.ypos = PLAYER1_INIT_Y;
	Player player2;
	player2.xpos = PLAYER2_INIT_X;
	player2.ypos = PLAYER2_INIT_Y;

	Ball ball = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 10, 5};

	while (!quit)
	{
		UpdateBallPos(&ball, &player1, &player2);

		SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(gRenderer);

		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawLine(gRenderer, SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT);

		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_Rect p1 = {player1.xpos, player1.ypos, PLAYER_WIDTH, PLAYER_HEIGHT};
		SDL_Rect p2 = {player2.xpos, player2.ypos, PLAYER_WIDTH, PLAYER_HEIGHT};
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawRect(gRenderer, &p1);
		SDL_RenderDrawRect(gRenderer, &p2);

		SDL_Rect b = {ball.xpos, ball.ypos, BALL_SIZE, BALL_SIZE};
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderDrawRect(gRenderer, &b);

		SDL_RenderPresent(gRenderer);

		while (SDL_PollEvent( &e ) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}else if(e.type == SDL_KEYDOWN){
				switch(e.key.keysym.sym)
				{
					case SDLK_w:
						UpdatePlayerPos(&player1, 10, UP);
						break;
					case SDLK_s:
						UpdatePlayerPos(&player1, 10, DOWN);
						break;
					case SDLK_UP:
						UpdatePlayerPos(&player2, 10, UP);
						break;
					case SDLK_DOWN:
						UpdatePlayerPos(&player2, 10, DOWN);
						break;
					default:
						break;
				}
			}
		}
	}
	return false;
}

int main(void)
{
	DIRECTION dir;

	SDL_Window* window = NULL;

	SDL_Surface* screen_surf = NULL;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Could not initialize: %s\n", SDL_GetError());
	}else{
		window = SDL_CreateWindow("Ping", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
		}else{
			screen_surf = SDL_GetWindowSurface(window);

			MainLoop(window, screen_surf);

			SDL_DestroyWindow(window);
		}
		SDL_Quit();
	}
	return 0;
}
