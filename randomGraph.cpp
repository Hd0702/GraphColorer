//
// Created by Hayden Donofrio on 10/8/20.
//

#include "randomGraph.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>

RandomGraph::RandomGraph(int vertCount, int edgeCount, std::string distributionType) {
    srand (time(NULL));
    numConflicts = new int[vertCount + 1];
    for (int i =0; i <= vertCount; i++) {
        numConflicts[i] = 0;
    }
    // dont allow for more edges than a complete graph
    if (edgeCount >= ((vertCount) * (vertCount- 1)) / 2) {
        edgeCount = (vertCount) * (vertCount - 1) / 2;
    }
    adjList = new LinkedList<int> [vertCount + 1];
    for (int i = 0; i < edgeCount;) {
            int randNum = GetRandomNumber(vertCount, distributionType);
            int attachEdge = GetRandomNumber(vertCount, distributionType);
            if (randNum != attachEdge && !adjList[randNum].contains(attachEdge)) {
                adjList[attachEdge].push_back(randNum);
                adjList[randNum].push_back(attachEdge);
                 i += 1;
                 numConflicts[attachEdge] += 1;
                 numConflicts[randNum] += 1;
            }
    }
}

int RandomGraph::GetRandomNumber(int vertCount, std::string distributionType) {
    if (randomDistribution == nullptr) {
        std::vector<int> debugVector;
        randomDistribution = new int[vertCount * vertCount];
        if (distributionType == "Skewed")  {// Skewed
            for (int i = 1; i <= vertCount; i++) {
                for (int j = vertCount; j >= i; j--) {
                    randomDistribution[distrSize++] = i;
                    debugVector.push_back(i);
                }
            }
        } else if (distributionType == "Random")  {// Random
            for (int i = 1; i <= vertCount; i++) {
                randomDistribution[distrSize++] = i;
            }
        }  else { // Tiered
                // first quarter is 80 %
                int firstQuarterEnd = vertCount / 4;
                int start = 1;
                int otherQuarterStart = firstQuarterEnd + 1;
                for (int i = 1; i <= (int)(vertCount * vertCount * .8); i++) {
                    randomDistribution[distrSize++] = start++;
                    debugVector.push_back(start - 1);
                    if (start > firstQuarterEnd) {
                        start = 1;
                    }
                }
                for (int i = 1; i <= (int)(vertCount * vertCount * .2); i++) {
                    randomDistribution[distrSize++] = otherQuarterStart++;
                    debugVector.push_back(otherQuarterStart - 1);
                    if (otherQuarterStart > vertCount) {
                        otherQuarterStart = firstQuarterEnd + 1;
                    }
                }
                // last three quarters is 20 %
        }
        distrSize--;
        for (int i = 1; i <= distrSize; i++) {
            std::swap(randomDistribution[i], randomDistribution[rand() % (distrSize - 1) + 1]);
        }
    }
    return randomDistribution[rand() % distrSize + 1];
}

int * RandomGraph::GetNumConflicts() {
    return numConflicts;
}

RandomGraph::~RandomGraph() {
    if (randomDistribution != nullptr) {
        delete [] randomDistribution;
    }
    delete numConflicts;
}