//
// Created by Bertrand LE GAL on 25/01/2021.
//

#ifndef MAZE_MAZESOLVER_H
#define MAZE_MAZESOLVER_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <vector>

#include "Tiles/MazeTiles.hpp"
#include "Mazes/Maze.hpp"

class MazeSolver
{
public:
    std::vector<uint32_t> cx;
    std::vector<uint32_t> cy;
    std::vector<uint32_t> fleche;

private:
    uint32_t w;
    uint32_t h;

    uint8_t* maze;

public:

    MazeSolver(Maze& m);

    void Update(Maze& m);


    ~MazeSolver();

    uint32_t get_xy(const int32_t x, const int32_t y);
    void     set_xy(const int32_t x, const int32_t y, const uint32_t type);

private:
    bool recursive_search(const uint32_t posX, const uint32_t posY);

};

#endif //MAZE_MAZESOLVER_H
