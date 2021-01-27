//
// Created by Bertrand LE GAL on 25/01/2021.
//

#include "Maze.hpp"

Maze::Maze(const uint32_t _w, const uint32_t _h)
{
    w = _w;
    h = _h;

    Level   = new uint8_t[w * h];
    visited = new uint8_t[w * h];
}

Maze::~Maze()
{
    delete[] Level;
    delete[] visited;
}

void Maze::generate()
{
    memset(Level,   UNKNOW_CELL, w * h);
    memset(visited, UNKNOW_CELL, w * h);

    for(int y = 0; y < h; y += 2)
        for(int x = 0; x < w; x += 1)
            Level[y * w + x] = WALL_CELL;

    for(int x = 0; x < w; x += 2)
        for(int y = 0; y < h; y += 1)
            Level[y * w + x] = WALL_CELL;

    //
    // On dessine le cadre du terrain de jeu
    //

    for(int x = 0; x < w; x += 1)
        Level[            x] = BORDER_CELL;
    for(int x = 0; x < w; x += 1)
        Level[(h-1) * w + x] = BORDER_CELL;

    for(int y = 0; y < h; y += 1)
        Level[y * w +    0 ] = BORDER_CELL;
    for(int y = 0; y < h; y += 1)
        Level[y * w + (w-1)] = BORDER_CELL;

    //
    //
    //

    for(int x = 1; x < w-1; x += 1)
        Level[1 * w + x] = VISITED_CELL;

    for(int y = 3; y < h; y += 2)
    {
        int startP = 1;
        for(int x = 1; x < w-1; x += 2)
        {
            if( (rand()%3 != 0) && (x != (w-2)) ){
                Level[y * w + x    ] = VISITED_CELL;
                Level[y * w + x + 1] = VISITED_CELL;
            }else{
                Level[y * w + x    ] = VISITED_CELL;
                const int route = (x - startP) / 2;
                const int truc = 2 * (rand() % ((route == 0)?1:route));
                Level[(y-1) * w + (x-truc)] = VISITED_CELL;
                startP = x + 2;
            }
        }
    }

    in_gate_x  = 0;
    in_gate_y  = h - 2;

    out_gate_x = w - 1;
    out_gate_y = h - 2;

    hero_x     = in_gate_x + 1;
    hero_y     = in_gate_y;

    tileType(in_gate_x,  out_gate_y, DOOR_CELL);
    tileType(out_gate_x, out_gate_y, DOOR_CELL);
//        tileType(hero_x,     hero_y,     HERO_DOWN);

//        Level[in_gate_y * w, in_gate_x] = DOOR_CELL; // On fait la barre du dessus
//        Level[(h-1) * w + porte_dw] = DOOR_CELL; // On fait la barre du dessus
//        Level[(h-2) * w + porte_dw] = HERO_DOWN; // On fait la barre du dessus

//        hero_x = porte_dw;
//        hero_y = (h-2);
}

uint32_t Maze::tileType(const int32_t x, const int32_t y)
{
    if( x <  0 ) return ERROR_CELL;
    if( x >= w ) return ERROR_CELL;
    if( y <  0 ) return ERROR_CELL;
    if( y >= h ) return ERROR_CELL;
    return Level[x + w * y];
}

void Maze::tileType(const int32_t x, const int32_t y, const uint32_t type)
{
    assert( x <  0 );
    assert( x >= w );
    assert( y <  0 );
    assert( y >= h );
    Level[x + w * y] = type;
}

void Maze::generate_2()
{
    memset(Level,   UNKNOW_CELL, w * h);
    memset(visited, UNKNOW_CELL, w * h);

    //
    // On trace la grille d'arbres realisant les délimiteurs
    //

    for(int y = 0; y < h; y += 2)
        for(int x = 0; x < w; x += 1)
            Level[y * w + x] = WALL_CELL;

    for(int x = 0; x < w; x += 2)
        for(int y = 0; y < h; y += 1)
            Level[y * w + x] = WALL_CELL;


    //
    // On trace les bords du terrain
    //

    for(int x = 0; x < w; x += 1)
    {
        Level[            x] = BORDER_CELL;
        Level[(h-1) * w + x] = BORDER_CELL;
    }

    for(int y = 0; y < h; y += 1)
    {
        Level[y * w +    0 ] = BORDER_CELL;
        Level[y * w + (w-1)] = BORDER_CELL;
    }


    //
    // On trace les bords du terrain
    //

    std::vector<int32_t> vec_x;
    std::vector<int32_t> vec_y;

    vec_x.push_back( (w/2) | 0x01 );
    vec_y.push_back( (h/2) | 0x01 );

//        printf("(II) The maze starting point (%d, %d) [%d, %d] is a WALL_CELL !\n", (w/2), (h/2), vec_x[0], vec_y[0]);
    if( tileType(vec_x[0], vec_y[0]) == WALL_CELL)
    {
        printf("(EE) The maze starting point [%d, %d] is a WALL_CELL !\n", vec_x[0], vec_y[0]);
        exit( EXIT_FAILURE );
    }

    while( vec_x.size() != 0 )
    {
        const int32_t cell = rand() % vec_x.size();

        int32_t posX = vec_x[cell];
        int32_t posY = vec_y[cell];

#ifdef _DEBUG_GENERATOR_
        printf("SELECTED CELL(%2d, %2d) WHO'S TYPE IS %d\n", posX, posY, tileType(posX, posY));
#endif

        vec_x.erase( vec_x.begin() + cell );
        vec_y.erase( vec_y.begin() + cell );

        if( tileType(posX, posY) == VISITED_CELL )
        {
#ifdef _DEBUG_GENERATOR_
            printf("===> SELECTED CELL(%2d, %2d) WAS ALREADY VISITED !!! <====\n", posX, posY);
#endif
            continue;
        }

        tileType(posX, posY, VISITED_CELL);

        std::vector<uint32_t> vis_x;
        std::vector<uint32_t> vis_y;

        if( tileType(posX + 0, posY - 2) == VISITED_CELL ){
            vis_x.push_back( +0 ); vis_y.push_back( -1 );
#ifdef _DEBUG_GENERATOR_
            printf(" -> CELL UP    WAS VISITED   [%2d, %2d]\n", vis_x[vis_x.size()-1], vis_y[vis_y.size()-1]);
#endif
        }
        if( tileType(posX + 2, posY + 0) == VISITED_CELL ){
            vis_x.push_back( +1 ); vis_y.push_back( +0 );
#ifdef _DEBUG_GENERATOR_
            printf(" -> CELL RIGHT WAS VISITED   [%2d, %2d]\n", vis_x[vis_x.size()-1], vis_y[vis_y.size()-1]);
#endif
        }
        if( tileType(posX + 0, posY + 2) == VISITED_CELL ){
            vis_x.push_back( +0 ); vis_y.push_back( +1 );
#ifdef _DEBUG_GENERATOR_
            printf(" -> CELL DOWN  WAS VISITED   [%2d, %2d]\n", vis_x[vis_x.size()-1], vis_y[vis_y.size()-1]);
#endif
        }
        if( tileType(posX - 2, posY + 0) == VISITED_CELL ){
            vis_x.push_back( -1 ); vis_y.push_back( +0 );
#ifdef _DEBUG_GENERATOR_
            printf(" -> CELL LEFT  WAS VISITED   [%2d, %2d]\n", vis_x[vis_x.size()-1], vis_y[vis_y.size()-1]);
#endif
        }
#ifdef _DEBUG_GENERATOR_
        for (int ll = 0; ll < vis_x.size(); ll++)
                printf(" -> VISITED NEAR [%2d, %2d]\n", vis_x[ll], vis_y[ll]);
#endif
        if( vis_x.size() != 0 )
        {
            int choice = rand()%vis_x.size();
            tileType(posX + 1 * vis_x[choice], posY + 1 * vis_y[choice], VISITED_CELL);
            tileType(posX + 2 * vis_x[choice], posY + 2 * vis_y[choice], VISITED_CELL);
#ifdef _DEBUG_GENERATOR_
            printf(" +> CELL SELECTED (%d) : [%2d, %2d]\n", choice, posX + 2 * vis_x[choice], posY + 2 * vis_y[choice]);
#endif
        }

        if( tileType(posX + 0, posY - 2) == UNKNOW_CELL ){
            if( tileType(posX + 2, posY + 0) == UNKNOW_CELL ) {
                bool already_in = false;
                for (uint32_t i = 0; i < vec_x.size(); i += 1) {
                    if ((vec_x[i] == (posX)) && (vec_y[i] == (posY - 2)))
                        already_in = true;
                }
                if (already_in == false) {
                    vec_x.push_back(posX + 0);
                    vec_y.push_back(posY - 2);
                }
            }
//                printf(" => CELL UP    IS UNVISITED   [%2d, %2d]\n", vec_x[vec_x.size()-1], vec_y[vec_y.size()-1]);
        }
        if( tileType(posX + 2, posY + 0) == UNKNOW_CELL ){
            bool already_in = false;
            for(uint32_t i = 0; i < vec_x.size(); i += 1)
            {
                if( (vec_x[i] == (posX+2)) && (vec_y[i] == (posY) ) )
                    already_in = true;
            }
            if( already_in == false )
            {
                vec_x.push_back( posX+2 );
                vec_y.push_back( posY+0 );
            }
//                printf(" => CELL RIGHT IS UNVISITED   [%2d, %2d]\n", vec_x[vec_x.size()-1], vec_y[vec_y.size()-1]);
        }
        if( tileType(posX + 0, posY + 2) == UNKNOW_CELL ){
            bool already_in = false;
            for(uint32_t i = 0; i < vec_x.size(); i += 1)
            {
                if( (vec_x[i] == (posX)) && (vec_y[i] == (posY+2) ) )
                    already_in = true;
            }
            if( already_in == false )
            {
                vec_x.push_back( posX+0 );
                vec_y.push_back( posY+2 );
            }
#ifdef _DEBUG_GENERATOR_
            printf(" => CELL DOWN  IS UNVISITED   [%2d, %2d]\n", vec_x[vec_x.size()-1], vec_y[vec_y.size()-1]);
#endif
        }
        if( tileType(posX - 2, posY + 0) == UNKNOW_CELL ){
            bool already_in = false;
            for(uint32_t i = 0; i < vec_x.size(); i += 1)
            {
                if( (vec_x[i] == (posX-2)) && (vec_y[i] == (posY-2) ) )
                    already_in = true;
            }
            if( already_in == false )
            {
                vec_x.push_back( posX-2 );
                vec_y.push_back( posY+0 );
            }
        }

#ifdef _DEBUG_GENERATOR_
        if( vec_x.size() != 0 )
            {
                printf("Possible choice for next round\n");
                for (int ll = 0; ll < vec_x.size(); ll++)
                    printf(" + [%2d, %2d]\n", vec_x[ll], vec_y[ll]);
                printf("\n");
            }
#endif

#ifdef _DEBUG_GENERATOR_
        for (int yy = 0; yy < h; yy++) {
                for (int xx = 0; xx < w; xx++) {
                    if( tileType(xx, yy) == UNKNOW_CELL  ) printf("?");
                    if( tileType(xx, yy) == VISITED_CELL ) printf(".");
                    if( tileType(xx, yy) == WALL_CELL    ) printf("#");
                    if( tileType(xx, yy) == BORDER_CELL  ) printf("*");
                }
                printf("\n");
            }
            printf("\n");
            printf("\n");
#endif
    }
#if 0
    for(int x = 1; x < w-1; x += 1)
            Level[1 * w + x] = VISITED_CELL; // On fait la barre du dessus

        for(int y = 3; y < h; y += 2)
        {
            int startP = 1;
            for(int x = 1; x < w-1; x += 2)
            {
                if( (rand()%3 != 0) && (x != (w-2)) ){
                    Level[y * w + x    ] = VISITED_CELL;
                    Level[y * w + x + 1] = VISITED_CELL;
                }else{
                    Level[y * w + x    ] = VISITED_CELL;
                    const int route = (x - startP) / 2;
                    const int truc = 2 * (rand() % ((route == 0)?1:route));
                    Level[(y-1) * w + (x-truc)] = VISITED_CELL;
                    startP = x + 2;
                }
            }
        }
#endif
    in_gate_x  = 0;
    in_gate_y  = h - 2;

    out_gate_x = w - 1;
    out_gate_y = h - 2;

    hero_x     = in_gate_x;// + 1;
    hero_y     = in_gate_y;

    tileType( in_gate_x,  in_gate_y, DOOR_CELL);
    tileType(out_gate_x, out_gate_y, DOOR_CELL);
}

bool Maze::isGameFinished()
{
    return  ((hero_x == out_gate_x) && (hero_y == out_gate_y));
}

uint32_t Maze::Width ()
{
    return  w;
}

uint32_t Maze::Height()
{
    return h;
}

bool Maze::CanHeroMoveUp()
{
    const uint32_t cell = tileType(hero_x, hero_y - 1);
    if( cell == VISITED_CELL ) return true;
    if( cell == DOOR_CELL    ) return true;
    return false;
}

void Maze::MoveHeroUp()
{
    if( CanHeroMoveUp() == true )
        hero_y -= 1;
}

bool Maze::CanHeroMoveDown()
{
    const uint32_t cell = tileType(hero_x, hero_y + 1);
    if( cell == VISITED_CELL ) return true;
    if( cell == DOOR_CELL    ) return true;
    return false;
}

void Maze::MoveHeroDown()
{
    if( CanHeroMoveDown() == true )
        hero_y += 1;
}

bool Maze::CanHeroMoveLeft()
{
    const uint32_t cell = tileType(hero_x - 1, hero_y);
    if( cell == VISITED_CELL ) return true;
    if( cell == DOOR_CELL    ) return true;
    return false;
}

void Maze::MoveHeroLeft()
{
    if( CanHeroMoveLeft() == true )
        hero_x -= 1;
}

bool Maze::CanHeroMoveRight()
{
    const uint32_t cell = tileType(hero_x + 1, hero_y);
    if( cell == VISITED_CELL ) return true;
    if( cell == DOOR_CELL    ) return true;
    return false;
}

void Maze::MoveHeroRight()
{
    if( CanHeroMoveRight() == true )
        hero_x += 1;
}

uint32_t Maze::GetHeroPosX()
{
    return ( hero_x );
}

uint32_t Maze::GetHeroPosY()
{
    return ( hero_y );
}

uint8_t* Maze::GetLevel()
{
    return Level;
}