//
// Created by Hayden Donofrio on 11/24/20.
//
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "randomOrder.h"

void RandomOrder::order() {
    int numVerts = getNumVerts();
    srand (time(NULL));
    // shuffle the order of the vertices
    int * shuffledVerts = new int[numVerts + 1];
    for (int i = 1; i <= numVerts; i++) {
        shuffledVerts[i] = i;
    }
    for (int i = 1; i <= numVerts; i++) {
        std::swap(shuffledVerts[i], shuffledVerts[rand() % numVerts + 1]);
    }
    for (int i =1; i <= numVerts; i++) {
        colorOrder.push_front(shuffledVerts[i]);
    }
}