#include "Level.h"
#include "MyGame.h"
#include "TextureManager.h"
#include<string>
#include <cmath>
#include <vector>




Level::Level(SDL_Renderer* renderer)
{
	LevelTime = SDL_GetTicks();
	ClearTime = SDL_GetTicks();
    createLevelFromString(levelText);
}

void Level::createLevelFromString(std::string level)
{
    int index = 0;
    for (int row = 0; row < NUM_ROWS; ++row) {
        for (int col = 0; col < NUM_COLS; ++col) {
            map[row][col] = level[index] - '0'; // Convert character to integer
            index++;
        }
    }

}

void Level::drawMap(SDL_Renderer* renderer)
{
	int mapTileType = 0;

	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			mapTileType = map[row][col];
			dest.x = col * MAP_TILE_SIZE;
			dest.y = row * MAP_TILE_SIZE;
			dest.w = MAP_TILE_SIZE;
			dest.h = MAP_TILE_SIZE;

			switch (mapTileType)
			{
			case 0:
				break;
			case 1:
				TextureManager::Draw(renderer, wall, dest);
				break;
			case 2:
				TextureManager::Draw(renderer, brick, dest);
				break;
			case 3:
				TextureManager::Draw(renderer, bomb, dest);
				break;
			case 4:
				TextureManager::Draw(renderer, bombExplosionWave, dest);
				break;
			case 5:
				TextureManager::Draw(renderer, powerUp, dest);
				break;
			default:
				break;
			}
		}
	}
}

void Level::updateMap(int sentX, int sentY, int newTileType)
{
	for (int row = 0; row < NUM_ROWS; row++)
	{
		for (int col = 0; col < NUM_COLS; col++)
		{
			// If updating existing tiles
			if (sentX == col * MAP_TILE_SIZE && sentY == row * MAP_TILE_SIZE) map[row][col] = newTileType;

			// If creating a new tile
			else
			{
				map[(int)(sentY / MAP_TILE_SIZE)][(int)(sentX / MAP_TILE_SIZE)] = newTileType;
			}
		}
	}
}

void Level::bombExplosion(int bombX, int bombY, int bombRadius)
{
	// COnversion from absoulte X and Y pos to map array pos
	int bombPosX = bombX / MAP_TILE_SIZE;
	int bombPosY = bombY / MAP_TILE_SIZE;

	// Clear out bomb texture
	map[bombPosY][bombPosX] = 0;

	// Loop through tiles that are in the bomb radius
	for (int explosionCircle = 0; explosionCircle <= (bombRadius / MAP_TILE_SIZE); explosionCircle++)
	{
		for (int row = 0; row < NUM_ROWS; row++)
		{
			for (int col = 0; col < NUM_COLS; col++)
			{
				// Look for tiles that are on the same X and Y axis as the bomb
				if (((abs(bombPosX - col) == 0 && abs(bombPosY - row) == explosionCircle) || (abs(bombPosY - row) == 0 
					&& abs(bombPosX - col) == explosionCircle)) && map[row][col] != 1 )
				{
					// Save tiles before the explosion
					if (map[row][col] != 5)
					{
						previousTileType.push_back(map[row][col]);
						previousTilePosX.push_back(row);
						previousTilePosY.push_back(col);

						map[row][col] = 4;
					}
				}

			}
		}
	}
}

void Level::clearBombExplosion()
{
	int time_now = SDL_GetTicks();
	int time_delta = time_now - LevelTime;

	if (time_delta > 1000)
	{
		for (int row = 0; row < NUM_ROWS; row++)
		{
			for (int col = 0; col < NUM_COLS; col++)
			{
				for (unsigned int i = 0; i < previousTileType.size(); i++)
				{
					// Check if the tile was modified and if it is powerup
					if ((row == previousTilePosX[i] && col == previousTilePosY[i]) && map[row][col] == 4)
					{
						map[row][col] = previousTileType[i];

						// If bomb texture was remembered clear it
						if (map[row][col] == 3) { map[row][col] = 0; }

					}
				}

			}
		}
		LevelTime = time_now;
	}
}

