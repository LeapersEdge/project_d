#include "game.hpp"

Game::Game()
    :
    tmx(RESOURCES_PATH "tilemaps/tmx/grassfield1.tmx")
{
}

void Game::Init()
{

    tmx.Draw({0,0});    
}

void Game::Update()
{

}

void Game::Render()
{
}

void Game::Post_Update()
{

} 


