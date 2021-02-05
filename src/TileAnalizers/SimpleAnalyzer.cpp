//
// Created by Bertrand LE GAL on 25/01/2021.
//

#include "SimpleAnalyzer.hpp"


SimpleAnalyzer::SimpleAnalyzer(Maze& m) : Analyzer( m )
{
    Update(m);
}


SimpleAnalyzer::~SimpleAnalyzer()
{

}


void SimpleAnalyzer::Update(Maze& m)
{
    for(int y = 0; y < h; y += 1)
    {
        for(int x = 0; x < w; x += 1)
        {
            texture[y*w+x] = m.tileType(x, y);
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
