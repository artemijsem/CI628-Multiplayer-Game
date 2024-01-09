#pragma once
#include "MyGame.h"

class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const char* filename, SDL_Renderer* renderer);
	static void Draw(SDL_Renderer* renderer, SDL_Texture* tex, SDL_Rect dest);
};