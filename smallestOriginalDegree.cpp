//
// Created by Hayden Donofrio on 11/24/20.
//

#include "smallestOriginalDegree.h"

void SmallestOriginalDegree::order() {
    int numVerts = getNumVerts();
    int currentEdgeIndex = 0;
    LinkedList<int>::node * edgeFront = edgeList[0].front();
    while (colorOrder.length() < numVerts) {
        if (edgeFront == nullptr) {
            currentEdgeIndex += 1;
            edgeFront = edgeList[currentEdgeIndex].front();
            continue;
        }
        int currentVert = edgeFront->item;
        colorOrder.push_front(currentVert);
        adjList[currentVert].deleted = true;
        edgeFront = edgeFront->next;
    }
    int k = 0;
}