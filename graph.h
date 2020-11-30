//
// Created by Hayden Donofrio on 10/8/20.
//

#ifndef HW3_GRAPH_H
#define HW3_GRAPH_H
#include "../HW3/linkedList.h"
#include <string>
class Graph {
protected:
    int numVerts;
    LinkedList<int> * adjList;
public:
    void generateFile(int, std::string);
    virtual ~Graph();

};


#endif //HW3_GRAPH_H
