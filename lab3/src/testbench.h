#include "aliases.h"
#include "AIMN91_DataStructure.h"
#include <fstream>
#include <chrono>
#include <boost/graph/copy.hpp>

// time(ns), typeOfGraph, operation, numberOfNodes, numberOfEdges, algorithm
struct Durations {
    std::chrono::duration<double, std::nano> time;
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

void time_add(
    AIMN91_DataStructure& AIMN91,
    std::string typeOfGraph,
    std::tuple<Vertex, Vertex, int> edge,
    std::fstream& csv)
{
    Graph G;
    boost::copy_graph(*AIMN91.get_graph(), G);
    
    Durations dur(typeOfGraph, "add", boost::num_vertices(G),
        boost::num_edges(G) + 1, "AIMN91");
    auto t0 = std::chrono::high_resolution_clock::now();
    AIMN91.add(std::get<0>(edge), std::get<1>(edge), std::get<2>(edge));
    auto t1 = std::chrono::high_resolution_clock::now();
    dur.time = t1 - t0;
    dur.save_into_csv(csv);

    dur.updateInfo(typeOfGraph, "add", boost::num_vertices(G),
        boost::num_edges(G) + 1, "BOOST");
    auto t2 = std::chrono::high_resolution_clock::now();
    auto e = boost::add_edge(std::get<0>(edge), std::get<1>(edge), G);
    boost::put(boost::get(&EdgeInfo::cost, G), e.first, std::get<2>(edge));
    auto t3 = std::chrono::high_resolution_clock::now();
    dur.time = t3 - t2;
    dur.save_into_csv(csv);
}

void time_minpath(
    AIMN91_DataStructure& AIMN91,
    const std::vector<std::pair<Vertex, Vertex> > queries, 
    std::string typeOfGraph,
    std::fstream& csv)
{

}

void time_length(
    AIMN91_DataStructure& AIMN91,
    const std::vector<std::pair<Vertex, Vertex> > queries, 
    std::string typeOfGraph,
    std::fstream& csv)
{

}

void time_decrease(
    Graph& G, 
    AIMN91_DataStructure& AIMN91,
    const std::vector<std::pair<Vertex, Vertex> > queries, 
    std::string typeOfGraph,
    std::fstream& csv)
{

}