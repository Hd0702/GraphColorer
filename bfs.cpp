//
// Created by Hayden Donofrio on 11/26/20.
//

#include "bfs.h"

void Bfs::order() {
    int numVerts = getNumVerts();
    LinkedList<int> notDeleted;
    for (int i = 1; i <= numVerts; i++) {
        adjList[i].edgeListEntry = &notDeleted.push_front(i);
    }
    LinkedList<int> queue;
    int currentItem = queue.push_front(notDeleted.front()->item).item;
    queue.pop_front();
    colorOrder.push_front(notDeleted.front()->item);
    notDeleted.pop_front();
    adjList[currentItem].deleted = true;
    while(colorOrder.length() < numVerts) {
        LinkedList<int>::node * neighbor = adjList[currentItem].vertices.front();
         for (int i = 0; i < adjList[currentItem].vertices.length(); i++){
             if (!adjList[neighbor->item].deleted) {
                 adjList[neighbor->item].deleted = true;
                 notDeleted.remove(adjList[neighbor->item].edgeListEntry);
                 queue.push_front(neighbor->item);
                 colorOrder.push_back(neighbor->item);
             }
             neighbor = neighbor->next;
         }
         if (queue.length() > 0) {
             currentItem = queue.front()->item;
             queue.pop_front();
         }
         else {
             // get a new separate section of the graph
             currentItem = notDeleted.front()->item;
             adjList[currentItem].deleted = true;
             colorOrder.push_back(currentItem);
             notDeleted.pop_front();
         }
    }
}