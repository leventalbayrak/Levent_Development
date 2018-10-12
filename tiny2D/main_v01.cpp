#include <iostream>
#include <assert.h>
#include <time.h>
using namespace std;

//include SDL header
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"
#include "SDL2-2.0.8\include\SDL_mixer.h"

#include "Tilemap_core.h"
#include "Grid_core.h"
#include "Grid_Camera_core.h"

//load libraries
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2main.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_image.lib")
#pragma comment(lib,"SDL2-2.0.8\\lib\\x86\\SDL2_mixer.lib")

#pragma comment(linker,"/subsystem:console")

namespace Game
{
	SDL_Renderer *renderer = NULL;
	int screen_width = 800;
	int screen_height = 600;
}

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_VIDEO);
	srand(time(0));

	SDL_Window *window = SDL_CreateWindow(
		"Fortnite",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		Game::screen_width, Game::screen_height, SDL_WINDOW_SHOWN);

	Game::renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_ACCELERATED);

	Tileset::Tileset tileset;
	Tileset::load(&tileset, "tileset.png", 32, 32, Game::renderer);

	unsigned char prev_key_state[256];
	unsigned char *keys = (unsigned char*)SDL_GetKeyboardState(NULL);

	Grid::Grid level;
	Grid::init(&level, 512, 512);

	for (int i = 0; i < level.n_cols*level.n_rows; i++)
	{
		level.data[i] = rand() % 3;
	}

	Grid_Camera::Grid_Camera camera = { 50,50,80,60 };
	float camera_move_speed = 0.00001;
	float camera_zoom_speed = 0.00001;

	bool done = false;
	while (!done)
	{
		memcpy(prev_key_state, keys, 256);

		//consume all window events first
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
		}

		int cmd_UP = 0;
		int cmd_LEFT = 0;
		int cmd_RIGHT = 0;
		int cmd_DOWN = 0;
		int cmd_ZOOMIN = 0;
		int cmd_ZOOMOUT = 0;

		if (keys[SDL_SCANCODE_W]) cmd_UP = 1;
		if (keys[SDL_SCANCODE_S]) cmd_DOWN = 1;
		if (keys[SDL_SCANCODE_A]) cmd_LEFT = 1;
		if (keys[SDL_SCANCODE_D]) cmd_RIGHT = 1;
		if (keys[SDL_SCANCODE_R]) cmd_ZOOMOUT = 1;
		if (keys[SDL_SCANCODE_T]) cmd_ZOOMIN = 1;

		/*
		GAME CODE
		*/
		if (cmd_ZOOMIN)
		{
			camera.x += camera_zoom_speed*Game::screen_width;
			camera.w += -camera_zoom_speed *Game::screen_width;
			camera.y += camera_zoom_speed *Game::screen_height;
			camera.h += -camera_zoom_speed *Game::screen_height;
		}
		if (cmd_ZOOMOUT)
		{
			camera.x += -camera_zoom_speed *Game::screen_width;
			camera.w += camera_zoom_speed *Game::screen_width;
			camera.y += -camera_zoom_speed *Game::screen_height;
			camera.h += camera_zoom_speed *Game::screen_height;
		}

		if (cmd_LEFT)
		{
			camera.x += -camera_move_speed*Game::screen_width;
		}
		if (cmd_RIGHT)
		{
			camera.x += camera_move_speed *Game::screen_width;
		}
		if (cmd_UP)
		{
			camera.y += -camera_move_speed *Game::screen_height;
		}
		if (cmd_DOWN)
		{
			camera.y += camera_move_speed *Game::screen_height;
		}
	
		//printf("camera %f %f\n", camera.x, camera.y);

		//RENDER

		//set color to white
		SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
		//clear screen with white
		SDL_RenderClear(Game::renderer);

		Grid::Active_Range active_range;
		Grid_Camera::translate(&active_range, &camera);

		for (int i = 0; i < active_range.h; i++)
		{
			for (int j = 0; j < active_range.w; j++)
			{
				float w = (float)Game::screen_width / active_range.w;
				float h = (float)Game::screen_height / active_range.h;
				float x = j * w;
				float y = i * h;
				
				int grid_data = level.data[(i+active_range.y)*level.n_cols + j + active_range.x];

				Tileset::draw(x, y, w, h, 1.0, grid_data, 0, &tileset, Game::renderer);
			}
		}

		float w = (float)Game::screen_width / active_range.w;
		float h = (float)Game::screen_height / active_range.h;
		Tileset::draw(Game::screen_width/2, Game::screen_height/2, w, h, 1.0, 12, 0, &tileset, Game::renderer);
	
		//flip buffers
		SDL_RenderPresent(Game::renderer);

	}



	return 0;
}