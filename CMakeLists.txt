cmake_minimum_required(VERSION 3.17)
project(HW3)

set(CMAKE_CXX_STANDARD 14)

add_executable(HW3 main.cpp completeGraph.cpp completeGraph.h cycleGraph.cpp cycleGraph.h randomGraph.cpp randomGraph.h graph.cpp graph.h linkedList.h graphColorer.cpp graphColorer.h smallestLast.cpp smallestLast.h smallestOriginalDegree.cpp smallestOriginalDegree.h randomOrder.cpp randomOrder.h incidenceOrder.cpp incidenceOrder.h firstFit.cpp firstFit.h bfs.cpp bfs.h)