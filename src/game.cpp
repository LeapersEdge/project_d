#include "game.hpp"

Game::Game()
    :
    tmx(RESOURCES_PATH "tilemaps/tmx/grassfield1.tmx")
{
}

void Game::Init()
{

}

void Game::Update()
{

}

void Game::Render()
{
    tmx.Draw({0,0});    
} 

void Game::Post_Update() 
{

} 


