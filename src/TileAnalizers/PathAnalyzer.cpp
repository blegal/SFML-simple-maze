//
// Created by Bertrand LE GAL on 25/01/2021.
//

#include "PathAnalyzer.hpp"


PathAnalyzer::PathAnalyzer(Maze& m) : Analyzer( m )
{
    Update(m);
}


PathAnalyzer::~PathAnalyzer()
{

}


void PathAnalyzer::Update(Maze& m)
{
    for(int y = 0; y < h; y += 1)
    {
        for(int x = 0; x < w; x += 1)
        {
            if( m.tileType(x, y) != T_CHEMIN )
            {
                texture[x + w * y] = m.tileType(x, y);
                continue;
            }
            const bool up       = m.tileType(x+0, y-1) == T_CHEMIN;
            const bool left     = m.tileType(x-1, y+0) == T_CHEMIN;
            const bool right    = m.tileType(x+1, y+0) == T_CHEMIN;
            const bool down     = m.tileType(x+0, y+1) == T_CHEMIN;
            const uint32_t code = (up << 3 ) | (left << 2 ) | (down << 1 ) | (right);
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

#if 0
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
