#include "aliases.h"
#include "utils.h"
#include "graphGenerators.h"
#include "testbench.h"

int main() {
    unsigned int num_vertices = 400;
    unsigned int num_edges = 800;
    unsigned int num_queries = 200;

    std::fstream csv;
    csv.open(TIMES_FILE, std::fstream::in | std::fstream::out | std::fstream::app);
    if(!csv.is_open()) {
        std::cout << "Something went wrong with the .csv file!" << std::endl;
        // return 1;
    }
    // Create the AIMN91 Datastructure
    std::cout << "Setting up the AIMN91 data structure (adds edges)..." << std::endl;
    AIMN91_DataStructure AIMN91(num_vertices);

    // Generate the starting edges for the AIMN91 data structure
    std::cout << "Generating random edges..." << std::endl;
    auto random_edges = createRandomEdges(num_vertices, num_edges);
    std::cout << "Edges are ready..." << std::endl;

    // Add num_edges edges to the graph
    for(auto it = random_edges.begin(); it != random_edges.end(); ++it) {
        AIMN91.add(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
    }
    std::cout << "done..." << std::endl;
    
    // Create more random edges to add into the graph as queries
    std::cout << "Generating random queries..." << std::endl;
    auto random_queries = createRandomQueries(*AIMN91.get_graph(), num_queries);
    std::cout << "Random queries ready..." << std::endl;

    // Time
    time_me_driver("decrease", AIMN91, random_queries, "random", csv);
    time_me_driver("minpath", AIMN91, random_queries, "random", csv);
    time_me_driver("length", AIMN91, random_queries, "random", csv);
    time_me_driver("add", AIMN91, random_queries, "random", csv);

    csv.close();

/*     std::cout << "Generating random edges..." << std::endl;
    auto edges = createRandomEdges(num_vertices, num_edges);
    std::cout << "Edges are ready..." << std::endl;
    time_with_floyd_warshall_sp(num_vertices, edges, "random"); */

}
