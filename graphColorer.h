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
    AdjacencyListEntry * adjList;
    LinkedList<int> * edgeList;
    int * degreesWhenColored;
    LinkedList<int> colorOrder;
    int terminalCliqueSize = 0;
    int highestDegreeDeleted = 0;
public:
    virtual void order() = 0;
    int getNumVerts();
    void readFile(const std::string);
    int color();
    virtual ~GraphColorer();
    int getTerminalCliqueSize();
    int * getDegreesWhenColored();

};


#endif //HW3_GRAPHCOLORER_H
