//
// Created by Hayden Donofrio on 11/19/20.
//

#ifndef HW3_GRAPHCOLORER_H
#define HW3_GRAPHCOLORER_H
#include "../HW3/linkedList.h"
#include <string>
#include <fstream>

struct AdjacencyListEntry {
    int degree = 0;
    int colorValue = 0;
    bool deleted = false;
    int originalDegree = 0;
    int deletedIncidenceDegree = 0;
    int saturationDegree = 0;
    LinkedList<int> vertices;
    LinkedList<int>::node * edgeListEntry;
};

class GraphColorer {
private:
    int numVerts;
protected:
    // this should be an array of size verticies not a list
    // this is so we can immediately jump to vertices
    AdjacencyListEntry * adjList;
    LinkedList<int> * edgeList;
    LinkedList<int> colorOrder;
    int terminalCliqueSize = 0;
    int highestDegreeDeleted = 0;
public:
    virtual void order() = 0;
    int getNumVerts();
    void readFile(const std::string);
    int color();
    virtual ~GraphColorer();

};


#endif //HW3_GRAPHCOLORER_H
