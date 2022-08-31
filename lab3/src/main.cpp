#include "aliases.h"
#include "utils.h"
#include "graphGenerators.h"
#include "testbench.h"

int main() {
    unsigned int num_vertices = 10;
    unsigned int num_edges = 20;

    std::cout << "Generating random edges..." << std::endl;
    auto random_edges = createRandomEdges(num_vertices, num_edges);
    std::cout << "Edges are ready..." << std::endl;

    // std::cout << "Inserting the random edges into the AIMN91 data structure..." << std::endl;
    // // Insert them in the graph
    // AIMN91_DataStructure AIMN91(num_vertices);
    // for(auto it = random_edges.begin(); it != random_edges.end(); ++it) {
    //     // std::cout << std::get<0>(*it) << "->" << std::get<1>(*it) << "[label=" << std::get<2>(*it) << "]" << std::endl;
    //     AIMN91.add(std::get<0>(*it), std::get<1>(*it), std::get<2>(*it));
    // }
    // std::cout << "AIMN91 is ready..." << std::endl;
    
    // /* Testing */
    // // test_directed_graph_no_weight();
    // cli();

    std::fstream csv;
    csv.open(TIMES_FILE, std::fstream::in | std::fstream::out | std::fstream::app);
    if(!csv.is_open()) {
        std::cout << "Something went wrong with the .csv file!" << std::endl;
        return 1;
    }
    AIMN91_DataStructure AIMN91(num_vertices);
    for(auto it = random_edges.begin(); it != random_edges.end(); ++it) {
        time_add(AIMN91, "random", *it, csv);
    }

    csv.close();
    
}
