#pragma once
#include <string>
#include <vector>
#include "SDL.h"


const int NUM_ROWS = 18;
const int NUM_COLS = 26;
const int MAP_TILE_SIZE = 30;


class Level
{
public:
	Level(SDL_Renderer* renderer);

	std::string levelText = "000000000000000000000000000000000000000000000000000001111111111111111111111111010022222222222222222220010102220222220022222202220101222222222222222222222221012222222022222202222222210122222222222222222222222101222022222222222222202221012222222222002222222222210122222222220022222222222101222022222222222222202221012222222022222202222222210122222222222222222222222101222022222200222222202221010222222222222222222222010100222222222222222222200101111111111111111111111111";

	int map[NUM_ROWS][NUM_COLS];
	int LevelTime = 0;
	int ClearTime = 0;
	
	std::vector<int> previousTileType;
	std::vector<int> previousTilePosX;
	std::vector<int> previousTilePosY;

	void drawMap(SDL_Renderer* renderer);
	void createLevelFromString(std::string level);
	void updateMap(int sentX, int sentY, int newTileType);
	void bombExplosion(int bombX, int bombY, int bombRadius);
	void clearBombExplosion();
	
	SDL_Rect src;
	SDL_Rect dest;
	SDL_Texture* empty;
	SDL_Texture* wall;
	SDL_Texture* brick;
	SDL_Texture* bomb;
	SDL_Texture* bombExplosionWave;
	SDL_Texture* powerUp;
	
};