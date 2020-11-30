//
// Created by Hayden Donofrio on 10/8/20.
//

#include "cycleGraph.h"

CycleGraph::CycleGraph(int vertCount) {
    adjList = new LinkedList<int> [vertCount + 1];
    for (int i = 1; i <= vertCount; i++) {
        int addIndex = i + 1;
        if (i == vertCount) {
            addIndex = 1;
        }
        adjList[i].push_back(addIndex);
        adjList[addIndex].push_back(i);
    }
}