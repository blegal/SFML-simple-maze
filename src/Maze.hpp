//
// Created by Bertrand LE GAL on 25/01/2021.
//

#ifndef MAZE_MAZE_H
#define MAZE_MAZE_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cassert>

#include "MazeTiles.hpp"

class Maze
{
private:
    uint32_t w;
    uint32_t h;

    uint32_t in_gate_x;
    uint32_t in_gate_y;

    uint32_t out_gate_x;
    uint32_t out_gate_y;

    uint32_t hero_x;
    uint32_t hero_y;

    uint8_t* Level;
    uint8_t* texture;
//#define _MY_DEBUG_
#ifdef _MY_DEBUG_
    uint8_t* buffer;
#endif

public:

    Maze(const uint32_t _w, const uint32_t _h);

    ~Maze();

    void generate();

    uint32_t tileType(const int32_t x, const int32_t y);

    void tileType(const int32_t x, const int32_t y, const uint32_t type);

    void generate_2();

    void BasicSprites();
    void AdvancedSprites();

    bool isGameFinished();

    uint32_t Width ();
    uint32_t Height();

public:
    bool CanHeroMoveUp();
    bool CanHeroMoveRight();
    bool CanHeroMoveDown();
    bool CanHeroMoveLeft();

public:
    void MoveHeroUp();
    void MoveHeroRight();
    void MoveHeroDown();
    void MoveHeroLeft();

public:
    uint32_t GetHeroPosX();
    uint32_t GetHeroPosY();

public:
    uint8_t* GetLevel();
    uint8_t* GetSprites();
};

#endif //MAZE_MAZE_H
