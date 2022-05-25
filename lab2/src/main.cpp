#include "utils.h"
#include "graphGenerators.h"
#include "algorithms.h"
#include "test.h"
#include "testbench.h"

int main() {
    std::vector<std::pair<int, int> > gridGraphVec = {{30, 1000}, {60, 1000}, {80, 1000}};
    std::vector<std::pair<int, int> > randomGraphVec = {{10000, 20000}, {20000, 40000}, {60000, 120000}};
    std::vector<std::pair<int, int> > costBounds = {{1, 100}, {1, 10000}};
    std::fstream boostCsv, ledaCsv;
    boostCsv.open("boost_times.csv", std::fstream::in | std::fstream::out | std::fstream::app);
    ledaCsv.open("leda_times.csv", std::fstream::in | std::fstream::out | std::fstream::app);
    if(!boostCsv.is_open()) {
        std::cout << "Cannot open boost_times.csv!" << std::endl;
        return 0;
    }
    if(!ledaCsv.is_open()) {
        std::cout << "Cannot open leda_times.csv!" << std::endl;
        return 0;
    }
    for (int i=0; i<costBounds.size(); i++) {
        for (int j=0; j < gridGraphVec.size(); j++) {
            std::cout << "timeMe on random graph with " << randomGraphVec[j].first << " vertices and " 
                << randomGraphVec[j].second << " edges | Cost in [" << costBounds[i].first << ", " << costBounds[i].second << "]" << std::endl;
            timeMe(boostCsv, ledaCsv, "random", &randomGraph, randomGraphVec[j].first, randomGraphVec[j].second, costBounds[i].first, costBounds[i].second);
            std::cout << "timeMe on grid graph with " << gridGraphVec[j].first << " rows and " 
                << gridGraphVec[j].second << " colums | Cost in [" << costBounds[i].first << ", " << costBounds[i].second << "]" << std::endl;
            timeMe(boostCsv, ledaCsv, "grid", &gridGraph, gridGraphVec[j].first, gridGraphVec[j].second, costBounds[i].first, costBounds[i].second);
        }
    }

    boostCsv.close();
    ledaCsv.close();
    return 0;
}