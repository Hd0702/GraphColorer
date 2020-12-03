//
// Created by Hayden Donofrio on 10/8/20.
//

#include "completeGraph.h"

CompleteGraph::CompleteGraph(int vertCount) {
    // amount of vertices is the amount of entries
    // remember we are base 1 here.
    adjList = new LinkedList<int> [vertCount + 1];
    for (int i = 1; i <= vertCount; i++) {
        for (int j = i + 1; j <= vertCount; j++) {
            adjList[j].push_back(i);
            adjList[i].push_back(j);
        }
    }
}
