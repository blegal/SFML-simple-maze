//
// Created by Bertrand LE GAL on 25/01/2021.
//

#ifndef SimpleAnalyzer_H
#define SimpleAnalyzer_H

#include "Analyzer.hpp"

class SimpleAnalyzer : public Analyzer
{
public:

    SimpleAnalyzer(Maze& m);

    ~SimpleAnalyzer();

    void Update(Maze& m);

};

#endif //MAZE_MAZE_H
