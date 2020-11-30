//
// Created by Hayden Donofrio on 11/19/20.
//

#include "smallestLast.h"

void SmallestLast::order() {
    int numVerts = getNumVerts();
    int currentEdgeIndex = 0;
    while (colorOrder.length() < numVerts) {
        LinkedList<int>::node * edgeFront = edgeList[currentEdgeIndex].front();
        if (edgeFront == nullptr) {
            currentEdgeIndex += 1;
            continue;
        }
        if (currentEdgeIndex + 1 == (numVerts - colorOrder.length()) && terminalCliqueSize == 0) {
            // terminal clique
            terminalCliqueSize = currentEdgeIndex + 1;
        }
        int currentVert = edgeFront->item;
        colorOrder.push_front(currentVert);
        edgeList[currentEdgeIndex].pop_front();
        adjList[currentVert].deleted = true;
        if (adjList[currentVert].degree > highestDegreeDeleted) {
            highestDegreeDeleted = adjList[currentVert].degree;
        }
        LinkedList<int>::node * vertex = adjList[currentVert].vertices.front();
        for (int i = 0; i < adjList[currentVert].vertices.length(); i++) {
            LinkedList<int>::node * currentEdge = adjList[vertex->item].edgeListEntry;
            if (!adjList[vertex->item].deleted) {
                adjList[vertex->item].edgeListEntry = &edgeList[adjList[vertex->item].degree - 1].push_front(vertex->item);
                edgeList[adjList[vertex->item].degree].remove(currentEdge);
                adjList[vertex->item].degree--;
            }
            vertex = vertex->next;
        }
        if (currentEdgeIndex != 0) {
            currentEdgeIndex--;
        }
    }
}
