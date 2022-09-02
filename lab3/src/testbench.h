#include "aliases.h"
#include "AIMN91_DataStructure.h"
#include <fstream>
#include <chrono>
#include <boost/graph/copy.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>

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
    const std::tuple<Vertex, Vertex, int> edge,
    const std::string typeOfGraph,
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
        boost::num_edges(G) + 1, "DIJKSTRA");
    auto t2 = std::chrono::high_resolution_clock::now();
    auto e = boost::add_edge(std::get<0>(edge), std::get<1>(edge), G);
    boost::put(boost::get(&EdgeInfo::cost, G), e.first, std::get<2>(edge));
    auto t3 = std::chrono::high_resolution_clock::now();
    dur.time = t3 - t2;
    dur.save_into_csv(csv);

    dur.updateInfo(typeOfGraph, "add", boost::num_vertices(G),
    boost::num_edges(G) + 1, "DIJKSTRA");
}

void time_minpath(
    AIMN91_DataStructure& AIMN91,
    const std::pair<Vertex, Vertex> query, 
    const std::string typeOfGraph,
    std::fstream& csv)
{
    Graph G = *AIMN91.get_graph();

    // Run AIMN91
    Durations dur(typeOfGraph, "minpath", boost::num_vertices(G),
        boost::num_edges(G), "AIMN91");
    auto t0 = std::chrono::high_resolution_clock::now();
    AIMN91.minpath(query.first, query.second);
    auto t1 = std::chrono::high_resolution_clock::now();
    dur.time = t1 - t0;
    dur.save_into_csv(csv);

    // Run Dijkstra
    dur.updateInfo(typeOfGraph, "minpath", boost::num_vertices(G),
        boost::num_edges(G), "DIJKSTRA");
    // Create a predecessor map
    std::vector<Vertex> predVec(boost::num_vertices(G));
    auto t2 = std::chrono::high_resolution_clock::now();
    boost::dijkstra_shortest_paths(G, query.first, 
        boost::weight_map(boost::get(&EdgeInfo::cost, G)).
        predecessor_map(&predVec[0])
    );
    auto t3 = std::chrono::high_resolution_clock::now();
    dur.time = t3 - t2;
    dur.save_into_csv(csv);
}

void time_length(
    AIMN91_DataStructure& AIMN91,
    const std::pair<Vertex, Vertex> query, 
    std::string typeOfGraph,
    std::fstream& csv)
{
    Graph G = *AIMN91.get_graph();

    // Run AIMN91
    Durations dur(typeOfGraph, "length", boost::num_vertices(G),
        boost::num_edges(G), "AIMN91");
    auto t0 = std::chrono::high_resolution_clock::now();
    AIMN91.length(query.first, query.second);
    auto t1 = std::chrono::high_resolution_clock::now();
    dur.time = t1 - t0;
    dur.save_into_csv(csv);

    // Run Dijkstra
    dur.updateInfo(typeOfGraph, "length", boost::num_vertices(G),
        boost::num_edges(G), "DIJKSTRA");
    // Create a distance map
    std::vector<unsigned int> distVec(boost::num_vertices(G));
    auto t2 = std::chrono::high_resolution_clock::now();
    boost::dijkstra_shortest_paths(G, query.first, 
        boost::weight_map(boost::get(&EdgeInfo::cost, G)).
        distance_map(
            boost::make_iterator_property_map(
            distVec.begin(), get(boost::vertex_index, G)))
    );
    auto t3 = std::chrono::high_resolution_clock::now();
    dur.time = t3 - t2;
    dur.save_into_csv(csv);
}

void time_decrease(
    AIMN91_DataStructure& AIMN91,
    const std::tuple<Vertex, Vertex, int> query, 
    const std::string typeOfGraph,
    std::fstream& csv)
{
    Graph G;
    boost::copy_graph(*AIMN91.get_graph(), G);
    
    Durations dur(typeOfGraph, "decrease", boost::num_vertices(G),
        boost::num_edges(G) + 1, "AIMN91");
    auto t0 = std::chrono::high_resolution_clock::now();
    AIMN91.decrease(std::get<0>(query), std::get<1>(query), std::get<2>(query));
    auto t1 = std::chrono::high_resolution_clock::now();
    dur.time = t1 - t0;
    dur.save_into_csv(csv);

    dur.updateInfo(typeOfGraph, "decrease", boost::num_vertices(G),
        boost::num_edges(G) + 1, "DIJKSTRA");
    auto t2 = std::chrono::high_resolution_clock::now();
    auto e = boost::edge(std::get<0>(query), std::get<1>(query), G);
    boost::put(boost::get(&EdgeInfo::cost, G), e.first, std::get<2>(query));
    auto t3 = std::chrono::high_resolution_clock::now();
    dur.time = t3 - t2;
    dur.save_into_csv(csv);

    dur.updateInfo(typeOfGraph, "add", boost::num_vertices(G),
    boost::num_edges(G) + 1, "DIJKSTRA");
}

// Given the name of the operation and the number of queries
// execute the desired time_* function
// on a graph with num_vertices vertices and num_edges edges.
// Output the results using fstream csv
void time_me_driver(std::string operation, int num_queries, 
    int num_vertices, int num_edges, 
    std::fstream& csv) {
    std::cout << "\n\n/******** RUNNING TIME ME FUNCTION ********/" << std::endl;
    std::cout << "Generating random edges..." << std::endl;
    auto random_edges = createRandomEdges(num_vertices, num_edges);
    std::cout << "Edges are ready..." << std::endl;

    // Create the AIMN91 Datastructure
    std::cout << "Setting up the AIMN91 data structure (adds edges)..." << std::endl;
    AIMN91_DataStructure AIMN91(num_vertices);
    // Add num_edges edges to the graph
    for(auto it = random_edges.begin(); it != random_edges.end(); ++it) {
        AIMN91.add(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
    }
    std::cout << "done..." << std::endl;

    if(operation == "add") {
        // Create more random edges to add into the graph
        std::cout << "Generating more random edges..." << std::endl;
        auto random_queries = createRandomEdges(num_vertices, num_queries);
        std::cout << "Second batch of random edges is ready..." << std::endl;
        // Add them one by one - timing each insertion
        std::cout << "Running time_add()..." << std::endl;
        for(auto it = random_queries.begin(); it != random_queries.end(); ++it) {
            time_add(AIMN91, *it, "random", csv);
        }
        std::cout << "done..." << std::endl;
    }
    else {
        // Create more random edges to add into the graph
        std::cout << "Generating random queries..." << std::endl;
        auto random_queries = createRandomQueries(*AIMN91.get_graph(), num_queries);
        std::cout << "Random queries are ready..." << std::endl;

        if(operation == "minpath") {
            std::cout << "Starting minpath queries..." << std::endl;
            for(auto it = random_queries.begin(); it != random_queries.end(); ++it) {
                time_minpath(AIMN91, std::make_pair(std::get<0>(*it), std::get<1>(*it)), "random", csv);
                #if DEBUG
                    verifyUsingDijkstra(AIMN91, std::make_pair(std::get<0>(*it), std::get<1>(*it)));
                #endif
            }
            std::cout << "done..." << std::endl;
        }
        else if (operation == "length") {
            std::cout << "Starting length queries..." << std::endl;
            for(auto it = random_queries.begin(); it != random_queries.end(); ++it) {
                time_length(AIMN91, std::make_pair(std::get<0>(*it), std::get<1>(*it)), "random", csv);
                #if DEBUG
                    verifyUsingDijkstra(AIMN91, std::make_pair(std::get<0>(*it), std::get<1>(*it)));
                #endif
            }
            std::cout << "done..." << std::endl;
        }
        else if (operation == "decrease") {
            std::cout << "Starting decrease queries..." << std::endl;
            for(auto it = random_queries.begin(); it != random_queries.end(); ++it) {
                time_decrease(AIMN91, *it, "random", csv);
                #if DEBUG
                    verifyUsingDijkstra(AIMN91, std::make_pair(std::get<0>(*it), std::get<1>(*it)));
                #endif
            }
            std::cout << "done..." << std::endl;
        }
        else {
            std::cout << "Invalid operation..." << std::endl;
        }
    }
}

#if DEBUG
void verifyUsingDijkstra(AIMN91_DataStructure& AIMN91, std::pair<Vertex, Vertex> query) {
    Graph G = *AIMN91.get_graph();
    auto aimn_path = AIMN91.minpath(query.first, query.second);

    // Run the boost::disjkstra_shortest_paths and get the length of each path aswell,
    // since two minpaths may have different nodes in between but the same length
    // hence AIMN91 has choosen a different path than Dijkstra and has not made a mistake
    std::vector<Vertex> predVec(boost::num_vertices(G));
    std::vector<unsigned int> distVec(boost::num_vertices(G));
    boost::dijkstra_shortest_paths(G, query.first, 
        boost::weight_map(boost::get(&EdgeInfo::cost, G)).
        predecessor_map(&predVec[0]).
        distance_map(
            boost::make_iterator_property_map(
            distVec.begin(), get(boost::vertex_index, G)))
    );

    // Using the predVec to create a vector<Vertex> with the same format as the
    // one returned by AIMN91.minpath()
    std::vector<Vertex> dijkstra_path;
    Vertex s = query.second;
    dijkstra_path.push_back(s);
    while(query.first != s) {
        if(predVec[s] == s) {
            // There is no path from query.first to query.target, since s is
            // a source node
            dijkstra_path.clear();
            break;
        }        
        s = predVec[s];
        dijkstra_path.push_back(s);
    }
    std::reverse(dijkstra_path.begin(), dijkstra_path.end());

    // Check if the two solutions are equivelant
    if(dijkstra_path != aimn_path && 
        distVec[query.second] != AIMN91.length(query.first, query.second))
    {
        std::cout << "Dijkstra and AIMN91 disagree on query: " << 
            query.first << " -> " << query.second << std::endl;

        std::cout << "Dijkstra (length = " << distVec[query.second] << ")..." << std::endl;
        auto itt = dijkstra_path.begin();
        auto eitt = dijkstra_path.end();
        for( ; itt != eitt; itt++) {
            std::cout << (*itt) << std::endl;
        }

        std::cout << "AIMN91 (length = " << AIMN91.length(query.first, query.second) << ")..." << std::endl;
        auto it = aimn_path.begin();
        auto eit = aimn_path.end();
        for( ; it != eit; it++) {
            std::cout << (*it) << std::endl;
        }
    }
}
#endif