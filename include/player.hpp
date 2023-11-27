#pragma once

#include "raylib-cpp.hpp"

class Player
{
public:
    Player();

    const unsigned long walk_speed;
    const unsigned long sprint_speed;
    const unsigned long sneek_speed;

    raylib::Texture2D sprite;

    raylib::Vector2 position;
};
