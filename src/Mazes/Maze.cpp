//
// Created by Bertrand LE GAL on 25/01/2021.
//

#include "Maze.hpp"


Maze::Maze(const uint32_t _w, const uint32_t _h)
{
    w = _w;
    h = _h;

    Level   = new uint8_t[w * h];
    texture = new uint8_t[w * h];
#ifdef _my_debug_
    buffer  = new uint8_t[w * h];
#endif
}


Maze::~Maze()
{
    delete[] Level;
    delete[] texture;
#ifdef _MY_DEBUG_
    delete[] buffer;
#endif
}


void Maze::generate()
{
    memset(Level,   T_INITIAL, w * h);
    memset(texture, T_INITIAL, w * h);

    for(int y = 0; y < h; y += 2)
        for(int x = 0; x < w; x += 1)
            Level[y * w + x] = T_OBSTACLE;

    for(int x = 0; x < w; x += 2)
        for(int y = 0; y < h; y += 1)
            Level[y * w + x] = T_OBSTACLE;

    //
    // On dessine le cadre du terrain de jeu
    //

    for(int x = 0; x < w; x += 1)
        Level[            x] = T_CADRE;
    for(int x = 0; x < w; x += 1)
        Level[(h-1) * w + x] = T_CADRE;

    for(int y = 0; y < h; y += 1)
        Level[y * w +    0 ] = T_CADRE;
    for(int y = 0; y < h; y += 1)
        Level[y * w + (w-1)] = T_CADRE;

    //
    //
    //

    for(int x = 1; x < w-1; x += 1)
        Level[1 * w + x] = T_CHEMIN;

    for(int y = 3; y < h; y += 2)
    {
        int startP = 1;
        for(int x = 1; x < w-1; x += 2)
        {
            if( (rand()%3 != 0) && (x != (w-2)) ){
                Level[y * w + x    ] = T_CHEMIN;
                Level[y * w + x + 1] = T_CHEMIN;
            }else{
                Level[y * w + x    ] = T_CHEMIN;
                const int route = (x - startP) / 2;
                const int truc = 2 * (rand() % ((route == 0)?1:route));
                Level[(y-1) * w + (x-truc)] = T_CHEMIN;
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

    tileType(in_gate_x,  out_gate_y, T_MAZE_START);
    tileType(out_gate_x, out_gate_y, T_MAZE_STOP );

    BasicSprites();
}


void Maze::AdvancedSprites()
{
    for(int y = 0; y < h; y += 1)
    {
        for(int x = 0; x < w; x += 1)
        {
#ifdef _MY_DEBUG_
            buffer[y*w+x] = 0;
#endif
            if( tileType(x, y) != T_CHEMIN )
            {
                texture[x + w * y] = tileType(x, y);
                continue;
            }
            const bool up       = tileType(x+0, y-1) == T_CHEMIN;
            const bool left     = tileType(x-1, y+0) == T_CHEMIN;
            const bool right    = tileType(x+1, y+0) == T_CHEMIN;
            const bool down     = tileType(x+0, y+1) == T_CHEMIN;
            const uint32_t code = (up << 3 ) | (left << 2 ) | (down << 1 ) | (right);
#ifdef _MY_DEBUG_
            buffer[y*w+x] = code;
#endif
            switch( code )
            {
                //
                // On a une seule conection
                //
                case 0x01 : texture[y*w+x] = T_1x_RIGHT; break;
                case 0x02 : texture[y*w+x] = T_1x_DOWN;  break;
                case 0x04 : texture[y*w+x] = T_1x_LEFT;  break;
                case 0x08 : texture[y*w+x] = T_1x_UP;    break;
                    //
                case 0x05 : texture[y*w+x] = T_2x_LR;    break;
                case 0x0A : texture[y*w+x] = T_2x_UD;    break;
                    //
                case 0x03 : texture[y*w+x] = T_2x_RD;    break;
                case 0x06 : texture[y*w+x] = T_2x_DL;    break; // OK
                case 0x09 : texture[y*w+x] = T_2x_UR;    break; // OK
                case 0x0C : texture[y*w+x] = T_2x_LU;    break;
                    //
                case 0x0B : texture[y*w+x] = T_3x_LEFT;  break;
                case 0x07 : texture[y*w+x] = T_3x_UP;    break;
                case 0x0E : texture[y*w+x] = T_3x_RIGHT; break;
                case 0x0D : texture[y*w+x] = T_3x_DOWN;  break;
                    //
                case 0x0F : texture[y*w+x] = T_4x;       break;
            }
        }
    }

#ifdef _MY_DEBUG_
    printf("\n");
    printf("Analyse de la texture\n");
    for(int y = 0; y < h; y += 1)
    {
        for(int x = 0; x < w; x += 1)
        {
            printf("%2d ", (uint32_t)buffer[y*w+x]);
        }
        printf("\n");
    }
    printf("\n");

    printf("\n");
    printf("Analyse de la texture\n");
    for(int y = 0; y < h; y += 1)
    {
        for(int x = 0; x < w; x += 1)
        {
            printf("%2d ", (uint32_t)texture[y*w+x]);
        }
        printf("\n");
    }
    printf("\n");
#endif
}


void Maze::BasicSprites()
{
    for(int y = 0; y < h; y += 1)
    {
        for(int x = 0; x < w; x += 1)
        {
            texture[y*w+x] = tileType(x, y);
        }
    }

#ifdef _MY_DEBUG_
    printf("\n");
    printf("Analyse des données entrantes\n");
    for(int y = 0; y < h; y += 1)
    {
        for(int x = 0; x < w; x += 1)
        {
            printf("%2d ", (uint32_t)tileType(x, y));
        }
        printf("\n");
    }
    printf("\n");

    printf("\n");
    printf("Analyse de la texture\n");
    for(int y = 0; y < h; y += 1)
    {
        for(int x = 0; x < w; x += 1)
        {
            printf("%2d ", (uint32_t)texture[y*w+x]);
        }
        printf("\n");
    }
    printf("\n");
#endif

}


uint32_t Maze::tileType(const int32_t x, const int32_t y)
{
    if( x <  0 ) return T_OBSTACLE;
    if( x >= w ) return T_OBSTACLE;
    if( y <  0 ) return T_OBSTACLE;
    if( y >= h ) return T_OBSTACLE;
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
    memset(Level,   T_INITIAL, w * h);
    memset(texture, T_INITIAL, w * h);

    //
    // On trace la grille d'arbres realisant les délimiteurs
    //

    for(int y = 0; y < h; y += 2)
        for(int x = 0; x < w; x += 1)
            Level[y * w + x] = T_OBSTACLE;

    for(int x = 0; x < w; x += 2)
        for(int y = 0; y < h; y += 1)
            Level[y * w + x] = T_OBSTACLE;


    //
    // On trace les bords du terrain
    //

    for(int x = 0; x < w; x += 1)
    {
        Level[            x] = T_CADRE;
        Level[(h-1) * w + x] = T_CADRE;
    }

    for(int y = 0; y < h; y += 1)
    {
        Level[y * w +    0 ] = T_CADRE;
        Level[y * w + (w-1)] = T_CADRE;
    }


    //
    // On trace les bords du terrain
    //

    std::vector<int32_t> vec_x;
    std::vector<int32_t> vec_y;

    vec_x.push_back( (w/2) | 0x01 );
    vec_y.push_back( (h/2) | 0x01 );

//        printf("(II) The maze starting point (%d, %d) [%d, %d] is a WALL_CELL !\n", (w/2), (h/2), vec_x[0], vec_y[0]);
    if( tileType(vec_x[0], vec_y[0]) == T_OBSTACLE)
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

        if( tileType(posX, posY) == T_CHEMIN )
        {
#ifdef _DEBUG_GENERATOR_
            printf("===> SELECTED CELL(%2d, %2d) WAS ALREADY VISITED !!! <====\n", posX, posY);
#endif
            continue;
        }

        tileType(posX, posY, T_CHEMIN);

        std::vector<uint32_t> vis_x;
        std::vector<uint32_t> vis_y;

        if( tileType(posX + 0, posY - 2) == T_CHEMIN ){
            vis_x.push_back( +0 ); vis_y.push_back( -1 );
#ifdef _DEBUG_GENERATOR_
            printf(" -> CELL UP    WAS VISITED   [%2d, %2d]\n", vis_x[vis_x.size()-1], vis_y[vis_y.size()-1]);
#endif
        }
        if( tileType(posX + 2, posY + 0) == T_CHEMIN ){
            vis_x.push_back( +1 ); vis_y.push_back( +0 );
#ifdef _DEBUG_GENERATOR_
            printf(" -> CELL RIGHT WAS VISITED   [%2d, %2d]\n", vis_x[vis_x.size()-1], vis_y[vis_y.size()-1]);
#endif
        }
        if( tileType(posX + 0, posY + 2) == T_CHEMIN ){
            vis_x.push_back( +0 ); vis_y.push_back( +1 );
#ifdef _DEBUG_GENERATOR_
            printf(" -> CELL DOWN  WAS VISITED   [%2d, %2d]\n", vis_x[vis_x.size()-1], vis_y[vis_y.size()-1]);
#endif
        }
        if( tileType(posX - 2, posY + 0) == T_CHEMIN ){
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
            tileType(posX + 1 * vis_x[choice], posY + 1 * vis_y[choice], T_CHEMIN);
            tileType(posX + 2 * vis_x[choice], posY + 2 * vis_y[choice], T_CHEMIN);
#ifdef _DEBUG_GENERATOR_
            printf(" +> CELL SELECTED (%d) : [%2d, %2d]\n", choice, posX + 2 * vis_x[choice], posY + 2 * vis_y[choice]);
#endif
        }

        if( tileType(posX + 0, posY - 2) == T_INITIAL ){
            if( tileType(posX + 2, posY + 0) == T_INITIAL ) {
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
        if( tileType(posX + 2, posY + 0) == T_INITIAL ){
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
        if( tileType(posX + 0, posY + 2) == T_INITIAL ){
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
        if( tileType(posX - 2, posY + 0) == T_INITIAL ){
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

    hero_x     = in_gate_x + 1;
    hero_y     = in_gate_y;

    tileType( in_gate_x,  in_gate_y, T_MAZE_START);
    tileType(out_gate_x, out_gate_y, T_MAZE_STOP );

    BasicSprites();
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
    if( cell == T_CHEMIN    ) return true;
    if( cell == T_MAZE_START) return true;
    if( cell == T_MAZE_STOP ) return true;
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
    if( cell == T_CHEMIN ) return true;
    if( cell == T_MAZE_START) return true;
    if( cell == T_MAZE_STOP ) return true;
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
    if( cell == T_CHEMIN ) return true;
    if( cell == T_MAZE_START) return true;
    if( cell == T_MAZE_STOP ) return true;
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
    if( cell == T_CHEMIN ) return true;
    if( cell == T_MAZE_START) return true;
    if( cell == T_MAZE_STOP ) return true;
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


uint8_t* Maze::GetSprites()
{
    return texture;
}
