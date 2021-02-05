//
// Created by Bertrand LE GAL on 25/01/2021.
//

#ifndef PathAnalyzer_H
#define PathAnalyzer_H

#include "Analyzer.hpp"

class PathAnalyzer : public Analyzer
{
public:

    PathAnalyzer(Maze& m);

    virtual ~PathAnalyzer();

    virtual void Update(Maze& m);

};

#endif //MAZE_MAZE_H
