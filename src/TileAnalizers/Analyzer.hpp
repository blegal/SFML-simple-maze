//
// Created by Bertrand LE GAL on 25/01/2021.
//

#ifndef Analyzer_H
#define Analyzer_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cassert>

#include "Mazes/Maze.hpp"
#include "Tiles/MazeTiles.hpp"

class Analyzer
{
protected:
    uint32_t w;
    uint32_t h;

    uint8_t* texture;

public:

    Analyzer(Maze& m);

    virtual ~Analyzer();

    virtual void Update(Maze& m)  = 0;

public:
    uint8_t* GetSprites();
};

#endif //MAZE_MAZE_H
