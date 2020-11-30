#include <iostream>
#include <chrono>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <utility>
#include "completeGraph.h"
#include "cycleGraph.h"
#include "graphColorer.h"
#include "randomGraph.h"
#include "smallestLast.h"
#include "smallestLast.h"
#include "bfs.h"
#include "smallestOriginalDegree.h"
#include "firstFit.h"
#include "incidenceOrder.h"
#include "randomOrder.h"
// date 9/30 time 1:20 for guide on implementation
// a way to do it in constant time is to create another hash

// ALL CREDITS FOR THIS METHOD GO TO
//https://waterprogramming.wordpress.com/2016/08/12/a-quick-example-code-to-write-data-to-a-csv-file-in-c/
void writeCSV(std::string filename, std::vector<std::pair<std::string, std::vector<double>>> dataset){
    // Make a CSV file with one or more columns of doubleeger values
    // Each column of data is represented by the pair <column name, column data>
    //   as std::pair<std::string, std::vector<double>>
    // The dataset is represented as a vector of these columns
    // Note that all columns should be the same size

    // Create an output filestream object
    std::ofstream myFile(filename);

    // Send column names to the stream
    for(double j = 0; j < dataset.size(); ++j)
    {
        myFile << dataset.at(j).first;
        if(j != dataset.size() - 1) myFile << ","; // No comma at end of line
    }
    myFile << "\n";

    // Send data to the stream
    for(double i = 0; i < dataset.at(0).second.size(); ++i)
    {
        for(double j = 0; j < dataset.size(); ++j)
        {
            myFile << dataset.at(j).second.at(i);
            if(j != dataset.size() - 1) myFile << ","; // No comma at end of line
        }
        myFile << "\n";
    }

    // Close the file
    myFile.close();
}

double colorAndTime(std::string fileName) {
    SmallestLast algo;
    algo.readFile(fileName);
    algo.order();
    auto t1 = std::chrono::high_resolution_clock::now();
    algo.color();
    auto t2 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
}

// returns 1. the time and 2. the number of colors
std::pair<double, double> orderAndTime(GraphColorer * algo, std::string fileName) {
    algo->readFile(fileName);
    auto t1 = std::chrono::high_resolution_clock::now();
    algo->order();
    auto t2 = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    return std::make_pair(algo->color(), duration);
}

enum GraphAlgoType { BFS = 1, SL = 2, ID = 3, FF = 4, R = 5, SODL = 6};
enum GraphType { COMPLETE = 1, CYCLE = 2, RANDOMEMPTY = 3, RANDOMSMALL = 4, RANDOMTWENTYFIVE = 5,
        RANDOMFIFTY = 6, RANDOMSEVENTYFIVE = 7};

GraphColorer * createAlgo(GraphAlgoType algoType) {
    switch (algoType) {
        case BFS:
            return new Bfs();
        case SL:
            return new SmallestLast();
        case ID:
            return new IncidenceOrder();
        case FF:
            return new FirstFit();
        case R:
            return new RandomOrder();
        case SODL:
            return new SmallestOriginalDegree();
        default:
            return NULL;
    }
}

Graph * createGraph(GraphType graphType, double vertCount) {
    switch (graphType) {
        case COMPLETE:
            return new CompleteGraph(vertCount);
        case CYCLE:
            return new CycleGraph(vertCount);
        case RANDOMEMPTY:
            return new RandomGraph(vertCount, 0, "Random");
        case RANDOMSMALL:
            return new RandomGraph(vertCount, vertCount, "Random");
        case RANDOMTWENTYFIVE:
            return new RandomGraph(vertCount, ((vertCount * (vertCount - 1)) / 2) * .25, "Random");
        case RANDOMFIFTY:
            return new RandomGraph(vertCount, ((vertCount * (vertCount - 1)) / 2) * .5, "Random");
        case RANDOMSEVENTYFIVE:
            return new RandomGraph(vertCount, ((vertCount * (vertCount - 1)) / 2) * .75, "Random");
        default:
            return NULL;
    }
}
std::pair<double, double> createGraphAndOrder(GraphAlgoType algoType, GraphType graphType, double vertCount) {
    double avgMaximumColors = 0;
    double avgTime = 0;
    for(double i = 0; i < 5; i += 1) {
        Graph * graph = createGraph(graphType, vertCount);
        graph->generateFile(vertCount, "output.txt");
        GraphColorer * algo = createAlgo(algoType);
        std::pair<double, double> colorsAndTimes = orderAndTime(algo, "output.txt");
        avgMaximumColors += colorsAndTimes.first;
        avgTime += colorsAndTimes.second;
        delete graph;
        delete algo;
    }
    return std::make_pair(avgMaximumColors / 5, avgTime / 5);
}


int main( int argc, char ** argv) {
    double vertCount = 4;
    std::vector<double> verts;
    // 4 start and i < 7 for 256
    for (double i =0; i< 5; i++) {
        verts.push_back(vertCount);
        vertCount *= 2;
    }
    // timings
//    std::vector<double> times[20];
//    // 2-10 conflict counts
//    std::vector<double> conflictSums[18];
//    for (auto & vertices : verts) {
//        std::cout << vertices << std::endl;
//        auto t1 = std::chrono::high_resolution_clock::now();
//        for (int i =0; i < 5; i++) {
//            CompleteGraph complete(vertices);
//        }
//        auto t2 = std::chrono::high_resolution_clock::now();
//        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
//        times[0].push_back(duration / 5);
//        std::cout << duration / 5 << std::endl;
//
//        // 1
//        t1 = std::chrono::high_resolution_clock::now();
//        for (int i =0; i < 5; i++) {
//            CycleGraph cycle(vertices);
//        }
//        t2 = std::chrono::high_resolution_clock::now();
//        duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
//        times[1].push_back(duration / 5);
//        std::cout << duration/ 5 << std::endl;
//
//        // 2
//        // RANDOM ITERATIONS
//        std::string randomDistributions[3] = {"Random", "Skewed", "Tiered"};
//        double completeCount = (vertices * (vertices - 1)) / 2;
//        double vertCounts[6] = {0, vertices, completeCount * .25, completeCount * .5, completeCount * .75, completeCount};
//        int randomCounter = 2;
//        for(int j =0; j < 3; j++) {
//            for(int k = 0; k < 6; k++) {
//                int sumConflicts = 0;
//                t1 = std::chrono::high_resolution_clock::now();
//                for (int i =0; i < 5; i++) {
//                    RandomGraph random(vertices, vertCounts[k], randomDistributions[j]);
//                    sumConflicts += random.GetNumConflicts();
//                }
//                t2 = std::chrono::high_resolution_clock::now();
//                duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
//                conflictSums[randomCounter - 2].push_back(sumConflicts / 5);
//                times[randomCounter++].push_back(duration / 5);
//                std::cout << "Duration " << randomDistributions[j] << " " << vertCounts[k] << " " << duration / 5
//                 << " " << sumConflicts / 5 << std::endl;
//            }
//        }
//    }
//    std::string randomNames[18] = {"Random - Random dist e = 0", "Random - Random dist e = V", "Random - Random dist e = (V(V-1) / 2) * .25",
//                                  "Random - Random dist e = (V(V-1) / 2) * .5", "Random - Random dist e = (V(V-1) / 2) * .75", "Random - Random dist e = V(V-1) / 2",
//                                  "Random - Skewed dist e = 0", "Random - Skewed dist e = V", "Random - Skewed dist e = (V(V-1) / 2) * .25",
//                                  "Random - Skewed dist e = (V(V-1) / 2) * .5", "Random - Skewed dist e = (V(V-1) / 2) * .75", "Random - Skewed dist e = V(V-1) / 2",
//                                  "Random - Tiered dist e = 0", "Random - Tiered dist e = V", "Random - Tiered dist e = (V(V-1) / 2) * .25",
//                                  "Random - Tiered dist e = (V(V-1) / 2) * .5", "Random - Tiered dist e = (V(V-1) / 2) * .75", "Random - Tiered dist e = V(V-1) / 2",};
    std::vector<std::pair<std::string, std::vector<double>>> csvVals;
//    csvVals.push_back(std::make_pair("", verts));
//    csvVals.push_back(std::make_pair("Complete", times[0]));
//    csvVals.push_back(std::make_pair("Cycle", times[1]));
//    for (int i =0; i < 18; i++) {
//        csvVals.push_back(std::make_pair(randomNames[i], times[i + 2]));
//        csvVals.push_back(std::make_pair((randomNames[i] + " - Conflict"), conflictSums[i]));
//    }
//
//    // coloring section
//    // 0. complete
//    // 1. cycle
//    // 2. random e = 0
//    // 3. random e = v
//    // 4. random e = (v * (v-1))/2
//    std::string coloringMethods[8] = {"Complete", "Cycle", "Random e = 0", "Random e = v",
//                                      "Random e = ((v * (v-1)) / 2) * .25", "Random e = ((v * (v-1)) / 2) * .5",
//                                      "Random e = ((v * (v-1)) / 2) * .75", "Random e = (v * (v-1)) / 2"};
//    std::vector<double> coloringTimes[8];
//    std::string basicOutputName = "output.txt";
//    for (auto & vertices : verts) {
//        CompleteGraph completeGraph(vertices);
//        completeGraph.generateFile(vertices, basicOutputName);
//        // now color five times
//        double totalTime = 0;
//        for (int i =0; i < 5; i++) {
//            totalTime += colorAndTime(basicOutputName);
//        }
//        std::cout << 0 << " " << totalTime / 5 << std::endl;
//        coloringTimes[0].push_back(totalTime / 5);
//        totalTime = 0;
//        CycleGraph cycleGraph(vertices);
//        cycleGraph.generateFile(vertices,basicOutputName);
//        for (int i =0; i < 5; i++) {
//            totalTime += colorAndTime(basicOutputName);
//        }
//        std::cout << 1 << " " << totalTime / 5 << std::endl;
//        coloringTimes[1].push_back(totalTime / 5);
//        totalTime = 0;
//        double completeCount = (vertices * (vertices - 1)) / 2;
//        double vertCounts[6] = {0, vertices, completeCount * .25,
//                                completeCount * .5, completeCount * .75, completeCount};
//        for (int i = 0; i < 6; i++) {
//            RandomGraph randomGraph(vertices, vertCounts[i], "Random");
//            randomGraph.generateFile(vertices, basicOutputName);
//            for (int j = 0; j < 5; j++) {
//                totalTime += colorAndTime(basicOutputName);
//            }
//            std::cout << i + 2 << " " << totalTime / 5 << std::endl;
//            coloringTimes[i + 2].push_back(totalTime / 5);
//        }
//    }
//    for (int i = 0; i < 8; i++) {
//        csvVals.push_back(std::make_pair(coloringMethods[i], coloringTimes[i]));
//    }
//
//    csvVals.push_back(std::make_pair("Complete", times[0]));
//    csvVals.push_back(std::make_pair("Cycle", times[1]));
//    for (int i =0; i < 18; i++) {
//        csvVals.push_back(std::make_pair(randomNames[i], times[i + 2]));
//        csvVals.push_back(std::make_pair((randomNames[i] + " - Conflict"), conflictSums[i]));
//    }
//    writeCSV("timings.csv", csvVals);
////    // first dimension is ordering algorithim
////    // second dimenstion is graph type
    std::vector<double> avgColors[6][7];
    std::vector<double> avgTimes[6][7];
    for (auto & vertices : verts) {
        std::cout << "ORDERING " << vertices << std::endl;
        for (int i =1; i <= 6; i++){
            for (int j = 1; j <= 7; j++) {
                GraphAlgoType algoType = static_cast<GraphAlgoType>(i);
                GraphType graphType = static_cast<GraphType>(j);
                std::pair<double, double> avgColorAvgTime = createGraphAndOrder(algoType, graphType, vertices);
                avgColors[i - 1][j - 1].push_back(avgColorAvgTime.first);
                avgTimes[i - 1][j - 1].push_back(avgColorAvgTime.second);
            }
        }
    }
    csvVals.clear();
    std::string orderMethods[6] = { "BFS ", "SmallestLast ", "IncidenceDegree ", "FirstFit ", "Random ", "SmallestOriginalDegreeLast "};
    std::string graphMethods[7] = { "Complete", "Cycle", "Random E = 0", "Random E = V", "Random E = ((V * (V-1))/2) * .25",
                                    "Random E = ((V * (V-1))/2) * .5", "Random E = ((V * (V-1))/2) * .75"};
    csvVals.push_back(std::make_pair("", verts));
    for (int i = 0; i < 6; i++) {
        for (int j =0; j < 7; j++){
            csvVals.push_back(std::make_pair(orderMethods[i] + graphMethods[j] + " - average Colors", avgColors[i][j]));
            csvVals.push_back(std::make_pair(orderMethods[i] + graphMethods[j] + " - average time", avgTimes[i][j]));
        }
    }
    writeCSV("orderingMethods.csv", csvVals);
    return 0;
}
