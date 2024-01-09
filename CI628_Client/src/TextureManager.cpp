#include "TextureManager.h"
#include "MyGame.h"


//=================================================================================

SDL_Texture* TextureManager::LoadTexture(const char* pFilename, SDL_Renderer* renderer)
{
	SDL_Surface* tempSurface = IMG_Load(pFilename);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	if (texture == NULL) printf(" Image Load Error  - SDL Error: %s\n", SDL_GetError());

	SDL_FreeSurface(tempSurface);
	return texture;
}//---

//=================================================================================


void TextureManager::Draw(SDL_Renderer* renderer, SDL_Texture* pTexture, SDL_Rect pDestinationRect)
{
	SDL_RenderCopy(renderer, pTexture, NULL, &pDestinationRect);

}//---
//=================================================================================