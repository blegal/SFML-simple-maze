//
// Created by Bertrand LE GAL on 25/01/2021.
//

#include "MazeSolver.hpp"


bool MazeSolver::recursive_search(const uint32_t posX, const uint32_t posY)
{
    if( get_xy(posX, posY) == 2 )
        return true;

    set_xy(posX, posY, 1); // On transforme la case en mur pour eviter les boucles

    cx.push_back( posX );
    cy.push_back( posY );

    if( get_xy(posX+1, posY) != 1 ) // 0 = road, 1 = wall, 2 = error
    {
        const bool ok = recursive_search(posX+1, posY);
        if( ok ) { set_xy(posX, posY, 0); return true; }
    }

    if( get_xy(posX, posY-1) != 1 ) // 0 = road, 1 = wall, 2 = error
    {
        const bool ok = recursive_search(posX, posY-1);
        if( ok ) { set_xy(posX, posY, 0); return true; }
    }

    if( get_xy(posX, posY+1) != 1 ) // 0 = road, 1 = wall, 2 = error
    {
        const bool ok = recursive_search(posX, posY+1);
        if( ok ) { set_xy(posX, posY, 0); return true; }
    }

    if( get_xy(posX-1, posY) != 1 ) // 0 = road, 1 = wall, 2 = error
    {
        const bool ok = recursive_search(posX-1, posY);
        if( ok ) { set_xy(posX, posY, 0); return true; }
    }

    set_xy(posX, posY, 0); // On restaure la case comme etant utilisable

    cx.pop_back();
    cy.pop_back();

    return false;
}

MazeSolver::MazeSolver(Maze& m) {
    w = m.Width();
    h = m.Height();

    maze = new uint8_t[w * h];
}

void MazeSolver::Update(Maze& m)
{
    cx.clear();
    cy.clear();

    for(uint32_t p = 0; p < w * h; p += 1)
    {
        const uint32_t v = m.GetLevel()[p];
        if     ( v == WALL_CELL   ) maze[p] = 1;
        else if( v == BORDER_CELL ) maze[p] = 1;
        else                        maze[p] = 0;
    }

    set_xy(w-1, h-2, 2);

    bool ok = recursive_search(0, h-2);
    if( ok )
        printf("(EE) No maze solution !!!\n");
}

MazeSolver::~MazeSolver()
{
    delete[] maze;
}

uint32_t MazeSolver::get_xy(const int32_t x, const int32_t y)
{
    if( x <  0 ) return 1; // like a wall
    if( x >= w ) return 1; // like a wall
    if( y <  0 ) return 1; // like a wall
    if( y >= h ) return 1; // like a wall
    return maze[x + w * y];
}

void MazeSolver::set_xy(const int32_t x, const int32_t y, const uint32_t type)
{
    assert( x <  0 );
    assert( x >= w );
    assert( y <  0 );
    assert( y >= h );
    maze[x + w * y] = type;
}
