#pragma once
#include <assert.h>
#include "Tileset_data.h"
#include "SDL2-2.0.8\include\SDL.h"
#include "SDL2-2.0.8\include\SDL_image.h"

namespace Tileset
{

	void init(Tileset *t, int array_size)
	{
		t->n_tilesets = 0;
		t->array_size = array_size;
		t->tex = (SDL_Texture**)malloc(sizeof(SDL_Texture*)*t->array_size);
		t->n_cols = (int*)malloc(sizeof(int)*t->array_size);
		t->n_rows = (int*)malloc(sizeof(int)*t->array_size);
		t->tile_w = (int*)malloc(sizeof(int)*t->array_size);
		t->tile_h = (int*)malloc(sizeof(int)*t->array_size);
	}

	void resize(Tileset *t)
	{
		t->array_size += t->array_size >> 1;

		t->tex = (SDL_Texture**)realloc(t->tex, sizeof(SDL_Texture*)*t->array_size);
		t->n_cols = (int*)realloc(t->n_cols, sizeof(int)*t->array_size);
		t->n_rows = (int*)realloc(t->n_rows, sizeof(int)*t->array_size);
		t->tile_w = (int*)realloc(t->tile_w, sizeof(int)*t->array_size);
		t->tile_h = (int*)realloc(t->tile_h, sizeof(int)*t->array_size);

	}

	int make(Tileset *t)
	{
		++t->n_tilesets;
		return t->n_tilesets - 1;
	}

	void modify(int index,Tileset *t, char *image_filename, int tile_w, int tile_h, SDL_Renderer *renderer)
	{
		SDL_Surface *sprite_surface = IMG_Load(image_filename);
		assert(sprite_surface);
		SDL_Texture *sprite_texture = SDL_CreateTextureFromSurface(renderer, sprite_surface);

		SDL_FreeSurface(sprite_surface);

		t->tile_w[index] = tile_w;
		t->tile_h[index] = tile_h;
		t->tex[index] = sprite_texture;
	}

	void draw(int index, int tileset_row, int tileset_col, Tileset *t, int dest_x, int dest_y, int dest_w, int dest_h, SDL_Renderer *renderer)
	{
		SDL_Rect src;
		src.x = tileset_col * t->tile_w[index];
		src.y = tileset_row * t->tile_h[index];
		src.w = t->tile_w[index];
		src.h = t->tile_h[index];

		SDL_Rect dest;
		dest.x = dest_x;
		dest.y = dest_y;
		dest.w = dest_w;
		dest.h = dest_h;

		SDL_RenderCopyEx(renderer, t->tex[index], &src, &dest, 0, NULL, SDL_FLIP_NONE);
	}
}