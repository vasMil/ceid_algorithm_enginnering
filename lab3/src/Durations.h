#pragma once
#include "aliases.h"
#include <fstream>
#include <chrono>

// time(ns), typeOfGraph, operation, numberOfNodes, numberOfEdges, algorithm
template<typename time_unit_t>
struct Durations {
    std::chrono::duration<double, time_unit_t> time;
    std::string typeOfGraph;
    std::string operation;
    int numOfNodes;
    int numOfEdges;
    std::string algorithm;

    Durations(std::string typeOfGraph, std::string operation, int n, int m, std::string algorithm) {
        this->typeOfGraph = typeOfGraph;
        this->operation = operation;
        this->numOfNodes = n;
        this->numOfEdges = m;
        this->algorithm = algorithm;
    }

    void updateInfo(std::string typeOfGraph, std::string operation, int n, int m, std::string algorithm) {
        this->typeOfGraph = typeOfGraph;
        this->operation = operation;
        this->numOfNodes = n;
        this->numOfEdges = m;
        this->algorithm = algorithm;
        this->time = (std::chrono::duration<double, std::nano>)0;
    }

    void save_into_csv(std::fstream& csv) {
        // Create the output file if it does not exist and format it accordingly
        createFileIfNotExists(csv);
        // Write the measurment - add a newline at the end
        csv << time.count() << "," << typeOfGraph << "," << operation << "," << numOfNodes << "," 
            << numOfEdges << "," << algorithm << std::endl;
    }

    private:
    void createFileIfNotExists(std::fstream& csv) {
        csv.seekg(0);
        // If file is not empty
        if(csv.peek() != EOF) {
            // Get back at the end of the file
            csv.seekg(std::ios::end);
            return;
        }
        // Clear the flags (I only care about eof flag)
        csv.clear();
        // Get at the beggining of the file
        csv.seekg(0);
        // Else create and setup the first line - add a newline at the end
        csv << "time,typeOfGraph,operation,numberOfNodes,numberOfEdges,algorithm" << std::endl;
    }
};