//
// Created by Hayden Donofrio on 10/8/20.
//

#ifndef HW3_RANDOMGRAPH_H
#define HW3_RANDOMGRAPH_H
#include "../HW3/graph.h"
#include <string>
class RandomGraph: public Graph {
private:
    std::string distributionType;
    int distrCounter = 1;
    int distrSize = 1;
    int * randomDistribution = nullptr;
    int * numConflicts;
    int GetRandomNumber(int, std::string);
public:
    RandomGraph(int, int, std::string);
    ~RandomGraph();
    int * GetNumConflicts();
};


#endif //HW3_RANDOMGRAPH_H
