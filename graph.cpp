//
// Created by Hayden Donofrio on 10/8/20.
//

#include "graph.h"
#include <fstream>
#include <iostream>

Graph::~Graph() {
    delete [] adjList;

}

void Graph::generateFile(int vertCount, std::string outputFile = "output.txt") {
    LinkedList<int> list;
    int counter = vertCount + 1;
    LinkedList<int> startingIndexes;
    for(int i = 1; i <= vertCount; i ++) {
        startingIndexes.push_back(counter);
        for (int j = 0; j < adjList[i].size(); j ++) {
            list.push_back(adjList[i].get(j));
            counter++;
        }
    }
    for (int i = startingIndexes.size() - 1; i >= 0; i --) {
        list.push_front(startingIndexes.get(i));
    }
    list.push_front(vertCount);
    std::ofstream outfile;
    outfile.open(outputFile);
    for (int i = 0; i < list.size(); i++) {
        outfile << list.get(i) << std::endl;
    }
    outfile.close();
}
