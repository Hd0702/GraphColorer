//
// Created by Hayden Donofrio on 11/25/20.
//

#include "incidenceOrder.h"

void IncidenceOrder::order() {
    // start with setting all vertices to 0 in deleted incidence
    int numVertices = getNumVerts();
    LinkedList<int> * incidenceOrder = new LinkedList<int>[numVertices + 1];
    for (int i = 1; i <= numVertices; i++) {
        adjList[i].edgeListEntry = &incidenceOrder[0].push_front(i);
    }
    int highestIncidence = 0;
    LinkedList<int>::node * currentVert;
    // largest amount of colored neighbors goes as current node
    while (colorOrder.length() < numVertices) {
        currentVert = incidenceOrder[highestIncidence].front();
        if (currentVert == nullptr) {
            highestIncidence--;
            continue;
        }
        int currentVertDegree = currentVert->item;
        colorOrder.push_back(currentVertDegree);
        incidenceOrder[highestIncidence].pop_front();
        adjList[currentVertDegree].deleted = true;
        LinkedList<int>::node * vertex = adjList[currentVertDegree].vertices.front();
        for (int i = 0; i < adjList[currentVertDegree].vertices.length(); i++) {
            LinkedList<int>::node * currentEdge = adjList[vertex->item].edgeListEntry;
            if (!adjList[vertex->item].deleted) {
                adjList[vertex->item].edgeListEntry = &incidenceOrder[adjList[vertex->item].deletedIncidenceDegree + 1].push_front(vertex->item);
                incidenceOrder[adjList[vertex->item].deletedIncidenceDegree++].remove(currentEdge);
                if (adjList[vertex->item].deletedIncidenceDegree > highestIncidence) {
                    highestIncidence = adjList[vertex->item].deletedIncidenceDegree;
                }
            }
            vertex = vertex->next;
        }
    }
    delete [] incidenceOrder;
}