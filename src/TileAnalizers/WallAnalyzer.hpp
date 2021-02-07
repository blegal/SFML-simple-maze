//
// Created by Bertrand LE GAL on 25/01/2021.
//

#ifndef WallAnalyzer_H
#define WallAnalyzer_H

#include "Analyzer.hpp"

class WallAnalyzer : public Analyzer
{
public:

    WallAnalyzer(Maze& m);

    virtual ~WallAnalyzer();

    virtual void Update(Maze& m);

};

#endif //MAZE_MAZE_H
