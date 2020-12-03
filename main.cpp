#include <iostream>
#include <chrono>
#include <string>
#include <vector>
#include <utility>
#include "completeGraph.h"
#include "cycleGraph.h"
#include "graphColorer.h"
#include "randomGraph.h"
#include "smallestLast.h"
#include "bfs.h"
#include "smallestOriginalDegree.h"
#include "firstFit.h"
#include "incidenceOrder.h"
#include "randomOrder.h"

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

// 0. time
// 1. colors used
// 2. size of terminal clique
// 3. degree when deleted vs order colored (1 entry is first item colored and value is its degree)
std::tuple<double, double, double, double *> orderAndTime(GraphColorer * algo, std::string fileName) {
    algo->readFile(fileName);
    auto t1 = std::chrono::high_resolution_clock::now();
    algo->order();
    auto t2 = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    int numVerts = algo->getNumVerts();
    double numColors = algo->color();
    double * orderingStats = new double [numVerts + 1];
    int * degreesWhenColored = algo->getDegreesWhenColored();
    for (int i = 1; i < numVerts + 1; i++) {
        orderingStats[i] = (double) degreesWhenColored[i];
    }
    return std::make_tuple(numColors, duration, algo->getTerminalCliqueSize(), orderingStats);
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
// 0. colors used
// 1. time
// 2. size of terminal clique
// 3. degree when deleted vs order colored (1 entry is first item colored and value is its degree)
std::tuple<double, double, double, double *> createGraphAndOrder(GraphAlgoType algoType, GraphType graphType, double vertCount) {
    double avgMaximumColors = 0;
    double avgTime = 0;
    double avgTerminalCliqueSize = 0;
    double * avgDegreesWhenColored = new double[vertCount + 1];
    for (int i =0; i < vertCount + 1; i++) {
        avgDegreesWhenColored[i] = 0;
    }
    for(int i = 0; i < 5; i += 1) {
        Graph * graph = createGraph(graphType, vertCount);
        graph->generateFile(vertCount, "output.txt");
        GraphColorer * algo = createAlgo(algoType);
        std::tuple<double, double, double, double*> colorInfo = orderAndTime(algo, "output.txt");
        avgTime += std::get<1>(colorInfo);
        avgMaximumColors += std::get<0>(colorInfo);
        avgTerminalCliqueSize += std::get<2>(colorInfo);
        double * degreesWhenColored = std::get<3>(colorInfo);
        for (int j = 1; j < vertCount + 1; j++) {
            avgDegreesWhenColored[j] += degreesWhenColored[j];
        }
        delete [] degreesWhenColored;
        delete graph;
        delete algo;
    }
    for (int i =1; i < vertCount + 1; i++) {
        avgDegreesWhenColored[i] /= 5;
    }
    return std::make_tuple(avgMaximumColors / 5, avgTime / 5, avgTerminalCliqueSize / 5, avgDegreesWhenColored);
}

// NOTE: THIS MAIN CREATES ALL OUTPUT FILES IT TAKES TIME TO RUN
int main( int argc, char ** argv) {
    double vertCount = 4;
    std::vector<double> verts;
    std::vector<double> edges[6];
    // 4 start and i < 7 for 256
    for (double i =0; i < 5; i++) {
        verts.push_back(vertCount);
        vertCount *= 2;
    }
    // timings
    std::vector<double> times[20];
    // 2-10 conflict counts
    std::vector<double> conflictSums[18][(int)(verts.back()) + 1];
    for (auto & vertices : verts) {
        std::cout << vertices << std::endl;
        auto t1 = std::chrono::high_resolution_clock::now();
        for (int i =0; i < 5; i++) {
            CompleteGraph complete(vertices);
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        times[0].push_back(duration / 5);
        std::cout << duration / 5 << std::endl;

        // 1
        t1 = std::chrono::high_resolution_clock::now();
        for (int i =0; i < 5; i++) {
            CycleGraph cycle(vertices);
        }
        t2 = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        times[1].push_back(duration / 5);
        std::cout << duration/ 5 << std::endl;

        // 2
        // RANDOM ITERATIONS
        std::string randomDistributions[3] = {"Random", "Skewed", "Tiered"};
        double completeCount = (vertices * (vertices - 1)) / 2;
        double vertCounts[6] = {0, vertices, completeCount * .25, completeCount * .5, completeCount * .75, completeCount};
        for (int j = 0; j < 6; j++) {
            edges[j].push_back(vertCounts[j]);
        }
        int randomCounter = 2;
        for(int j =0; j < 3; j++) {
            for(int k = 0; k < 6; k++) {
                int * sumConflicts = new int[(int)verts.back() + 1];
                for (int l = 0; l < (int)verts.back() + 1; l++) {
                    sumConflicts[l] = 0;
                }
                t1 = std::chrono::high_resolution_clock::now();
                for (int i =0; i < 5; i++) {
                    RandomGraph random(vertices, vertCounts[k], randomDistributions[j]);
                    int *conflicts = random.GetNumConflicts();
                    for (int l = 1; l < vertices + 1; l++) {
                        sumConflicts[l] += conflicts[l];
                    }
                }
                t2 = std::chrono::high_resolution_clock::now();
                duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
                for (int l = 1; l <= (int)verts.back(); l++) {
                    conflictSums[randomCounter - 2][l].push_back(sumConflicts[l] / 5);
                }
//                conflictSums[randomCounter - 2].push_back(sumConflicts / 5);
                times[randomCounter++].push_back(duration / 5);
                std::cout << "Duration " << randomDistributions[j] << " " << vertCounts[k] << " " << duration / 5
                 << " "  << std::endl;
                delete [] sumConflicts;
            }
        }
    }
    std::string randomNames[18] = {"Random - Random dist e = 0", "Random - Random dist e = V", "Random - Random dist e = (complete) * .25",
                                  "Random - Random dist e = (complete) * .5", "Random - Random dist e = (complete) * .75", "Random - Random dist e = complete",
                                  "Random - Skewed dist e = 0", "Random - Skewed dist e = V", "Random - Skewed dist e = (complete) * .25",
                                  "Random - Skewed dist e = (complete) * .5", "Random - Skewed dist e = (complete) * .75", "Random - Skewed dist e = complete",
                                  "Random - Tiered dist e = 0", "Random - Tiered dist e = V", "Random - Tiered dist e = (complete) * .25",
                                  "Random - Tiered dist e = (complete) * .5", "Random - Tiered dist e = (complete) * .75", "Random - Tiered dist e = complete",};
    std::vector<std::pair<std::string, std::vector<double>>> csvVals;
    csvVals.push_back(std::make_pair("", verts));
    csvVals.push_back(std::make_pair("Complete", times[0]));
    csvVals.push_back(std::make_pair("Cycle", times[1]));
    for (int i =0; i < 18; i++) {
        csvVals.push_back(std::make_pair("E", edges[i % 6]));
        csvVals.push_back(std::make_pair(randomNames[i], times[i + 2]));
    }
    writeCSV("graph_generation_times.csv", csvVals);
    csvVals.clear();

    for (int i =0; i < 18; i++) {
        std::vector<std::pair<std::string, std::vector<double>>> conflictVals;
        conflictVals.push_back(std::make_pair("", verts));
        for (int j =1; j <= (int)verts.back(); j++) {
            conflictVals.push_back(std::make_pair(randomNames[i] + ": " + std::to_string(j), conflictSums[i][j]));
        }
        writeCSV(randomNames[i] + " conflicts.csv", conflictVals);

    }
    // coloring section
    // 0. complete
    // 1. cycle
    // 2. random e = 0
    // 3. random e = v
    // 4. random e = (v * (v-1))/2
    std::string coloringMethods[8] = {"Complete", "Cycle", "Random e = 0", "Random e = v",
                                      "Random e = (complete) * .25", "Random e = (complete) * .5",
                                      "Random e = (complete) * .75", "Random e = (complete)"};
    std::vector<double> coloringTimes[8];
    std::string basicOutputName = "output.txt";
    for (auto & vertices : verts) {
        CompleteGraph completeGraph(vertices);
        completeGraph.generateFile(vertices, basicOutputName);
        // now color five times
        double totalTime = 0;
        for (int i =0; i < 6; i++) {
            totalTime += colorAndTime(basicOutputName);
        }
        std::cout << 0 << " " << totalTime / 5 << std::endl;
        coloringTimes[0].push_back(totalTime / 5);
        totalTime = 0;
        CycleGraph cycleGraph(vertices);
        cycleGraph.generateFile(vertices,basicOutputName);
        for (int i =0; i < 5; i++) {
            totalTime += colorAndTime(basicOutputName);
        }
        std::cout << 1 << " " << totalTime / 5 << std::endl;
        coloringTimes[1].push_back(totalTime / 5);
        double completeCount = (vertices * (vertices - 1)) / 2;
        double vertCounts[6] = {0, vertices, completeCount * .25,
                                completeCount * .5, completeCount * .75, completeCount};
        for (int i = 0; i < 6; i++) {
            RandomGraph randomGraph(vertices, vertCounts[i], "Random");
            randomGraph.generateFile(vertices, basicOutputName);
            totalTime = 0;
            for (int j = 0; j < 5; j++) {
                totalTime += colorAndTime(basicOutputName);
            }
            std::cout << i + 2 << " " << totalTime / 5 << std::endl;
            coloringTimes[i + 2].push_back(totalTime / 5);
        }
    }
    for (int i = 0; i < 7; i++) {
        if (i >= 2) {
            // push random edge counts on
            csvVals.push_back(std::make_pair("E", edges[i-2]));
        }
        csvVals.push_back(std::make_pair(coloringMethods[i], coloringTimes[i]));
    }

    writeCSV("order-timings.csv", csvVals);
    // first dimension is ordering algorithim
    // second dimenstion is graph type
    std::vector<double> avgColors[6][7];
    std::vector<double> avgTimes[6][7];
    std::vector<double> terminalCliqueSizes[7];
    std::vector<double> degreeWhenColored[7][(int)vertCount + 1];
    for (auto & vertices : verts) {
        std::cout << "ORDERING " << vertices << std::endl;
        for (int i =1; i <= 6; i++){
            for (int j = 1; j <= 7; j++) {
                GraphAlgoType algoType = static_cast<GraphAlgoType>(i);
                GraphType graphType = static_cast<GraphType>(j);
                std::tuple<double, double, double, double *> avgColorAvgTime = createGraphAndOrder(algoType, graphType, vertices);
                avgColors[i - 1][j - 1].push_back(std::get<0>(avgColorAvgTime));
                avgTimes[i - 1][j - 1].push_back(std::get<1>(avgColorAvgTime));
                // Terminal Clique info only for smallest last
                if (i == 2) {
                    terminalCliqueSizes[j - 1].push_back(std::get<2>(avgColorAvgTime));
                    double * degreesWhenDeleted = std::get<3>(avgColorAvgTime);
                    for (int l = 1; l < vertCount + 1; l++) {
                        if (l <= vertices) {
                            degreeWhenColored[j - 1][l].push_back(degreesWhenDeleted[l]);
                        } else {
                            degreeWhenColored[j - 1][l].push_back(0);
                        }
                    }
                }
                delete [] std::get<3>(avgColorAvgTime);
            }
        }
    }
    csvVals.clear();
    std::string orderMethods[6] = { "BFS ", "SmallestLast ", "IncidenceDegree ", "FirstFit ", "Random ", "SmallestOriginalDegreeLast "};
    std::string graphMethods[7] = { "Complete", "Cycle", "Random E = 0", "Random E = V", "Random E = (complete) * .25",
                                    "Random E = (complete) * .5", "Random E = (complete) * .75"};
    for (int i = 0; i < 6; i++) {
        csvVals.push_back(std::make_pair("V", verts));
        for (int j =0; j < 7; j++){
            if (j >= 2) {
                csvVals.push_back(std::make_pair("E", edges[j - 2]));
            }
            csvVals.push_back(std::make_pair(orderMethods[i] + graphMethods[j] + " - average time", avgTimes[i][j]));
        }
        writeCSV(orderMethods[i] + "- orderingMethodsTimes.csv", csvVals);
        csvVals.clear();
        csvVals.push_back(std::make_pair("V", verts));
        for (int j =0; j < 7; j++){
            if (j >= 2) {
                csvVals.push_back(std::make_pair("E", edges[j - 2]));
            }
            csvVals.push_back(std::make_pair(orderMethods[i] + graphMethods[j] + " - average Colors", avgColors[i][j]));
        }
        writeCSV(orderMethods[i] + "- orderingMethodsColors.csv", csvVals);
        csvVals.clear();

        if (i == 1) {
            // terminal clique info and degree when colored on here
            csvVals.push_back(std::make_pair("V", verts));

            for (int j =0; j < 7; j++){
                if (j >= 2) {
                    csvVals.push_back(std::make_pair("E", edges[j - 2]));
                }
                csvVals.push_back(std::make_pair(orderMethods[i] + graphMethods[j] + " - average terminal clique size", terminalCliqueSizes[j]));
            }
            writeCSV("SmallestLastTerminalCliques.csv", csvVals);
            csvVals.clear();
            csvVals.push_back(std::make_pair("V", verts));

            for (int j =0; j < 7; j++){
                for (int l = 1; l < vertCount + 1; l++) {
                    csvVals.push_back(std::make_pair(std::to_string(l), degreeWhenColored[j][l]));
                }
                writeCSV(graphMethods[j] + "-degreeWhenColored.csv", csvVals);
                csvVals.clear();
            }
        }
    }
    return 0;
}
