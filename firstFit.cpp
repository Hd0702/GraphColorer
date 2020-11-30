//
// Created by Hayden Donofrio on 11/25/20.
//

#include "firstFit.h"

void FirstFit::order() {
    for (int i = 1; i <= getNumVerts(); i++) {
        // Viola!
        colorOrder.push_back(i);
    }
}