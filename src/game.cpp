#include "game.hpp"

Game::Game()
    :
    tmx(RESOURCES_PATH "tilemaps/tmx/grassfield1.tmx")
{
}

void Game::Init()
{

    tex.Load(RESOURCES_PATH "tilesets/Outdoors AutoTile Dark Grass.png");
}

void Game::Update()
{

}

void Game::Render()
{
    tmx.Draw({0,0});    
//    tex.Draw(raylib::Rectangle({0,0},{32,32}));
} 

void Game::Post_Update() 
{

} 


