//
// Created by Bertrand LE GAL on 25/01/2021.
//

#include "Analyzer.hpp"


Analyzer::Analyzer(Maze& m)
{
    w = m.Width ();
    h = m.Height();

    texture = new uint8_t[w * h];
}


Analyzer::~Analyzer()
{
    delete[] texture;
}


uint8_t* Analyzer::GetSprites()
{
    return texture;
}
