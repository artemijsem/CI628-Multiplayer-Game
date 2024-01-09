#include "MyGame.h"
#include "Level.h"
#include "TextureManager.h"

void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {


    if (cmd == "SETUP")
    {
        std::cout << "Setup information recevied" << std::endl; 
        game_data.playerNum = stoi(args.at(0));
        game_data.playerSize = stoi(args.at(1)); 
        std::cout << "Player Size: " << game_data.playerSize << std::endl;
       
        player1.h = game_data.playerSize; 
        player1.w = game_data.playerSize; 
        player2.h = game_data.playerSize; 
        player2.w = game_data.playerSize;
        player3.h = game_data.playerSize;
        player3.w = game_data.playerSize;
        player4.h = game_data.playerSize;
        player4.w = game_data.playerSize;               
        
    }

    if (cmd == "GAME_DATA") {
        // we should have exactly 12 arguments
        if (args.size() == 12) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player1X = stoi(args.at(1));
            game_data.player2Y = stoi(args.at(2));
            game_data.player2X = stoi(args.at(3));
            game_data.player3Y = stoi(args.at(4));
            game_data.player3X = stoi(args.at(5));
            game_data.player4Y = stoi(args.at(6));
            game_data.player4X = stoi(args.at(7));
            game_data.player1Lives = stoi(args.at(8));
            game_data.player2Lives = stoi(args.at(9));
            game_data.player3Lives = stoi(args.at(10));
            game_data.player4Lives = stoi(args.at(11));
            
        }
    }

    else if (cmd == "SPAWN_BOMB")
    {
        std::cout << "SPAWN BOMB: " << cmd << " " << args.at(0) << " " << args.at(1) << std::endl;
        level->updateMap(stoi(args.at(0)), stoi(args.at(1)), 3);
        
    }

    else if (cmd == "SPAWN_POWERUP")
    {
        level->updateMap(stoi(args.at(0)), stoi(args.at(1)), 5);
    }

    else if (cmd == "POWERUP_PICKED")
    {
        level->updateMap(stoi(args.at(0)), stoi(args.at(1)), 0);
        Mix_PlayChannel(-1, powerUpPickUp, 0);
    }

    else if (cmd == "BRICK_DESTROYED")
    {
        // make the map tile with brick to become clear (0)
        level->updateMap(stoi(args.at(0)), stoi(args.at(1)), 0);
    }

    else if (cmd == "BOMB_EXPLODED")
    {
        level->bombExplosion(stoi(args.at(0)), stoi(args.at(1)), stoi(args.at(2)));
        std::cout << "BOMB_EXPLODED : " << cmd << " " << args.at(0) << " " << args.at(1) << " " << args.at(2) << std::endl;
        Mix_PlayChannel(-1, bombExplosion, 0);
        
        
    }

    else if (cmd == "PLAYER_HIT")
    {
        Mix_PlayChannel(-1, playerHit, 0);
    }

    else if (cmd == "PLAYER_LOST")
    {
        switch (stoi(args.at(0)))
        {
        case 1:
            playersAlive[1] = 0;
            alivePlayersNum--;
            break;
        case 2:
            playersAlive[2] = 0;
            alivePlayersNum--;
            break;
        case 3:
            playersAlive[3] = 0;
            alivePlayersNum--;
            break;
        case 4:
            playersAlive[4] = 0;
            alivePlayersNum--;
            break;
        default:
            break;
        }

        if (stoi(args.at(0)) == game_data.playerNum)
        {
            gameIsOver = true;
            Mix_PlayChannel(-1, gameLostSound, 0);
        }


        for (auto it = playersAlive.begin(); it != playersAlive.end(); ++it)
        {
            if ((it->first == game_data.playerNum && it->second == 1) && alivePlayersNum == 1)
            {
                gameIsWon = true;
                Mix_PlayChannel(-1, gameWinSound, 0);
            }
        }

    }
    
    else if (cmd == "SERVER_IS_FULL")
    {
        disconnected = true;
    }
    
    else {
        std::cout << "Received: " << cmd << std::endl;
    }
}

void MyGame::send(std::string message) {
    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_w:
            send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
            break;
        case SDLK_s:
            send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
            break;
        case SDLK_a:
            send(event.type == SDL_KEYDOWN ? "A_DOWN" : "A_UP");
            break;
        case SDLK_d:
            send(event.type == SDL_KEYDOWN ? "D_DOWN" : "D_UP");
            break;
        case SDLK_SPACE:
            send(event.type == SDL_KEYDOWN ? "F_DOWN" : "F_UP");
            break;
        case SDLK_z:

            gameIsWon = true;
            break;
        case SDLK_x:
            gameIsOver = true;
            break;
    }



}

void MyGame::update() {
    player1.y = game_data.player1Y;
    player1.x = game_data.player1X;
    player2.y = game_data.player2Y;
    player2.x = game_data.player2X;
    player3.y = game_data.player3Y;
    player3.x = game_data.player3X;
    player4.y = game_data.player4Y;
    player4.x = game_data.player4X;

}


void MyGame::init(SDL_Renderer* renderer)
{
    level = new Level(renderer);
    level->LevelTime = SDL_GetTicks();
    level->wall = TextureManager::LoadTexture("../assets/images/wall.png", renderer);
    level->brick = TextureManager::LoadTexture("../assets/images/brick.png", renderer);
    level->bomb = TextureManager::LoadTexture("../assets/images/bomb.png", renderer);
    level->bombExplosionWave = TextureManager::LoadTexture("../assets/images/bombExplosion.png", renderer);
    level->powerUp = TextureManager::LoadTexture("../assets/images/powerup.png", renderer);
    playerOneText = TextureManager::LoadTexture("../assets/images/Player_One.png", renderer);
    playerTwoText = TextureManager::LoadTexture("../assets/images/Player_Two.png", renderer);
    playerThreeText = TextureManager::LoadTexture("../assets/images/Player_Three.png", renderer);
    playerFourText = TextureManager::LoadTexture("../assets/images/Player_Four.png", renderer);

    font = TTF_OpenFont("../assets/fonts/dogicapixel.ttf", 72);
    smallFont = TTF_OpenFont("../assets/fonts/dogicapixel.ttf", 24);

    playersAlive[1] = 1;
    playersAlive[2] = 1;
    playersAlive[3] = 1;
    playersAlive[4] = 1;

    // Audio
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

    music = Mix_LoadWAV("../assets/audio/BG_Music.wav");
    bombExplosion = Mix_LoadWAV("../assets/audio/Bomb_Explosion.wav");
    playerHit = Mix_LoadWAV("../assets/audio/Player_hit_Sound.wav");
    powerUpPickUp = Mix_LoadWAV("../assets/audio/PickUp_Sound.wav");
    gameWinSound = Mix_LoadWAV("../assets/audio/Player_Win_Sound.wav");
    gameLostSound = Mix_LoadWAV("../assets/audio/Player_Lose_Sound.wav");

    Mix_PlayChannel(-1, music, -1);

}

// Comment
void MyGame::render(SDL_Renderer* renderer) {

    if (!disconnected)
    {
        level->drawMap(renderer);

        if(playersAlive[1] == 1) TextureManager::Draw(renderer, playerOneText, getPlayerOneRect());
        if(playersAlive[2] == 1) TextureManager::Draw(renderer, playerTwoText, getPlayerTwoRect());
        if(playersAlive[3] == 1) TextureManager::Draw(renderer, playerThreeText, getPlayerThreeRect());
        if(playersAlive[4] == 1) TextureManager::Draw(renderer, playerFourText, getPlayerFourRect());

        level->clearBombExplosion();
    
        drawUI(renderer);
    }
    else
    {
        textSurface = TTF_RenderText_Solid(font, "Server is full!", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
        textRect = { 10, 200, textW, textH };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        textSurface = TTF_RenderText_Solid(smallFont, "Sadly, we were unable to connect you", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
        textRect = { 70, 290, textW, textH };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        textSurface = TTF_RenderText_Solid(smallFont, "to the server as it was full", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
        textRect = { 150, 330, textW, textH };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }




}

void MyGame::drawUI(SDL_Renderer* renderer) {

    // Player One Lives
    TextureManager::Draw(renderer, playerOneText, { 70, 15, 30, 30 });

    textSurface = TTF_RenderText_Solid(smallFont, "X", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
    textRect = { 100 , 20, textW, textH };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    textSurface = TTF_RenderText_Solid(smallFont, std::to_string(game_data.player1Lives).c_str(), textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
    textRect = { 125, 20, textW, textH };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);


    // Player Two Lives

    TextureManager::Draw(renderer, playerTwoText, { 175, 15, 30, 30 });

    textSurface = TTF_RenderText_Solid(smallFont, "X", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
    textRect = { 205 , 20, textW, textH };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    textSurface = TTF_RenderText_Solid(smallFont, std::to_string(game_data.player2Lives).c_str(), textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
    textRect = { 230, 20, textW, textH };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    // Player Three Lives

    TextureManager::Draw(renderer, playerThreeText, { 280, 15, 30, 30 });

    textSurface = TTF_RenderText_Solid(smallFont, "X", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
    textRect = { 310 , 20, textW, textH };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    textSurface = TTF_RenderText_Solid(smallFont, std::to_string(game_data.player3Lives).c_str(), textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
    textRect = { 335, 20, textW, textH };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    // Player Four Lives

    TextureManager::Draw(renderer, playerFourText, { 385, 15, 30, 30 });

    textSurface = TTF_RenderText_Solid(smallFont, "X", textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
    textRect = { 415 , 20, textW, textH };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    textSurface = TTF_RenderText_Solid(smallFont, std::to_string(game_data.player4Lives).c_str(), textColor);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
    textRect = { 440, 20, textW, textH };
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
 

    if (gameIsOver)
    {
        textSurface = TTF_RenderText_Solid(font, "You have died!", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
        textRect = { 20, 200, textW, textH };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        textSurface = TTF_RenderText_Solid(smallFont, "You can spectate other players or leave", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
        textRect = { 30, 290, textW, textH };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

    if (gameIsWon)
    {
        textSurface = TTF_RenderText_Solid(font, "You have won!", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
        textRect = { 40, 200, textW, textH };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        textSurface = TTF_RenderText_Solid(smallFont, "Congratulations!", textColor);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
        textRect = { 260, 290, textW, textH };
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

}

void MyGame::quitGame() {
    TTF_CloseFont(font); 
    TTF_CloseFont(smallFont); 
    TTF_Quit();

    Mix_FreeChunk(playerHit);
    Mix_FreeChunk(powerUpPickUp);
    Mix_FreeChunk(gameLostSound);
    Mix_FreeChunk(gameWinSound);
    Mix_FreeChunk(bombExplosion);
    Mix_FreeChunk(music);
    Mix_CloseAudio();
    
}

