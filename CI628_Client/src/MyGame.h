#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include "SDL_net.h"
#include <SDL.h>
#include<SDL_Image.h>
#include<SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "SDL.h"
#include "Level.h"

static struct GameData {
    int player1Lives = 0;
    int player1Y = 0;
    int player1X = 0;
    int player2Lives = 0;
    int player2Y = 0;
    int player2X = 0;
    int player3Lives = 0;
    int player3Y = 0;
    int player3X = 0;
    int player4Lives = 0;
    int player4Y = 0;
    int player4X = 0;
    int playerSize;
    int playerNum;
} game_data;

class MyGame {

    private:
        SDL_Rect player1 = { 0, 0, 40, 40 };
        SDL_Rect player2 = { 600, 300, 40, 40 };
        SDL_Rect player3 = { 0,0,0,0 };
        SDL_Rect player4 = { 0,0,0,0 };
        SDL_Rect bomb_arr[50];
        SDL_Rect bomb_explosion_arr[500];
        SDL_Window* gameWindow = nullptr;

    public:
        Level* level = nullptr;
        SDL_Texture* playerOneText;
        SDL_Texture* playerTwoText;
        SDL_Texture* playerThreeText;
        SDL_Texture* playerFourText;
        SDL_Texture* bombText;
        SDL_Texture* explosionText;
        std::vector<std::string> messages;
        TTF_Font* font = nullptr; 
        TTF_Font* smallFont = nullptr; 
        SDL_Colour textColor = { 255, 255, 255 }; 
        SDL_Surface* textSurface = nullptr; 
        SDL_Texture* textTexture = nullptr; 
        SDL_Rect textRect; 
        int textW, textH; 

        Mix_Chunk* music = NULL;
        Mix_Chunk* bombExplosion = NULL;
        Mix_Chunk* playerHit = NULL;
        Mix_Chunk* powerUpPickUp = NULL;
        Mix_Chunk* gameWinSound = NULL;
        Mix_Chunk* gameLostSound = NULL;

        bool gameIsOver = false;
        bool gameIsWon = false;
        bool disconnected = false;
       
        std::map<int, int> playersAlive;
        int alivePlayersNum = 4;
        

        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void init(SDL_Renderer* renderer);
        void input(SDL_Event& event);
        SDL_Rect getPlayerOneRect() { return player1; }
        SDL_Rect getPlayerTwoRect() { return player2; }
        SDL_Rect getPlayerThreeRect() { return player3; }
        SDL_Rect getPlayerFourRect() { return player4; }
        void update();
        void render(SDL_Renderer* renderer);
        void drawUI(SDL_Renderer* renderer);
        void gameOver(SDL_Renderer* renderer);
        void quitGame(); 
        void setGameWindow(SDL_Window* window) { gameWindow = window; }
        SDL_Window* getGameWindow() { return gameWindow; }
};

#endif