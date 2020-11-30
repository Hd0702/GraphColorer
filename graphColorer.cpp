//
// Created by Hayden Donofrio on 11/19/20.
//

#include "graphColorer.h"

GraphColorer::~GraphColorer() {
    delete [] adjList;
    delete [] edgeList;
}

int GraphColorer::getNumVerts() {
    return numVerts;
}


int GraphColorer::color() {
    int highestColorUsed = 0;
    while (colorOrder.length() > 0) {
        int vertex = colorOrder.front()->item;
        colorOrder.pop_front();
        int lowestColor = 1;
        bool * colorsAvailable = new bool[numVerts];
        for (int i =0; i < numVerts + 1; i++) {
            colorsAvailable[i] = true;
        }
        // this is 10000 colors,
        LinkedList<int>::node * currentItem = adjList[vertex].vertices.front();
        // V squared + E complexity
        while (currentItem != nullptr) {
            colorsAvailable[adjList[currentItem->item].colorValue] = false;
            currentItem = currentItem->next;
        }
        for (int i = 1; i < numVerts + 1; i++) {
            if (colorsAvailable[i] == true) {
                lowestColor = i;
                break;
            }
        }
        delete [] colorsAvailable;
        if (lowestColor > highestColorUsed) {
            highestColorUsed = lowestColor;
        }
        adjList[vertex].colorValue = lowestColor;
    }
    return highestColorUsed;
}

void GraphColorer::readFile(const std::string fileName) {
    std::fstream fileReader;
    fileReader.open(fileName, std::ios::in);
    if (fileReader.is_open()) {
        std::string line;
        getline(fileReader, line);
        numVerts = std::stoi(line); // first entry is the number of vertices
        adjList = new AdjacencyListEntry[numVerts + 1];
        edgeList = new LinkedList<int>[numVerts];
        int * vertexLocations = new int[numVerts + 1];
        vertexLocations[0] = 0;
        getline(fileReader, line); // skip the line for the first entry
        int previousEntry = std::stoi(line);
        for (int i = 1; i < numVerts; i++) {
            // iterate through each vertex and get the number of entries for each vertex
            getline(fileReader, line);
            int newEntry = std::stoi(line);
            // 1 minus the number of vertices - amount of previous vertices
            vertexLocations[i] = newEntry - previousEntry;
            previousEntry = newEntry;
        }
        for (int i = 1; i < numVerts; i++) {
            int numEdges = vertexLocations[i];
            int edgeCount = 0;
            for (int j = 0; j < numEdges; j++) {
                getline(fileReader, line);
                int edge = std::stoi(line);
                adjList[i].vertices.push_front(edge);
                edgeCount += 1;
            }
            adjList[i].degree = edgeCount;
            adjList[i].originalDegree = edgeCount;
            adjList[i].edgeListEntry = &edgeList[edgeCount].push_front(i);
        }
        // since we are not sure how many edges are in the last vertex, read until the end of file
        int finalEdgeCounter = 0;
        while(getline(fileReader, line)) {
            adjList[numVerts].vertices.push_front(std::stoi(line));
            finalEdgeCounter += 1;
        }
        adjList[numVerts].degree = finalEdgeCounter;
        adjList[numVerts].originalDegree = finalEdgeCounter;
        adjList[numVerts].edgeListEntry = &edgeList[finalEdgeCounter].push_front(numVerts);

        fileReader.close(); //close the file object.
    }
}